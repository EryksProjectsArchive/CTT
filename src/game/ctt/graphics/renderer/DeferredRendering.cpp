//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/renderer/DeferredRendering.cpp
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#include "DeferredRendering.h"
#include "Renderer.h"

#include <resources/materials/MaterialLib.h>
#include <core/DynString.h>

#include <graphics/Material.h>
#include <graphics/ShaderProgram.h>
#include <graphics/Camera.h>
#include <graphics/Geometry.h>

DeferredRendering::DeferredRendering()
	: m_deferredResultMaterial(0), m_fbo(0),
	  m_diffuseTexture(0), m_normalTexture(0), m_depthTexture(0),
	  m_diffuseRenderBuffer(0), m_normalRenderBuffer(0), m_depthRenderBuffer(0),
	  m_renderer(0), m_width(0), m_height(0)
{
}

DeferredRendering::~DeferredRendering()
{
}

bool DeferredRendering::initialize(Renderer* renderer, uint32 width, uint32 height)
{
	m_deferredResultMaterial = MaterialLib::get()->findByName("deferredResult");
	if (!m_deferredResultMaterial)
		return false;

	m_deferredResultMaterial->acquire();

	m_width = width;
	m_height = height;
	m_renderer = renderer;

	// Setup deferred rendering

	Renderer::glGenFramebuffers(1, &m_fbo);
	Renderer::glGenRenderbuffers(1, &m_diffuseRenderBuffer);
	Renderer::glGenRenderbuffers(1, &m_normalRenderBuffer);
	Renderer::glGenRenderbuffers(1, &m_depthRenderBuffer);

	Renderer::glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	Renderer::glBindRenderbuffer(GL_RENDERBUFFER, m_diffuseRenderBuffer);
	Renderer::glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	Renderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_diffuseRenderBuffer);

	Renderer::glBindRenderbuffer(GL_RENDERBUFFER, m_normalRenderBuffer);
	Renderer::glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB16F_ARB, width, height);
	Renderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, m_normalRenderBuffer);

	Renderer::glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
	Renderer::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
	Renderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

	// Create diffuse texture
	glGenTextures(1, &m_diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach texture to frame buffer
	Renderer::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_diffuseTexture, 0);

	glGenTextures(1, &m_normalTexture);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach the texture to the FBO
	Renderer::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTexture, 0);

	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	Renderer::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	// Check if all worked fine and unbind the FBO
	GLenum status = Renderer::glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Renderer::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		return false;
	}

	Renderer::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void DeferredRendering::destroy()
{
	if (m_deferredResultMaterial)
		m_deferredResultMaterial->release();

	m_deferredResultMaterial = 0;
	
	if (m_normalTexture)
		glDeleteTextures(1, &m_normalTexture);

	m_normalTexture = 0;

	if(m_diffuseTexture)
		glDeleteTextures(1, &m_diffuseTexture);

	m_diffuseTexture = 0;

	if (m_fbo)
		Renderer::glDeleteFramebuffers(1, &m_fbo);

	m_fbo = 0;

	if (m_diffuseRenderBuffer)
		Renderer::glDeleteRenderbuffers(1, &m_diffuseRenderBuffer);

	m_diffuseRenderBuffer = 0;

	if (m_normalRenderBuffer)
		Renderer::glDeleteRenderbuffers(1, &m_normalRenderBuffer);

	m_normalRenderBuffer = 0;

	if (m_depthRenderBuffer)
		Renderer::glDeleteRenderbuffers(1, &m_depthRenderBuffer);

	m_depthRenderBuffer = 0;
}

void DeferredRendering::begin()
{
	Renderer::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glPushAttrib(GL_VIEWPORT_BIT);

	glViewport(0, 0, m_width, m_height);

	// Clear the render targets
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer::glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	Renderer::glDrawBuffers(2, buffers);
}

