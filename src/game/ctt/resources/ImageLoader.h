//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: resources/ImageLoader.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>
#include <core/String.h>
#include <core/List.h>
#include <core/Singleton.h>

#include <stdio.h>

class ImageLoader : public Singleton<ImageLoader>
{
private:
	List<ImageLoader *> m_loaders;
public:
	ImageLoader();
	virtual ~ImageLoader();

	virtual bool isFileValid(const FilePath& filePath);
	virtual void registerLoader(ImageLoader * loader);

	virtual ImageData * load(const FilePath& filePath);
};