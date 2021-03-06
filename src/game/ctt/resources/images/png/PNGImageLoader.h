//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: resources/images/png/PNGImageLoader.h
// Author	: Patryk �awicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>
#include <resources/ImageLoader.h>
#include <Windows.h>

namespace PNG
{
	class ImageLoader : public ::ImageLoader
	{

	public:
		ImageLoader();
		~ImageLoader();

		bool isFileValid(const FilePath& filePath);
		ImageData * load(const FilePath& filePath);
	};
};