void DeferredRendering::end()
{
	Renderer::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glPopAttrib();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Renderer::glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	// Render final result
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Geometry<Vertex2d> geometry;

	Vertex2d vertices[4] = {
		{ 0, 0, 0, 1, 0xFFFFFFFF },
		{ (float)m_width, 0, 1, 1, 0xFFFFFFFF },
		{ (float)m_width, (float)m_height, 1, 0, 0xFFFFFFFF },
		{ 0, (float)m_height, 0, 0, 0xFFFFFFFF }
	};

	uint16 triangles[] = {
		0, 1, 2,
		0, 2, 3
	};

	geometry.fillData(vertices, 4, triangles, 2);

	if (!m_deferredResultMaterial)
		return;

	if (!m_deferredResultMaterial->m_program || !Renderer::glIsProgram(m_deferredResultMaterial->m_program->m_programId))
		return;

	Renderer::glUseProgram(m_deferredResultMaterial->m_program->m_programId);

	unsigned int orthoMatrixLocation = m_deferredResultMaterial->m_program->getUniformLocation("orthoMatrix");
	if (orthoMatrixLocation != -1)
	{
		Renderer::glUniformMatrix4fv(orthoMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_renderer->m_orthoMatrix));
	}

	unsigned int textureLocation = m_deferredResultMaterial->m_program->getUniformLocation("diffuseTexture");
	if (textureLocation != -1)
	{
		Renderer::glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
		Renderer::glUniform1i(textureLocation, 0);
	}

	textureLocation = m_deferredResultMaterial->m_program->getUniformLocation("normalTexture");
	if (textureLocation != -1)
	{
		Renderer::glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_normalTexture);
		Renderer::glUniform1i(textureLocation, 1);
	}

	textureLocation = m_deferredResultMaterial->m_program->getUniformLocation("depthTexture");
	if (textureLocation != -1)
	{
		Renderer::glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		Renderer::glUniform1i(textureLocation, 2);
	}

	unsigned int projectionMatrixLocation = m_deferredResultMaterial->m_program->getUniformLocation("projectionMatrix");
	if (projectionMatrixLocation != -1)
	{
		Renderer::glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_renderer->m_projectionMatrix));
	}

	glm::mat4x4 viewMatrix = Camera::current->getViewMatrix();
	unsigned int viewMatrixLocation = m_deferredResultMaterial->m_program->getUniformLocation("viewMatrix");
	if (viewMatrixLocation != -1)
	{
		Renderer::glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	unsigned int cameraPositionLocation = m_deferredResultMaterial->m_program->getUniformLocation("cameraPosition");
	if (cameraPositionLocation != -1)
	{
		Renderer::glUniform3f(cameraPositionLocation, Camera::current->getPosition().x, Camera::current->getPosition().y, Camera::current->getPosition().z);
	}

	glm::mat4 unProjectMatrix = glm::inverse(m_renderer->m_projectionMatrix * viewMatrix);

	unsigned int unProjectMatrixLocation = m_deferredResultMaterial->m_program->getUniformLocation("unProjectMatrix");
	if (unProjectMatrixLocation != -1)
	{
		Renderer::glUniformMatrix4fv(unProjectMatrixLocation, 1, GL_FALSE, glm::value_ptr(unProjectMatrix));
	}

	unsigned int attributePosition = m_deferredResultMaterial->m_program->getAttributeLocation("vertexPosition");
	unsigned int attributeUV = m_deferredResultMaterial->m_program->getAttributeLocation("vertexUV");
	unsigned int attributeColor = m_deferredResultMaterial->m_program->getAttributeLocation("vertexColor");

	Renderer::glEnableVertexAttribArray(attributePosition);
	if (attributeColor != -1)
	{
		Renderer::glEnableVertexAttribArray(attributeColor);
	}

	if (attributeUV != -1)
	{
		Renderer::glEnableVertexAttribArray(attributeUV);
	}

	Renderer::glBindBuffer(GL_ARRAY_BUFFER, geometry.m_vertexBuffer->m_bufferId);

	Renderer::glVertexAttribPointer(attributePosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2d), 0);
	if (attributeColor != -1)
		Renderer::glVertexAttribPointer(attributeColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2d), (void *)offsetof(Vertex2d, color));
	if (attributeUV != -1)
		Renderer::glVertexAttribPointer(attributeUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2d), (void *)offsetof(Vertex2d, u));

	Renderer::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.m_indexBuffer->m_bufferId);

	glDrawElements(m_deferredResultMaterial->m_wireframe ? GL_LINE_STRIP : GL_TRIANGLES, geometry.m_trianglesCount * 3, GL_UNSIGNED_SHORT, 0);

	Renderer::glDisableVertexAttribArray(attributePosition);
	if (attributeColor != -1)
	{
		Renderer::glDisableVertexAttribArray(attributeColor);
	}

	if (attributeUV != -1)
	{
		Renderer::glDisableVertexAttribArray(attributeUV);
	}
}