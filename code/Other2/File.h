#pragma once

#include "Stream.h"

enum class FileMode
{
	Read = 0,
	Write,
	ReadWrite
};

class File : public Stream
{
public:
	File() = default;
	File(const String& fileName, FileMode fileMode = FileMode::Read);
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
	bool Open(const String& fileName, FileMode fileMode = FileMode::Read);
	// Close the fileFileMode::Read
	void Close();
	// Flush any buffered output to the file.
	void Flush();

	// Return the open mode.
	FileMode Mode() const { return m_mode; }
	// Return whether is open.
	bool IsOpen() const;
	// Return the file handle.
	void* Handle() const { return m_handle; }

	using Stream::Read;
	using Stream::Write;

private:
	// Open mode.
	FileMode m_mode = FileMode::Read;
	// File handle.
	void* m_handle = nullptr;
	// Synchronization needed before read -flag.
	bool m_readSyncNeeded = false;
	// Synchronization needed before write -flag.
	bool m_writeSyncNeeded = false;
};