//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: io/Config.cpp
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#include "Config.h"

#include <json/json.h>

#include <io/fs/FileSystem.h>

Config * Config::s_singleton = 0;

Config::Config()
{
	s_singleton = this;

	Info("CONSTRUCTOR", "Config");
}

Config::~Config()
{
	s_singleton = 0;

	for (Config::Entry * entry : m_entries)
		delete entry;

	m_entries.clear();
}

void Config::serialize(File *file)
{
	Json::StyledWriter writer;
	Json::Value root;
	root.setComment("// This file has been generated by City Transport Tycoon game.\n// Do not edit if if you are not sure about that what are you doing.", Json::CommentPlacement::commentBefore);

	for (Config::Entry *entry : m_entries)	
		entry->serialize(file, root);	

	std::string data = writer.write(root);
	file->write(data.c_str(), data.length(), sizeof(uint8));
	file->flush();
}

void Config::deserialize(File* file)
{
	if (file->isLoaded() && false)
	{
		DynString data = file->getContent();
		if (data.getLength() > 0)
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(data.get(), root))
			{
				for (std::string names : root.getMemberNames())
				{
					Config::Entry *newEntry = new Config::Entry(this, names.c_str());
					newEntry->deserialize(file, root);
				}
			}
		}
	}
}

Config& Config::get()
{
	return *s_singleton;
}

Config::Entry* Config::find(DynString name)
{
	for (Config::Entry *entry : m_entries)
	{
		if (entry->m_name == name)
		{
			return entry;
		}
	}
	return new Config::Entry(this, name);
}

Config::Entry& Config::operator[](DynString name)
{
	return *find(name);
}

Config::Entry::Entry(Config * config, DynString name)
	: m_name(name), m_type(Config::Entry::ValueType::Empty), m_config(config)
{
	if (m_config) // NOTE: It's possible to set config to null if this entry is a subchild of entry!
		m_config->m_entries.pushBack(this);
}

Config::Entry::~Entry()
{
}

uint32 Config::Entry::getInteger(uint32 def)
{
	if (m_type == Config::Entry::ValueType::Integer)
		return m_data.integerValue;
	
	m_type = Config::Entry::ValueType::Integer;
	m_data.integerValue = def;
	return def;
}

float Config::Entry::getFloat(float def)
{
	if (m_type == Config::Entry::ValueType::Float)
		return m_data.floatValue;
	
	m_type = Config::Entry::ValueType::Float;
	m_data.floatValue = def;
	return def;
}

List<Config::Entry *> Config::Entry::getArrayData()
{
	return m_data.arrayData;
}

DynString Config::Entry::getString(DynString def)
{
	if (m_type == Config::Entry::ValueType::String) 
		return m_data.stringData;
	
	m_type = Config::Entry::ValueType::String;
	m_data.stringData = def;
	return def;
}

bool Config::Entry::getBool(bool def)
{
	if (m_type == Config::Entry::ValueType::Boolean)
		return m_data.booleanData;

	m_type = Config::Entry::ValueType::Boolean;
	m_data.booleanData = def;
	return def;
}

Config::Entry* Config::Entry::find(DynString name)
{
	for (Config::Entry *entry : m_data.arrayData)
	{
		if (entry->m_name == name)
		{
			return entry;
		}
	}
	m_type = Config::Entry::ValueType::Array;
	Config::Entry * entry = new Config::Entry(NULL, name);
	m_data.arrayData.pushBack(entry);
	return entry;
}

Config::Entry& Config::Entry::operator[](DynString name)
{
	return *find(name);
}

void Config::Entry::serialize(File *file, Json::Value& parent)
{
	switch (m_type)
	{
	case Config::Entry::ValueType::Array:
		{
			for (Config::Entry *entry : m_data.arrayData)
				entry->serialize(file, parent[m_name]);
		} break;
	case Config::Entry::ValueType::Boolean:
		{					
			parent[m_name] = m_data.booleanData;
		} break;
	case Config::Entry::ValueType::Integer:
		{
			parent[m_name] = m_data.integerValue;
		} break;
	case Config::Entry::ValueType::Float:
		{
			parent[m_name] = m_data.floatValue;
		} break;
	case Config::Entry::ValueType::Empty:
		{
			parent[m_name] = Json::nullValue;
		} break;
	case Config::Entry::ValueType::String:
		{
			parent[m_name] = m_data.stringData.get();
		} break;
	}
}

void Config::Entry::deserialize(File* file, Json::Value& parent)
{
	switch(parent[m_name].type())
	{
	case Json::ValueType::arrayValue:
		{
			m_type = Config::Entry::ValueType::Array;
			for (std::string names : parent[m_name].getMemberNames())
			{
				Config::Entry *newEntry = new Config::Entry (m_config, names.c_str());
				newEntry->deserialize(file, parent[m_name]);
			}
		} break;
	case Json::ValueType::intValue:
		{ 
			m_type = Config::Entry::ValueType::Integer;
			m_data.integerValue = parent[m_name].asInt();
		} break;
	case Json::ValueType::realValue:
		{
			m_type = Config::Entry::ValueType::Float;
			m_data.floatValue = parent[m_name].asFloat();
		} break;
	case Json::ValueType::booleanValue:
		{
			m_type = Config::Entry::ValueType::Boolean;
			m_data.booleanData = parent[m_name].asBool();
		} break;
	case Json::ValueType::nullValue:
		{
			m_type = Config::Entry::ValueType::Empty;
		} break;
	case Json::ValueType::stringValue:
		{
			m_type = Config::Entry::ValueType::String;
			m_data.stringData = parent[m_name].asCString();
		} break;
	}
}