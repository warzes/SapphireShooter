#include "stdafx.h"
#include "Vector.h"
#include "HashMap.h"
#include "JSONValue.h"
#include "Stream.h"
//-----------------------------------------------------------------------------
const JSONValue JSONValue::EMPTY;
const JSONArray JSONValue::emptyJSONArray;
const JSONObject JSONValue::emptyJSONObject;
//-----------------------------------------------------------------------------
JSONValue::JSONValue(const JSONValue& value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(bool value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(int value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(unsigned value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(float value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(double value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(const String& value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(const char* value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(const JSONArray& value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::JSONValue(const JSONObject& value)
{
	*this = value;
}
//-----------------------------------------------------------------------------
JSONValue::~JSONValue()
{
	setType(JSON_NULL);
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(const JSONValue& rhs)
{
	setType(rhs.m_type);

	switch (m_type)
	{
	case JSON_BOOL:
		m_data.boolValue = rhs.m_data.boolValue;
		break;

	case JSON_NUMBER:
		m_data.numberValue = rhs.m_data.numberValue;
		break;

	case JSON_STRING:
		*(reinterpret_cast<String*>(&m_data)) = *(reinterpret_cast<const String*>(&rhs.m_data));
		break;

	case JSON_ARRAY:
		*(reinterpret_cast<JSONArray*>(&m_data)) = *(reinterpret_cast<const JSONArray*>(&rhs.m_data));
		break;

	case JSON_OBJECT:
		*(reinterpret_cast<JSONObject*>(&m_data)) = *(reinterpret_cast<const JSONObject*>(&rhs.m_data));
		break;

	case JSON_NULL:
	case MAX_JSON_TYPES:
	default:
		break;
	}

	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(bool rhs)
{
	setType(JSON_BOOL);
	m_data.boolValue = rhs;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(int rhs)
{
	setType(JSON_NUMBER);
	m_data.numberValue = (double)rhs;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(unsigned rhs)
{
	setType(JSON_NUMBER);
	m_data.numberValue = (double)rhs;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(float rhs)
{
	setType(JSON_NUMBER);
	m_data.numberValue = (double)rhs;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(double rhs)
{
	setType(JSON_NUMBER);
	m_data.numberValue = rhs;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(const String& value)
{
	setType(JSON_STRING);
	*(reinterpret_cast<String*>(&m_data)) = value;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(const char* value)
{
	setType(JSON_STRING);
	*(reinterpret_cast<String*>(&m_data)) = value;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(const JSONArray& value)
{
	setType(JSON_ARRAY);
	*(reinterpret_cast<JSONArray*>(&m_data)) = value;
	return *this;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
JSONValue& JSONValue::operator=(const JSONObject& value)
{
	setType(JSON_OBJECT);
	*(reinterpret_cast<JSONObject*>(&m_data)) = value;
	return *this;
}
//-----------------------------------------------------------------------------
JSONValue& JSONValue::operator[](size_t index)
{
	if (m_type != JSON_ARRAY)
		setType(JSON_ARRAY);

	return (*(reinterpret_cast<JSONArray*>(&m_data)))[index];
}
//-----------------------------------------------------------------------------
const JSONValue& JSONValue::operator[](size_t index) const
{
	if (m_type == JSON_ARRAY)
		return (*(reinterpret_cast<const JSONArray*>(&m_data)))[index];
	else
		return EMPTY;
}
//-----------------------------------------------------------------------------
JSONValue& JSONValue::operator[](const String& key)
{
	if (m_type != JSON_OBJECT)
		setType(JSON_OBJECT);

	return (*(reinterpret_cast<JSONObject*>(&m_data)))[key];
}
//-----------------------------------------------------------------------------
const JSONValue& JSONValue::operator[](const String& key) const
{
	if (m_type == JSON_OBJECT)
	{
		const JSONObject& object = *(reinterpret_cast<const JSONObject*>(&m_data));
		auto it = object.Find(key);
		return it != object.End() ? it->second : EMPTY;
	}
	else
		return EMPTY;
}
<<<<<<< HEAD

=======
//-----------------------------------------------------------------------------
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
bool JSONValue::operator==(const JSONValue& rhs) const
{
	if (m_type != rhs.m_type)
		return false;

	switch (m_type)
	{
	case JSON_BOOL:
		return m_data.boolValue == rhs.m_data.boolValue;

	case JSON_NUMBER:
		return m_data.numberValue == rhs.m_data.numberValue;

	case JSON_STRING:
		return *(reinterpret_cast<const String*>(&m_data)) == *(reinterpret_cast<const String*>(&rhs.m_data));

	case JSON_ARRAY:
		return *(reinterpret_cast<const JSONArray*>(&m_data)) == *(reinterpret_cast<const JSONArray*>(&rhs.m_data));

	case JSON_OBJECT:
		return *(reinterpret_cast<const JSONObject*>(&m_data)) == *(reinterpret_cast<const JSONObject*>(&rhs.m_data));
	
	case JSON_NULL:
	case MAX_JSON_TYPES:
	default:
		return true;
	}
}
//-----------------------------------------------------------------------------
bool JSONValue::FromString(const String& str)
{
	const char* pos = str.CString();
	const char* end = pos + str.Length();
	return parse(pos, end);
}
//-----------------------------------------------------------------------------
bool JSONValue::FromString(const char* str)
{
	const char* pos = str;
	const char* end = pos + String::CStringLength(str);
	return parse(pos, end);
}
//-----------------------------------------------------------------------------
void JSONValue::FromBinary(Stream& source)
{
	JSONType newType = (JSONType)source.Read<unsigned char>();

	switch (newType)
	{
	case JSON_NULL:
		Clear();
		break;

	case JSON_BOOL:
		*this = source.Read<bool>();
		break;

	case JSON_NUMBER:
		*this = source.Read<double>();
		break;

	case JSON_STRING:
		*this = source.Read<String>();
		break;

	case JSON_ARRAY:
	{
		SetEmptyArray();
		size_t num = source.ReadVLE();
		for (size_t i = 0; i < num && !source.IsEof(); ++i)
			Push(source.Read<JSONValue>());
	}
	break;

	case JSON_OBJECT:
	{
		SetEmptyObject();
		size_t num = source.ReadVLE();
		for (size_t i = 0; i < num && !source.IsEof(); ++i)
		{
			String key = source.Read<String>();
			(*this)[key] = source.Read<JSONValue>();
		}
	}
	break;

	case MAX_JSON_TYPES:
	default:
		break;
	}
}
//-----------------------------------------------------------------------------
void JSONValue::ToString(String& dest, int spacing, int indent) const
{
	switch (m_type)
	{
	case JSON_BOOL:
		dest += m_data.boolValue;
		return;

	case JSON_NUMBER:
		dest += m_data.numberValue;
		return;

	case JSON_STRING:
		writeJSONString(dest, *(reinterpret_cast<const String*>(&m_data)));
		return;

	case JSON_ARRAY:
	{
		const JSONArray& array = GetArray();
		dest += '[';

		if (array.Size())
		{
			indent += spacing;
			for (auto it = array.Begin(); it < array.End(); ++it)
			{
				if (it != array.Begin())
					dest += ',';
				dest += '\n';
				writeIndent(dest, indent);
				it->ToString(dest, spacing, indent);
			}
			indent -= spacing;
			dest += '\n';
			writeIndent(dest, indent);
		}

		dest += ']';
	}
	break;

	case JSON_OBJECT:
	{
		const JSONObject& object = GetObject();
		dest += '{';

		if (object.Size())
		{
			indent += spacing;
			for (auto it = object.Begin(); it != object.End(); ++it)
			{
				if (it != object.Begin())
					dest += ',';
				dest += '\n';
				writeIndent(dest, indent);
				writeJSONString(dest, it->first);
				dest += ": ";
				it->second.ToString(dest, spacing, indent);
			}
			indent -= spacing;
			dest += '\n';
			writeIndent(dest, indent);
		}

		dest += '}';
	}
	break;

	default:
		dest += "null";
	}
}
//-----------------------------------------------------------------------------
String JSONValue::ToString(int spacing) const
{
	String ret;
	ToString(ret, spacing);
	return ret;
}
//-----------------------------------------------------------------------------
void JSONValue::ToBinary(Stream& dest) const
{
	dest.Write((unsigned char)m_type);

	switch (m_type)
	{
	case JSON_BOOL:
		dest.Write(m_data.boolValue);
		break;

	case JSON_NUMBER:
		dest.Write(m_data.numberValue);
		break;

	case JSON_STRING:
		dest.Write(GetString());
		break;

	case JSON_ARRAY:
	{
		const JSONArray& array = GetArray();
		dest.WriteVLE(array.Size());
		for (auto it = array.Begin(); it != array.End(); ++it)
			it->ToBinary(dest);
	}
	break;

	case JSON_OBJECT:
	{
		const JSONObject& object = GetObject();
		dest.WriteVLE(object.Size());
		for (auto it = object.Begin(); it != object.End(); ++it)
		{
			dest.Write(it->first);
			it->second.ToBinary(dest);
		}
	}
	break;

	default:
		break;
	}
}
//-----------------------------------------------------------------------------
void JSONValue::Push(const JSONValue& value)
{
	setType(JSON_ARRAY);
	(*(reinterpret_cast<JSONArray*>(&m_data))).Push(value);
}
//-----------------------------------------------------------------------------
void JSONValue::Insert(size_t index, const JSONValue& value)
{
	setType(JSON_ARRAY);
	(*(reinterpret_cast<JSONArray*>(&m_data))).Insert(index, value);
}
//-----------------------------------------------------------------------------
void JSONValue::Pop()
{
	if (m_type == JSON_ARRAY)
		(*(reinterpret_cast<JSONArray*>(&m_data))).Pop();
}
//-----------------------------------------------------------------------------
void JSONValue::Erase(size_t pos, size_t length)
{
	if (m_type == JSON_ARRAY)
		(*(reinterpret_cast<JSONArray*>(&m_data))).Erase(pos, length);
}
//-----------------------------------------------------------------------------
void JSONValue::Resize(size_t newSize)
{
	setType(JSON_ARRAY);
	(*(reinterpret_cast<JSONArray*>(&m_data))).Resize(newSize);
}
//-----------------------------------------------------------------------------
void JSONValue::Insert(const Pair<String, JSONValue>& pair)
{
	setType(JSON_OBJECT);
	(*(reinterpret_cast<JSONObject*>(&m_data))).Insert(pair);
}
//-----------------------------------------------------------------------------
void JSONValue::Erase(const String& key)
{
	if (m_type == JSON_OBJECT)
		(*(reinterpret_cast<JSONObject*>(&m_data))).Erase(key);
}
//-----------------------------------------------------------------------------
void JSONValue::Clear()
{
	if (m_type == JSON_ARRAY)
		(*(reinterpret_cast<JSONArray*>(&m_data))).Clear();
	else if (m_type == JSON_OBJECT)
		(*(reinterpret_cast<JSONObject*>(&m_data))).Clear();
}
//-----------------------------------------------------------------------------
void JSONValue::SetEmptyArray()
{
	setType(JSON_ARRAY);
	Clear();
}
//-----------------------------------------------------------------------------
void JSONValue::SetEmptyObject()
{
	setType(JSON_OBJECT);
	Clear();
}
//-----------------------------------------------------------------------------
void JSONValue::SetNull()
{
	setType(JSON_NULL);
}
//-----------------------------------------------------------------------------
size_t JSONValue::Size() const
{
	if (m_type == JSON_ARRAY)
		return (*(reinterpret_cast<const JSONArray*>(&m_data))).Size();
	else if (m_type == JSON_OBJECT)
		return (*(reinterpret_cast<const JSONObject*>(&m_data))).Size();
	else
		return 0;
}
//-----------------------------------------------------------------------------
bool JSONValue::IsEmpty() const
{
	if (m_type == JSON_ARRAY)
		return (*(reinterpret_cast<const JSONArray*>(&m_data))).IsEmpty();
	else if (m_type == JSON_OBJECT)
		return (*(reinterpret_cast<const JSONObject*>(&m_data))).IsEmpty();
	else
		return false;
}
//-----------------------------------------------------------------------------
bool JSONValue::Contains(const String& key) const
{
	if (m_type == JSON_OBJECT)
		return (*(reinterpret_cast<const JSONObject*>(&m_data))).Contains(key);
	else
		return false;
}
//-----------------------------------------------------------------------------
bool JSONValue::parse(const char*& pos, const char*& end)
{
	char c;

	// Handle comments
	for (;;)
	{
		if (!nextChar(c, pos, end, true))
			return false;

		if (c == '/')
		{
			if (!nextChar(c, pos, end, false))
				return false;
			if (c == '/')
			{
				// Skip until end of line
				if (!matchChar('\n', pos, end))
					return false;
			}
			else if (c == '*')
			{
				// Skip until end of comment
				if (!matchChar('*', pos, end))
					return false;
				if (!matchChar('/', pos, end))
					return false;
			}
			else
				return false;
		}
		else
			break;
	}

	if (c == '}' || c == ']')
		return false;
	else if (c == 'n')
	{
		SetNull();
		return matchString("ull", pos, end);
	}
	else if (c == 'f')
	{
		*this = false;
		return matchString("alse", pos, end);
	}
	else if (c == 't')
	{
		*this = true;
		return matchString("rue", pos, end);
	}
	else if (IsDigit(c) || c == '-')
	{
		--pos;
		*this = strtod(pos, const_cast<char**>(&pos));
		return true;
	}
	else if (c == '\"')
	{
		setType(JSON_STRING);
		return readJSONString(*(reinterpret_cast<String*>(&m_data)), pos, end, true);
	}
	else if (c == '[')
	{
		SetEmptyArray();
		// Check for empty first
		if (!nextChar(c, pos, end, true))
			return false;
		if (c == ']')
			return true;
		else
			--pos;

		for (;;)
		{
			JSONValue arrayValue;
			if (!arrayValue.parse(pos, end))
				return false;
			Push(arrayValue);
			if (!nextChar(c, pos, end, true))
				return false;
			if (c == ']')
				break;
			else if (c != ',')
				return false;
		}
		return true;
	}
	else if (c == '{')
	{
		SetEmptyObject();
		if (!nextChar(c, pos, end, true))
			return false;
		if (c == '}')
			return true;
		else
			--pos;

		for (;;)
		{
			String key;
			if (!readJSONString(key, pos, end, false))
				return false;
			if (!nextChar(c, pos, end, true))
				return false;
			if (c != ':')
				return false;

			JSONValue objectValue;
			if (!objectValue.parse(pos, end))
				return false;
			(*this)[key] = objectValue;
			if (!nextChar(c, pos, end, true))
				return false;
			if (c == '}')
				break;
			else if (c != ',')
				return false;
		}
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
void JSONValue::setType(JSONType newType)
{
	if (m_type == newType)
		return;

	switch (m_type)
	{
	case JSON_STRING:
		(reinterpret_cast<String*>(&m_data))->~String();
		break;

	case JSON_ARRAY:
		(reinterpret_cast<JSONArray*>(&m_data))->~JSONArray();
		break;

	case JSON_OBJECT:
		(reinterpret_cast<JSONObject*>(&m_data))->~JSONObject();
		break;

	default:
		break;
	}

	m_type = newType;

	switch (m_type)
	{
	case JSON_STRING:
		new(reinterpret_cast<String*>(&m_data)) String();
		break;

	case JSON_ARRAY:
		new(reinterpret_cast<JSONArray*>(&m_data)) JSONArray();
		break;

	case JSON_OBJECT:
		new(reinterpret_cast<JSONObject*>(&m_data)) JSONObject();
		break;

	default:
		break;
	}
}
//-----------------------------------------------------------------------------
void JSONValue::writeJSONString(String& dest, const String& str)
{
	dest += '\"';

	for (auto it = str.Begin(); it != str.End(); ++it)
	{
		char c = *it;

		if (c >= 0x20 && c != '\"' && c != '\\')
			dest += c;
		else
		{
			dest += '\\';

			switch (c)
			{
			case '\"':
			case '\\':
				dest += c;
				break;

			case '\b':
				dest += 'b';
				break;

			case '\f':
				dest += 'f';
				break;

			case '\n':
				dest += 'n';
				break;

			case '\r':
				dest += 'r';
				break;

			case '\t':
				dest += 't';
				break;

			default:
			{
				char buffer[6];
				sprintf(buffer, "u%04x", c);
				dest += buffer;
			}
			break;
			}
		}
	}

	dest += '\"';
}
//-----------------------------------------------------------------------------
void JSONValue::writeIndent(String& dest, int indent)
{
	size_t oldLength = dest.Length();
	dest.Resize(oldLength + indent);
	for (int i = 0; i < indent; ++i)
		dest[oldLength + i] = ' ';
}
//-----------------------------------------------------------------------------
bool JSONValue::readJSONString(String& dest, const char*& pos, const char*& end, bool inQuote)
{
	char c;

	if (!inQuote)
	{
		if (!nextChar(c, pos, end, true) || c != '\"')
			return false;
	}

	dest.Clear();
	for (;;)
	{
		if (!nextChar(c, pos, end, false))
			return false;
		if (c == '\"')
			break;
		else if (c != '\\')
			dest += c;
		else
		{
			if (!nextChar(c, pos, end, false))
				return false;
			switch (c)
			{
			case '\\':
				dest += '\\';
				break;

			case '\"':
				dest += '\"';
				break;

			case 'b':
				dest += '\b';
				break;

			case 'f':
				dest += '\f';
				break;

			case 'n':
				dest += '\n';
				break;

			case 'r':
				dest += '\r';
				break;

			case 't':
				dest += '\t';
				break;

			case 'u':
			{
				// \todo Doesn't handle unicode surrogate pairs
				unsigned code;
				sscanf(pos, "%x", &code);
				pos += 4;
				dest.AppendUTF8(code);
			}
			break;
			}
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
bool JSONValue::matchString(const char* str, const char*& pos, const char*& end)
{
	while (*str)
	{
		if (pos >= end || *pos != *str)
			return false;
		else
		{
			++pos;
			++str;
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
bool JSONValue::matchChar(char c, const char*& pos, const char*& end)
{
	char next;

	while (nextChar(next, pos, end, false))
	{
		if (next == c)
			return true;
	}
	return false;
}
//-----------------------------------------------------------------------------