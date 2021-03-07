#include "stdafx.h"
#include "File.h"
#include "FileSystem.h"
#include "Engine/DebugNew.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
constexpr inline const wchar_t* openModesTranslate(FileMode mode)
{
	switch(mode)
	{
	case FileMode::Read: return L"rb";
	case FileMode::Write: return L"wb";
	case FileMode::ReadWrite: return L"r+b";
	default: break;
	}
	return L"w+b";
}
#else
constexpr inline const char* openModesTranslate(FileMode mode)
{
	switch (mode)
	{
	case FileMode::Read: return "rb";
	case FileMode::Write: return "wb";
	case FileMode::ReadWrite: return "r+b";
	default: break;
	}
	return "w+b";
}
#endif
//-----------------------------------------------------------------------------
File::File(const String& fileName, FileMode mode)
{
	Open(fileName, mode);
}
//-----------------------------------------------------------------------------
File::~File()
{
	Close();
}
//-----------------------------------------------------------------------------
bool File::Open(const String& fileName, FileMode fileMode)
{
	Close();

	if (fileName.IsEmpty())
		return false;

#if SE_PLATFORM_WINDOWS
	m_handle = _wfopen(WideNativePath(fileName).CString(), openModesTranslate(fileMode));
#else
	m_handle = fopen(NativePath(fileName).CString(), openModesTranslate(fileMode));
#endif

	// If file did not exist in readwrite mode, retry with write-update mode
	if (m_mode == FileMode::ReadWrite && !m_handle)
	{
#if SE_PLATFORM_WINDOWS
		m_handle = _wfopen(WideNativePath(fileName).CString(), L"w+b");
#else
		m_handle = fopen(NativePath(fileName).CString(), "w+b");
#endif
	}

	if (!m_handle)
		return false;

	m_name = fileName;
	m_mode = fileMode;
	m_position = 0;
	m_readSyncNeeded = false;
	m_writeSyncNeeded = false;

	fseek((FILE*)m_handle, 0, SEEK_END);
	m_size = static_cast<size_t>(ftell((FILE*)m_handle));
	fseek((FILE*)m_handle, 0, SEEK_SET);
	return true;
}
//-----------------------------------------------------------------------------
size_t File::Read(void* dest, size_t numBytes)
{
	if (!m_handle || m_mode == FileMode::Write)
		return 0;

	if (numBytes + m_position > m_size)
		numBytes = m_size - m_position;
	if (!numBytes)
		return 0;

	// Need to reassign the position due to internal buffering when transitioning from writing to reading
	if (m_readSyncNeeded)
	{
		fseek((FILE*)m_handle, (long)m_position, SEEK_SET);
		m_readSyncNeeded = false;
	}

	size_t ret = fread(dest, numBytes, 1, (FILE*)m_handle);
	if (ret != 1)
	{
		// If error, return to the position where the read began
		fseek((FILE*)m_handle, (long)m_position, SEEK_SET);
		return 0;
	}

	m_writeSyncNeeded = true;
	m_position += numBytes;
	return numBytes;
}
//-----------------------------------------------------------------------------
size_t File::Seek(size_t newPosition)
{
	if (!m_handle)
		return 0;

	// Allow sparse seeks if writing
	if (m_mode == FileMode::Read && newPosition > m_size)
		newPosition = m_size;

	fseek((FILE*)m_handle, (long)newPosition, SEEK_SET);
	m_position = newPosition;
	m_readSyncNeeded = false;
	m_writeSyncNeeded = false;
	return m_position;
}
//-----------------------------------------------------------------------------
size_t File::Write(const void* data, size_t numBytes)
{
	if (!m_handle || m_mode == FileMode::Read)
		return 0;

	if (!numBytes)
		return 0;

	// Need to reassign the position due to internal buffering when transitioning from reading to writing
	if (m_writeSyncNeeded)
	{
		fseek((FILE*)m_handle, (long)m_position, SEEK_SET);
		m_writeSyncNeeded = false;
	}

	if (fwrite(data, numBytes, 1, (FILE*)m_handle) != 1)
	{
		// If error, return to the position where the write began
		fseek((FILE*)m_handle, (long)m_position, SEEK_SET);
		return 0;
	}

	m_readSyncNeeded = true;
	m_position += numBytes;
	if (m_position > m_size)
		m_size = m_position;

	return m_size;
}
//-----------------------------------------------------------------------------
bool File::IsReadable() const
{
	return m_handle != 0 && m_mode != FileMode::Write;
}
//-----------------------------------------------------------------------------
bool File::IsWritable() const
{
	return m_handle != 0 && m_mode != FileMode::Read;
}
//-----------------------------------------------------------------------------
void File::Close()
{
	if (m_handle)
	{
		fclose((FILE*)m_handle);
		m_handle = 0;
		m_position = 0;
		m_size = 0;
	}
}
//-----------------------------------------------------------------------------
void File::Flush()
{
	if (m_handle)
		fflush((FILE*)m_handle);
}
//-----------------------------------------------------------------------------
bool File::IsOpen() const
{
	return m_handle != 0;
}
//-----------------------------------------------------------------------------