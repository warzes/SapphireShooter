#include "stdafx.h"
#include "Profiler.h"
#include "File.h"
#include "FileSystem.h"
#include "Image.h"
#include "JSONFile.h"
#include "ResourceCache.h"
#include "Engine/DebugNew.h"

ResourceCache::ResourceCache()
{
	RegisterSubsystem(this);
}

ResourceCache::~ResourceCache()
{
	UnloadAllResources(true);
	RemoveSubsystem(this);
}

bool ResourceCache::AddResourceDir(const String& pathName, bool addFirst)
{
	PROFILE(AddResourceDir);

	if (!FileSystem::DirExists(pathName))
	{
		SE_LOG_ERROR(("Could not open directory " + pathName).CString());
		return false;
	}

	String fixedPath = SanitateResourceDirName(pathName);

	// Check that the same path does not already exist
	for (size_t i = 0; i < resourceDirs.Size(); ++i)
	{
		if (!resourceDirs[i].Compare(fixedPath, false))
			return true;
	}

	if (addFirst)
		resourceDirs.Insert(0, fixedPath);
	else
		resourceDirs.Push(fixedPath);

	SE_LOG(("Added resource path " + fixedPath).CString());
	return true;
}

bool ResourceCache::AddManualResource(Resource* resource)
{
	if (!resource)
	{
		SE_LOG_ERROR("Null manual resource");
		return false;
	}

	if (resource->Name().IsEmpty())
	{
		SE_LOG_ERROR("Manual resource with empty name, can not add");
		return false;
	}

	resources[MakePair(resource->Type(), StringHash(resource->Name()))] = resource;
	return true;
}

void ResourceCache::RemoveResourceDir(const String& pathName)
{
	// Convert path to absolute
	String fixedPath = SanitateResourceDirName(pathName);

	for (size_t i = 0; i < resourceDirs.Size(); ++i)
	{
		if (!resourceDirs[i].Compare(fixedPath, false))
		{
			resourceDirs.Erase(i);
			SE_LOG_ERROR(("Removed resource path " + fixedPath).CString());
			return;
		}
	}
}

void ResourceCache::UnloadResource(StringHash type, const String& name, bool force)
{
	auto key = MakePair(type, StringHash(name));
	auto it = resources.Find(key);
	if (it == resources.End())
		return;

	Resource* resource = it->second;
	if (resource->Refs() == 1 || force)
		resources.Erase(key);
}

void ResourceCache::UnloadResources(StringHash type, bool force)
{
	// In case resources refer to other resources, repeat until there are no further unloads
	for (;;)
	{
		size_t unloaded = 0;

		for (auto it = resources.Begin(); it != resources.End();)
		{
			auto current = it++;
			if (current->first.first == type)
			{
				Resource* resource = current->second;
				if (resource->Refs() == 1 || force)
				{
					resources.Erase(current);
					++unloaded;
				}
			}
		}

		if (!unloaded)
			break;
	}
}

void ResourceCache::UnloadResources(StringHash type, const String& partialName, bool force)
{
	// In case resources refer to other resources, repeat until there are no further unloads
	for (;;)
	{
		size_t unloaded = 0;

		for (auto it = resources.Begin(); it != resources.End();)
		{
			auto current = it++;
			if (current->first.first == type)
			{
				Resource* resource = current->second;
				if (resource->Name().StartsWith(partialName) && (resource->Refs() == 1 || force))
				{
					resources.Erase(current);
					++unloaded;
				}
			}
		}

		if (!unloaded)
			break;
	}
}

void ResourceCache::UnloadResources(const String& partialName, bool force)
{
	// In case resources refer to other resources, repeat until there are no further unloads
	for (;;)
	{
		size_t unloaded = 0;

		for (auto it = resources.Begin(); it != resources.End();)
		{
			auto current = it++;
			Resource* resource = current->second;
			if (resource->Name().StartsWith(partialName) && (!resource->Refs() == 1 || force))
			{
				resources.Erase(current);
				++unloaded;
			}
		}

		if (!unloaded)
			break;
	}
}

void ResourceCache::UnloadAllResources(bool force)
{
	// In case resources refer to other resources, repeat until there are no further unloads
	for (;;)
	{
		size_t unloaded = 0;

		for (auto it = resources.Begin(); it != resources.End();)
		{
			auto current = it++;
			Resource* resource = current->second;
			if (resource->Refs() == 1 || force)
			{
				resources.Erase(current);
				++unloaded;
			}
		}

		if (!unloaded)
			break;
	}
}

bool ResourceCache::ReloadResource(Resource* resource)
{
	if (!resource)
		return false;

	AutoPtr<Stream> stream = OpenResource(resource->Name());
	return stream ? resource->Load(*stream) : false;
}

