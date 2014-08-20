//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/Model.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>

#include <resources/Resource.h>
#include "Mesh.h"

class RenderContext;

class Model : public Resource
{
private:
	unsigned char m_meshesCount;
	Mesh** m_meshes;
public:
	Model();
	~Model() override;
	
	void destroy() override;
	bool load(FilePath file) override;

	void render(RenderContext& renderContext);
};