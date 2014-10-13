//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: core/WDynString.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>

/**
* WDynString class is used to store dynamically allocated string. May be used to save memory.
* String stored in this class is null terminated so can be used in any standard c methods.
*/
class WDynString
{
private:
	wchar_t * m_buffer;
	unsigned int m_size;

	void clear()
	{
		if (m_buffer)
		{
			delete[] m_buffer;
			m_buffer = 0;
		}
		m_size = 0;
	}
public:
	WDynString()
	{
		m_buffer = new wchar_t[1];
		m_buffer[0] = '\0';
		m_size = 0;
	}

	WDynString(const WDynString& string)
	{
		size_t size = string.m_size;
		m_buffer = new wchar_t[size + 1];
		wcscpy(m_buffer, string.m_buffer);
		m_buffer[size] = '\0';
		m_size = size;
	}

	WDynString(const wchar_t *buffer)
	{
		size_t size = wcslen(buffer);
		m_buffer = new wchar_t[size + 1];
		wcscpy(m_buffer, buffer);
		m_buffer[size] = '\0';
		m_size = size;
	}

	template <int sizeOfString>
	WDynString(WString<sizeOfString> buffer)
	{
		size_t size = wcslen(buffer.get());
		m_buffer = new wchar_t[size + 1];
		wcscpy(m_buffer, buffer.get());
		m_buffer[size] = L'\0';
		m_size = size;
	}

	// NOTE: We need to do format-based constructor here - but how :-)
	// if ill find some idea how to "guess" size of desired buffer ill do it
	// for now ill leave it without that kind of constructor

	~WDynString()
	{
		clear();
	}

	const wchar_t * get() const
	{
		return m_buffer;
	}

	WDynString& operator=(const WDynString& rhs)
	{
		clear();
		m_buffer = new wchar_t[rhs.m_size + 1];
		wcscpy(m_buffer, rhs.m_buffer);
		m_buffer[rhs.m_size] = '\0';
		m_size = rhs.m_size;
		return *this;
	}

	bool operator==(const WDynString& rhs)
	{
		return !wcscmp(m_buffer, rhs.m_buffer);
	}

	wchar_t operator[](unsigned int index) const
	{
		if (index < 0 || index > m_size)
			return 0;

		return m_buffer[index];
	}

	unsigned int getSize()
	{
		return m_size;
	}

	unsigned int getLength() const
	{
		return m_size;
	}
};