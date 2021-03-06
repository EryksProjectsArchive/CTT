//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/VertexBuffer.cpp
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#include "BufferBase.h"
#include "renderer/Renderer.h"

#include "VertexBuffer.h"
#include <core/Logger.h>
#include <graphics/Vertex3d.h>

VertexBuffer::VertexBuffer()
{
}

void VertexBuffer::fillData(void * data)
{
	Renderer::glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	Renderer::glBufferData(GL_ARRAY_BUFFER, m_size, data, m_isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	Renderer::glBindBuffer(GL_ARRAY_BUFFER, 0);
}