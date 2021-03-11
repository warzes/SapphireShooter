#include "stdafx.h"
#include "AutoPtr.h"
#include "String.h"
#include "Vector.h"
#include "WString.h"
#include "File.h"
#include "FileSystem.h"
#include "Engine/DebugNew.h"
//-----------------------------------------------------------------------------
bool FileSystem::SetCurrentDir(const String& pathName)
{
#if SE_PLATFORM_WINDOWS
	if (SetCurrentDirectoryW(FileSystem::WideNativePath(pathName).CString()) == FALSE)
		return false;
#else
	if (chdir(FileSystem::NativePath(pathName).CString()) != 0)
		return false;
#endif

	return true;
}
//-----------------------------------------------------------------------------
bool FileSystem::CreateDir(const String& pathName)
{
#if SE_PLATFORM_WINDOWS
	const bool success = (CreateDirectoryW(FileSystem::WideNativePath(FileSystem::RemoveTrailingSlash(pathName)).CString(), 0) == TRUE) || (GetLastError() == ERROR_ALREADY_EXISTS);
#else
	const bool success = mkdir(FileSystem::NativePath(FileSystem::RemoveTrailingSlash(pathName)).CString(), S_IRWXU) == 0 || errno == EEXIST;
#endif

	return success;
}
//-----------------------------------------------------------------------------
bool FileSystem::CopyFile(const String& srcFileName, const String& destFileName)
{
	File srcFile(srcFileName, FileMode::Read);
	if (!srcFile.IsOpen())
		return false;
	File destFile(destFileName, FileMode::Write);
	if (!destFile.IsOpen())
		return false;

	// \todo Should use a fixed-size buffer to allow copying very large files
	const size_t fileSize = srcFile.Size();
	AutoArrayPtr<unsigned char> buffer(new unsigned char[fileSize]);

	const size_t bytesRead = srcFile.Read(buffer.Get(), fileSize);
	const size_t bytesWritten = destFile.Write(buffer.Get(), fileSize);
	return bytesRead == fileSize && bytesWritten == fileSize;
}
//-----------------------------------------------------------------------------
bool FileSystem::RenameFile(const String& srcFileName, const String& destFileName)
{
#if SE_PLATFORM_WINDOWS
	return MoveFileW(FileSystem::WideNativePath(srcFileName).CString(), FileSystem::WideNativePath(destFileName).CString()) != 0;
#else
	return rename(FileSystem::NativePath(srcFileName).CString(), FileSystem::NativePath(destFileName).CString()) == 0;
#endif
}
//-----------------------------------------------------------------------------
bool FileSystem::DeleteFile(const String& fileName)
{
#if SE_PLATFORM_WINDOWS
	return ::DeleteFileW(FileSystem::WideNativePath(fileName).CString()) != 0;
#else
	return remove(FileSystem::NativePath(fileName).CString()) == 0;
#endif
}
//-----------------------------------------------------------------------------
String FileSystem::CurrentDir()
{
#if SE_PLATFORM_WINDOWS
	wchar_t path[MAX_PATH];
	path[0] = 0;
	GetCurrentDirectoryW(MAX_PATH, path);
	return FileSystem::AddTrailingSlash(String(path));
#else
	char path[MAX_PATH];
	path[0] = 0;
	getcwd(path, MAX_PATH);
	return FileSystem::AddTrailingSlash(String(path));
#endif
}
//-----------------------------------------------------------------------------
unsigned FileSystem::LastModifiedTime(const String& fileName)
{
	if (fileName.IsEmpty())
		return 0;

#if SE_PLATFORM_WINDOWS
	struct _stat st;
	if (!_stat(fileName.CString(), &st))
		return (unsigned)st.st_mtime;
	else
		return 0;
#else
	struct stat st;
	if (!stat(fileName.CString(), &st))
		return (unsigned)st.st_mtime;
	else
		return 0;
#endif
}
//-----------------------------------------------------------------------------
bool FileSystem::SetLastModifiedTime(const String& fileName, unsigned newTime)
{
	if (fileName.IsEmpty())
		return false;

#if SE_PLATFORM_WINDOWS
	struct _stat oldTime;
	struct _utimbuf newTimes;
	if (_stat(fileName.CString(), &oldTime) != 0)
		return false;
	newTimes.actime = oldTime.st_atime;
	newTimes.modtime = newTime;
	return _utime(fileName.CString(), &newTimes) == 0;
#else
	struct stat oldTime;
	struct utimbuf newTimes;
	if (stat(fileName.CString(), &oldTime) != 0)
		return false;
	newTimes.actime = oldTime.st_atime;
	newTimes.modtime = newTime;
	return utime(fileName.CString(), &newTimes) == 0;
#endif
}
//-----------------------------------------------------------------------------
bool FileSystem::FileExists(const String& fileName)
{
	const String fixedName = FileSystem::NativePath(FileSystem::RemoveTrailingSlash(fileName));

#if SE_PLATFORM_WINDOWS
	const DWORD attributes = GetFileAttributesW(WString(fixedName).CString());
	if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
		return false;
#else
	struct stat st;
	if (stat(fixedName.CString(), &st) || st.st_mode & S_IFDIR)
		return false;
#endif

	return true;
}
//-----------------------------------------------------------------------------
bool FileSystem::DirExists(const String& pathName)
{
#if !SE_PLATFORM_WINDOWS
	// Always return true for the root directory
	if (pathName == "/")
		return true;
#endif

	const String fixedName = FileSystem::NativePath(FileSystem::RemoveTrailingSlash(pathName));

#if SE_PLATFORM_WINDOWS
	const DWORD attributes = GetFileAttributesW(WString(fixedName).CString());
	if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
		return false;
#else
	struct stat st;
	if (stat(fixedName.CString(), &st) || !(st.st_mode & S_IFDIR))
		return false;
#endif

	return true;
}
//-----------------------------------------------------------------------------
inline void ScanDirInternal(Vector<String>& result, String path, const String& startPath, const String& filter, unsigned flags, bool recursive)
{
	path = FileSystem::AddTrailingSlash(path);
	String deltaPath;
	if (path.Length() > startPath.Length())
		deltaPath = path.Substring(startPath.Length());

	String filterExtension = filter.Substring(filter.Find('.'));
	if (filterExtension.Contains('*'))
		filterExtension.Clear();

#if SE_PLATFORM_WINDOWS
	WIN32_FIND_DATAW info;
	HANDLE handle = FindFirstFileW(WString(path + "*").CString(), &info);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			String fileName(info.cFileName);
			if (!fileName.IsEmpty())
			{
				if (info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN && !(flags & FileSystem::SCAN_HIDDEN))
					continue;
				if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (flags & FileSystem::SCAN_DIRS)
						result.Push(deltaPath + fileName);
					if (recursive && fileName != "." && fileName != "..")
						ScanDirInternal(result, path + fileName, startPath, filter, flags, recursive);
				}
				else if (flags & FileSystem::SCAN_FILES)
				{
					if (filterExtension.IsEmpty() || fileName.EndsWith(filterExtension))
						result.Push(deltaPath + fileName);
				}
			}
		} while (FindNextFileW(handle, &info));

		FindClose(handle);
	}
