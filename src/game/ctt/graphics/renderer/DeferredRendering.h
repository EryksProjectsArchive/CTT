//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/renderer/DeferredRendering.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>

#include <graphics/Material.h>

class DeferredRendering
{
private:
	Material* m_deferredResultMaterial;

	uint32 m_fbo;

	uint32 m_diffuseTexture;
	uint32 m_normalTexture;
	uint32 m_depthTexture;
	uint32 m_materialParameterTexture;

	uint32 m_diffuseRenderBuffer;
	uint32 m_normalRenderBuffer;
	uint32 m_depthRenderBuffer;
	uint32 m_materialParameterRB;

	uint32 m_width;
	uint32 m_height;
	uint32 m_internalWidth;
	uint32 m_internalHeight;

	uint32 m_depthShadowTexture;

	Renderer *m_renderer;
public:
	DeferredRendering();
	~DeferredRendering();

	bool initialize(Renderer *renderer, uint32 width, uint32 height, float scaling);
	void destroy();

	void setShadowTexture(uint32 depthShadowTexture);

	void begin();
	void end(const Matrix4x4& shadowMatrixParameter);
};