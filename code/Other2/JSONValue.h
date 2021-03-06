#pragma once

#include "HashMap.h"
#include "String.h"
#include "Vector.h"

class JSONValue;
class Stream;

typedef Vector<JSONValue> JSONArray;
typedef HashMap<String, JSONValue> JSONObject;

// JSON value types.
enum JSONType
{
	JSON_NULL = 0,
	JSON_BOOL,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
	MAX_JSON_TYPES
};

// JSON data union.
struct JSONData
{
	union
	{
		char charValue;
		bool boolValue;
		double numberValue;
		size_t padding[2];
	};
};

// JSON value. Stores a boolean, string or number, or either an array or dictionary-like collection of nested values.
class JSONValue
{
	friend class JSONFile;
public:
	// Empty (null) value.
	static const JSONValue EMPTY;
	// Empty array.
	static const JSONArray emptyJSONArray;
	// Empty object.
	static const JSONObject emptyJSONObject;

	JSONValue() = default;
	JSONValue(const JSONValue& value);
	JSONValue(bool value);
	JSONValue(int value);
	JSONValue(unsigned value);
	JSONValue(float value);
	JSONValue(double value);
	JSONValue(const String& value);
	JSONValue(const char* value);
	JSONValue(const JSONArray& value);
	JSONValue(const JSONObject& value);
	~JSONValue();

	JSONValue& operator=(const JSONValue& rhs);
	JSONValue& operator=(bool rhs);
	JSONValue& operator=(int rhs);
	JSONValue& operator=(unsigned rhs);
	JSONValue& operator=(float rhs);
	JSONValue& operator=(double rhs);
	JSONValue& operator=(const String& value);
	JSONValue& operator=(const char* value);
	JSONValue& operator=(const JSONArray& value);
	JSONValue& operator=(const JSONObject& value);

	// Index as an array. Becomes an array if was not before.
	JSONValue& operator[](size_t index);
	// Const index as an array. Return a null value if not an array.
	const JSONValue& operator[](size_t index) const;
	// Index as an object. Becomes an object if was not before.
	JSONValue& operator[](const String& key);
	// Const index as an object. Return a null value if not an object.
	const JSONValue& operator[](const String& key) const;
	// Test for equality with another JSON value.
	bool operator==(const JSONValue& rhs) const;
	// Test for inequality.
	bool operator!=(const JSONValue& rhs) const { return !(*this == rhs); }

	// Parse from a string. Return true on success.
	bool FromString(const String& str);
	// Parse from a C string. Return true on success.
	bool FromString(const char* str);
	// Parse from a binary stream.
	void FromBinary(Stream& source);
	// Write to a string. Called recursively to write nested values.
	void ToString(String& dest, int spacing = 2, int indent = 0) const;
	// Return as string.
	String ToString(int spacing = 2) const;
	// Serialize to a binary stream.
	void ToBinary(Stream& dest) const;

	// Push a value at the end. Becomes an array if was not before.
	void Push(const JSONValue& value);
	// Insert a value at position. Becomes an array if was not before.
	void Insert(size_t index, const JSONValue& value);
	// Remove the last value. No-op if not an array.
	void Pop();
	// Remove indexed value(s). No-op if not an array.
	void Erase(size_t pos, size_t length = 1);
	// Resize array. Becomes an array if was not before.
	void Resize(size_t newSize);
	// Insert an associative value. Becomes an object if was not before.
	void Insert(const Pair<String, JSONValue>& pair);
	// Remove an associative value. No-op if not an object.
	void Erase(const String& key);
	// Clear array or object. No-op otherwise.
	void Clear();
	// Set to an empty array.
	void SetEmptyArray();
	// Set to an empty object.
	void SetEmptyObject();
	// Set to null value.
	void SetNull();

	// Return number of values for objects or arrays, or 0 otherwise.
	size_t Size() const;
	// Return whether an object or array is empty. Return false if not an object or array.
	bool IsEmpty() const;

	// Return type.
	JSONType Type() const { return m_type; }
	// Return whether is null.
	bool IsNull() const { return m_type == JSON_NULL; }
	// Return whether is a bool.
	bool IsBool() const { return m_type == JSON_BOOL; }
	// Return whether is a number.
	bool IsNumber() const { return m_type == JSON_NUMBER; }
	// Return whether is a string.
	bool IsString() const { return m_type == JSON_STRING; }
	// Return whether is an array.
	bool IsArray() const { return m_type == JSON_ARRAY; }
	// Return whether is an object.
	bool IsObject() const { return m_type == JSON_OBJECT; }
	// Return value as a bool, or false on type mismatch.
	bool GetBool() const { return m_type == JSON_BOOL ? m_data.boolValue : false; }
	// Return value as a number, or zero on type mismatch.
	double GetNumber() const { return m_type == JSON_NUMBER ? m_data.numberValue : 0.0; }
	// Return value as a string, or empty string on type mismatch.
	const String& GetString() const { return m_type == JSON_STRING ? *(reinterpret_cast<const String*>(&m_data)) : String::EMPTY; }
	// Return value as an array, or empty on type mismatch.
	const JSONArray& GetArray() const { return m_type == JSON_ARRAY ? *(reinterpret_cast<const JSONArray*>(&m_data)) : emptyJSONArray; }
	// Return value as an object, or empty on type mismatch.
	const JSONObject& GetObject() const { return m_type == JSON_OBJECT ? *(reinterpret_cast<const JSONObject*>(&m_data)) : emptyJSONObject; }
	// Return whether has an associative value.
	bool Contains(const String& key) const;

private:
	// Parse from a char buffer. Return true on success.
	bool parse(const char*& pos, const char*& end);
	// Assign a new type and perform the necessary dynamic allocation / deletion.
	void setType(JSONType newType);

	// Append a string in JSON format into the destination.
	static void writeJSONString(String& dest, const String& str);
	// Append indent spaces to the destination.
	static void writeIndent(String& dest, int indent);
	// Read a string in JSON format from a stream. Return true on success.
	static bool readJSONString(String& dest, const char*& pos, const char*& end, bool inQuote);
	// Match until the end of a string. Return true if successfully matched.
	static bool matchString(const char* str, const char*& pos, const char*& end);
	// Scan until a character is found. Return true if successfully matched.
	static bool matchChar(char c, const char*& pos, const char*& end);

	// Get the next char from a stream. Return true on success or false if the stream ended.
	static bool nextChar(char& dest, const char*& pos, const char*& end, bool skipWhiteSpace)
	{
		while (pos < end)
		{
			dest = *pos;
			++pos;
			if (!skipWhiteSpace || dest > 0x20)
				return true;
		}

		return false;
	}

	JSONType m_type = JSON_NULL;
	JSONData m_data;
};