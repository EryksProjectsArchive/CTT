//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: resources/CacheableResource.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>

#include "Resource.h"

class CacheableResource : public Resource
{
private:
	unsigned int m_refCount;
public:
	CacheableResource(FilePath filePath);
	virtual ~CacheableResource();

	void acquire();
	void release();
};