//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/Model.cpp
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#include <stdio.h>

#include <core/Logger.h>

#include "Model.h"
#include "ModelFormat.h"


Model::Model()
{
	m_meshesCount = 0;
	m_meshes = 0;
}

Model::~Model()
{

}

void Model::destroy()
{
	if (m_isLoaded)
	{
		for (unsigned char i = 0; i < m_meshesCount; i++)
		{
			delete m_meshes[i];
		}
		delete m_meshes;
	}
}

bool Model::load(FilePath file)
{
	if (!m_isLoaded)
	{
		FILE *fp = fopen(file, "rb");

		if (fp)
		{
			mdl mdlData;
			if (ModelFormat::load(&mdlData, fp))
			{
				Info("Model", "Model loaded. %d meshes", mdlData.meshCount);

				m_meshes = new Mesh*[mdlData.meshCount];
				for (unsigned char i = 0; i < mdlData.meshCount; ++i)
				{
					Info("Model", "Mesh: %s", mdlData.meshes[i].material.value);

					m_meshes[i] = new Mesh(&mdlData.meshes[i]);

					delete[] mdlData.meshes[i].name.value;
					delete[] mdlData.meshes[i].material.value;
					delete[] mdlData.meshes[i].triangles;
					delete[] mdlData.meshes[i].vertices;
				}
				m_meshesCount = mdlData.meshCount;

				delete[] mdlData.meshes;

				m_isLoaded = true;
				return true;
			}
			fclose(fp);			
		}
	}
	return false;
}

void Model::render(RenderContext & renderContext)
{
	for (unsigned char i = 0; i < m_meshesCount; i++)
		m_meshes[i]->render(renderContext);	
}