AutoPtr<Stream> ResourceCache::OpenResource(const String& nameIn)
{
	String name = SanitateResourceName(nameIn);
	AutoPtr<Stream> ret;

	for (size_t i = 0; i < resourceDirs.Size(); ++i)
	{
		if (FileSystem::FileExists(resourceDirs[i] + name))
		{
			// Construct the file first with full path, then rename it to not contain the resource path,
			// so that the file's name can be used in further OpenResource() calls (for example over the network)
			ret = new File(resourceDirs[i] + name);
			break;
		}
	}

	// Fallback using absolute path
	if (!ret)
		ret = new File(name);

	if (!ret->IsReadable())
	{
		SE_LOG_ERROR(("Could not open resource file " + name).CString());
		ret.Reset();
	}

	return ret;
}

Resource* ResourceCache::LoadResource(StringHash type, const String& nameIn)
{
	String name = SanitateResourceName(nameIn);

	// If empty name, return null pointer immediately without logging an error
	if (name.IsEmpty())
		return nullptr;

	// Check for existing resource
	auto key = MakePair(type, StringHash(name));
	auto it = resources.Find(key);
	if (it != resources.End())
		return it->second;

	SharedPtr<Object> newObject = Create(type);
	if (!newObject)
	{
		SE_LOG_ERROR(("Could not load unknown resource type " + String(type)).CString());
		return nullptr;
	}
	Resource* newResource = dynamic_cast<Resource*>(newObject.Get());
	if (!newResource)
	{
		SE_LOG_ERROR(("Type " + String(type) + " is not a resource").CString());
		return nullptr;
	}

	// Attempt to load the resource
	AutoPtr<Stream> stream = OpenResource(name);
	if (!stream)
		return nullptr;

	if (name == "DiffuseNormal.vert")
		SE_LOG(("Loading resource " + name).CString());

	SE_LOG(("Loading resource " + name).CString());
	newResource->SetName(name);
	if (!newResource->Load(*stream))
		return nullptr;

	// Store to cache
	resources[key] = newResource;
	return newResource;
}

void ResourceCache::ResourcesByType(Vector<Resource*>& result, StringHash type) const
{
	result.Clear();

	for (auto it = resources.Begin(); it != resources.End(); ++it)
	{
		if (it->second->Type() == type)
			result.Push(it->second);
	}
}

bool ResourceCache::Exists(const String& nameIn) const
{
	String name = SanitateResourceName(nameIn);

	for (size_t i = 0; i < resourceDirs.Size(); ++i)
	{
		if (FileSystem::FileExists(resourceDirs[i] + name))
			return true;
	}

	// Fallback using absolute path
	return FileSystem::FileExists(name);
}

String ResourceCache::ResourceFileName(const String& name) const
{
	for (unsigned i = 0; i < resourceDirs.Size(); ++i)
	{
		if (FileSystem::FileExists(resourceDirs[i] + name))
			return resourceDirs[i] + name;
	}

	return String();
}

String ResourceCache::SanitateResourceName(const String& nameIn) const
{
	// Sanitate unsupported constructs from the resource name
	String name = FileSystem::NormalizePath(nameIn);
	name.Replace("../", "");
	name.Replace("./", "");

	// If the path refers to one of the resource directories, normalize the resource name
	if (resourceDirs.Size())
	{
		String namePath = FileSystem::Path(name);
		String exePath = FileSystem::ExecutableDir();
		for (unsigned i = 0; i < resourceDirs.Size(); ++i)
		{
			String relativeResourcePath = resourceDirs[i];
			if (relativeResourcePath.StartsWith(exePath))
				relativeResourcePath = relativeResourcePath.Substring(exePath.Length());

			if (namePath.StartsWith(resourceDirs[i], false))
				namePath = namePath.Substring(resourceDirs[i].Length());
			else if (namePath.StartsWith(relativeResourcePath, false))
				namePath = namePath.Substring(relativeResourcePath.Length());
		}

		name = namePath + FileSystem::FileNameAndExtension(name);
	}

	return name.Trimmed();
}

String ResourceCache::SanitateResourceDirName(const String& nameIn) const
{
	// Convert path to absolute
	String fixedPath = FileSystem::AddTrailingSlash(nameIn);
	if (!FileSystem::IsAbsolutePath(fixedPath))
		fixedPath = FileSystem::CurrentDir() + fixedPath;

	// Sanitate away /./ construct
	fixedPath.Replace("/./", "/");

	return fixedPath.Trimmed();
}

void RegisterResourceLibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	Image::RegisterObject();
	JSONFile::RegisterObject();
}