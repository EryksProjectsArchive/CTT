//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: core/WWString.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>

#include <cstring>
#include <cstdarg>

template <int maxSize>
class WString
{
private:
	wchar_t m_buffer[maxSize];
public:
	WString()
	{
		memset(m_buffer, 0, maxSize * sizeof(wchar_t));
	}

	WString(const WString<maxSize>& string)
	{
		memcpy(m_buffer, string.m_buffer, maxSize* sizeof(wchar_t));
	}

	WString(const wchar_t *buffer)
	{
		memset(m_buffer, 0, maxSize* sizeof(wchar_t));

		size_t size = wcslen(buffer);
		if (size > maxSize)
			size = maxSize;

		memcpy(m_buffer, buffer, size* sizeof(wchar_t));
	}

	WString(wchar_t *buffer)
	{
		memset(m_buffer, 0, maxSize* sizeof(wchar_t));

		size_t size = wcslen(buffer);
		if (size > maxSize)
			size = maxSize;

		strcpy(m_buffer, buffer * sizeof(wchar_t));
	}

	explicit WString(const wchar_t *buffer, ...)
	{
		wchar_t tempBuffer[maxSize] = { 0 };
		va_list args;
		va_start(args, buffer);
		vswprintf(tempBuffer, buffer, args);
		va_end(args);
		memcpy(m_buffer, tempBuffer, maxSize * sizeof(wchar_t));
	}

	WString::~WString()
	{
	}

	WString& append(const wchar_t *buffer)
	{
		size_t size = wcslen(buffer);
		size_t length = getLength();

		if (length + size >= maxSize)
			size = maxSize - length;

		if (size > 0)
		{
			memcpy(((wchar_t *)m_buffer + length), buffer, size * sizeof(wchar_t));
		}

		return *this;
	}

	WString& operator=(WString& string)
	{
		unsigned int size = wcslen(string.m_buffer);
		memcpy(m_buffer, string.m_buffer, (size > maxSize ? maxSize : size) * sizeof(wchar_t));
		return *this;
	}

	WString& operator=(const wchar_t *buffer)
	{
		uint32 size = wcslen(buffer);
		memcpy(m_buffer, buffer, (size > maxSize ? maxSize : size) * sizeof(wchar_t));
		return *this;
	}

	WString operator+(WString& string) const
	{
		return WString(L"%s%s", m_buffer, string.m_buffer);
	}

	WString operator+(const wchar_t *buffer) const
	{
		return WString(L"%s%s", m_buffer, buffer);
	}

	unsigned wchar_t operator[](unsigned int index)
	{
		if (index < 0 || index > maxSize)
			return 0;

		return m_buffer[index];
	}

	operator const wchar_t*() const
	{
		return m_buffer;
	}

	const wchar_t * WString::get() const
	{
		return m_buffer;
	}

	size_t WString::getLength()
	{
		return wcslen(m_buffer);
	}

	unsigned short WString::getMaxSize()
	{
		return maxSize;
	}

	uint16 WString::find(const wchar_t * key) const
	{
		uint32 range = wcslen(m_buffer);
		if (range + wcslen(key) > maxSize)
			range = maxSize;

		for (uint32 i = 0; i < range; ++i)
		{
			if (!memcmp(m_buffer + i, key, wcslen(key)*  sizeof(wchar_t)))
			{
				return i;
			}
		}

		return -1;
	}

	WString operator+=(const wchar_t *value)
	{
		return append(value);
	}

	WString operator+=(unsigned wchar_t c)
	{
		wchar_t value[2] = { 0 };
		value[0] = c;
		return append(value);
	}

	WString substr(uint16 begin, uint16 length)
	{
		WString newWString;
		if (begin < maxSize)
		{
			uint16 end = begin + length;
			if (end > maxSize)
				end = maxSize;
			for (uint16 i = begin; i < end; ++i)
			{
				newWString += m_buffer[i];
			}
		}
		return newWString;
	}

	WString replace(const wchar_t *key, const wchar_t *value)
	{
		WString newWString;
		uint16 keyPos = find(key);

		wchar_t temp[maxSize] = { 0 };
		wcscpy(temp, keyPos < wcslen(key) ? substr(wcslen(key), wcslen(m_buffer)) : substr(keyPos + wcslen(key), wcslen(m_buffer)));

		wcscpy(newWString.m_buffer, value);
		wcscat(newWString.m_buffer, temp);
		return newWString;
	}
};