#pragma once

#include "Stream.h"

<<<<<<< HEAD
// File open mode.
enum FileMode
=======
enum class FileMode
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
{
	Read = 0,
	Write,
	ReadWrite
};

<<<<<<< HEAD
class PackageFile;

// Filesystem file.
class File : public Stream
{
public:
	// Construct.
	File();
	// Construct and open a file.
	File(const String& fileName, FileMode fileMode = FILE_READ);
	// Destruct. Close the file if open.
=======
class File : public Stream
{
public:
	File() = default;
	File(const String& fileName, FileMode fileMode = FileMode::Read);
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	~File();

	// Read bytes from the file. Return number of bytes actually read.
	size_t Read(void* dest, size_t numBytes) override;
	// Set position in bytes from the beginning of the file.
	size_t Seek(size_t newPosition) override;
	// Write bytes to the file. Return number of bytes actually written.
	size_t Write(const void* data, size_t numBytes) override;
	// Return whether read operations are allowed.
	bool IsReadable() const override;
	// Return whether write operations are allowed.
	bool IsWritable() const override;

	// Open a file. Return true on success.
<<<<<<< HEAD
	bool Open(const String& fileName, FileMode fileMode = FILE_READ);
	// Close the file.
=======
	bool Open(const String& fileName, FileMode fileMode = FileMode::Read);
	// Close the fileFileMode::Read
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
	void Close();
	// Flush any buffered output to the file.
	void Flush();

	// Return the open mode.
<<<<<<< HEAD
	FileMode Mode() const { return mode; }
	// Return whether is open.
	bool IsOpen() const;
	// Return the file handle.
	void* Handle() const { return handle; }
=======
	FileMode Mode() const { return m_mode; }
	// Return whether is open.
	bool IsOpen() const;
	// Return the file handle.
	void* Handle() const { return m_handle; }
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5

	using Stream::Read;
	using Stream::Write;

private:
	// Open mode.
<<<<<<< HEAD
	FileMode mode;
	// File handle.
	void* handle;
	// Synchronization needed before read -flag.
	bool readSyncNeeded;
	// Synchronization needed before write -flag.
	bool writeSyncNeeded;
=======
	FileMode m_mode = FileMode::Read;
	// File handle.
	void* m_handle = nullptr;
	// Synchronization needed before read -flag.
	bool m_readSyncNeeded = false;
	// Synchronization needed before write -flag.
	bool m_writeSyncNeeded = false;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
};