#else
	DIR* dir;
	struct dirent* de;
	struct stat st;
	dir = opendir(NativePath(path).CString());
	if (dir)
	{
		while ((de = readdir(dir)))
		{
			// \todo Filename may be unnormalized Unicode on Mac OS X. Re-normalize as necessary
			String fileName(de->d_name);
			bool normalEntry = fileName != "." && fileName != "..";
			if (normalEntry && !(flags & SCAN_HIDDEN) && fileName.StartsWith("."))
				continue;
			String pathAndName = path + fileName;
			if (!stat(pathAndName.CString(), &st))
			{
				if (st.st_mode & FileSystem::S_IFDIR)
				{
					if (flags & FileSystem::SCAN_DIRS)
						result.Push(deltaPath + fileName);
					if (recursive && normalEntry)
						ScanDirInternal(result, path + fileName, startPath, filter, flags, recursive);
				}
				else if (flags & FileSystem::SCAN_FILES)
				{
					if (filterExtension.IsEmpty() || fileName.EndsWith(filterExtension))
						result.Push(deltaPath + fileName);
				}
			}
		}
		closedir(dir);
	}
#endif
}
//-----------------------------------------------------------------------------
void FileSystem::ScanDir(Vector<String>& result, const String& pathName, const String& filter, unsigned flags, bool recursive)
{
	const String initialPath = FileSystem::AddTrailingSlash(pathName);
	ScanDirInternal(result, initialPath, initialPath, filter, flags, recursive);
}
//-----------------------------------------------------------------------------
String FileSystem::ExecutableDir()
{
	String ret;

#if SE_PLATFORM_WINDOWS
	wchar_t exeName[MAX_PATH];
	exeName[0] = 0;
	GetModuleFileNameW(0, exeName, MAX_PATH);
	ret = FileSystem::Path(String(exeName));
#elif defined(__APPLE__)
	char exeName[MAX_PATH];
	memset(exeName, 0, MAX_PATH);
	unsigned size = MAX_PATH;
	_NSGetExecutablePath(exeName, &size);
	ret = FileSystem::Path(String(exeName));
#elif SE_PLATFORM_LINUX
	char exeName[MAX_PATH];
	memset(exeName, 0, MAX_PATH);
	pid_t pid = getpid();
	String link = "/proc/" + String(pid) + "/exe";
	readlink(link.CString(), exeName, MAX_PATH);
	ret = FileSystem::Path(String(exeName));
#endif

	// Sanitate /./ construct away
	ret.Replace("/./", "/");
	return ret;
}
//-----------------------------------------------------------------------------
void FileSystem::SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension)
{
	String fullPathCopy = FileSystem::NormalizePath(fullPath);

	const size_t extPos = fullPathCopy.FindLast('.');
	size_t pathPos = fullPathCopy.FindLast('/');

	if (extPos != String::NPOS && (pathPos == String::NPOS || extPos > pathPos))
	{
		extension = fullPathCopy.Substring(extPos);
		if (lowercaseExtension)
			extension = extension.ToLower();
		fullPathCopy = fullPathCopy.Substring(0, extPos);
	}
	else
		extension.Clear();

	pathPos = fullPathCopy.FindLast('/');
	if (pathPos != String::NPOS)
	{
		fileName = fullPathCopy.Substring(pathPos + 1);
		pathName = fullPathCopy.Substring(0, pathPos + 1);
	}
	else
	{
		fileName = fullPathCopy;
		pathName.Clear();
	}
}
//-----------------------------------------------------------------------------
String FileSystem::Path(const String& fullPath)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path;
}
//-----------------------------------------------------------------------------
String FileSystem::FileName(const String& fullPath)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return file;
}
//-----------------------------------------------------------------------------
String FileSystem::Extension(const String& fullPath, bool lowercaseExtension)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension, lowercaseExtension);
	return extension;
}
//-----------------------------------------------------------------------------
String FileSystem::FileNameAndExtension(const String& fileName, bool lowercaseExtension)
{
	String path, file, extension;
	SplitPath(fileName, path, file, extension, lowercaseExtension);
	return file + extension;
}
//-----------------------------------------------------------------------------
String FileSystem::ReplaceExtension(const String& fullPath, const String& newExtension)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path + file + newExtension;
}
//-----------------------------------------------------------------------------
String FileSystem::AddTrailingSlash(const String& pathName)
{
	String ret = pathName.Trimmed();
	ret.Replace('\\', '/');
	if (!ret.IsEmpty() && ret.Back() != '/')
		ret += '/';
	return ret;
}
//-----------------------------------------------------------------------------
String FileSystem::RemoveTrailingSlash(const String& pathName)
{
	String ret = pathName.Trimmed();
	ret.Replace('\\', '/');
	if (!ret.IsEmpty() && ret.Back() == '/')
		ret.Resize(ret.Length() - 1);
	return ret;
}
//-----------------------------------------------------------------------------
String FileSystem::ParentPath(const String& path)
{
	const size_t pos = RemoveTrailingSlash(path).FindLast('/');
	if (pos != String::NPOS)
		return path.Substring(0, pos + 1);
	else
		return String();
}
//-----------------------------------------------------------------------------
String FileSystem::NormalizePath(const String& pathName)
{
	return pathName.Replaced('\\', '/');
}
//-----------------------------------------------------------------------------
String FileSystem::NativePath(const String& pathName)
{
#if SE_PLATFORM_WINDOWS
	return pathName.Replaced('/', '\\');
#else
	return pathName;
#endif
}
//-----------------------------------------------------------------------------
WString FileSystem::WideNativePath(const String& pathName)
{
#if SE_PLATFORM_WINDOWS
	return WString(pathName.Replaced('/', '\\'));
#else
	return WString(pathName);
#endif
}
//-----------------------------------------------------------------------------
bool FileSystem::IsAbsolutePath(const String& pathName)
{
	if (pathName.IsEmpty())
		return false;

	const String path = NormalizePath(pathName);

	if (path[0] == '/')
		return true;

#if SE_PLATFORM_WINDOWS
	if (path.Length() > 1 && IsAlpha(path[0]) && path[1] == ':')
		return true;
#endif

	return false;
}
//-----------------------------------------------------------------------------