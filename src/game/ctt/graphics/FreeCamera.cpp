//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/FreeCamera.cpp
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#include "FreeCamera.h"

#include <core/Logger.h>

#include <SDL.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include <game/Game.h>
#include <physics/PhysicsWorld.h>

FreeCamera::FreeCamera()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);
	for (unsigned int i = 0; i < 6; ++i)
		m_keys[i] = false;

	m_position = Vector3(10, 10, 0);
	m_target = Vector3(0, 0, 0);
	m_speed = 25.f;
	m_sensitivity = 0.15f;
	updateMatrix();
}

FreeCamera::~FreeCamera()
{

}

void FreeCamera::onMouseMove(int x, int y, int relx, int rely)
{
	float _x = (float)relx * m_sensitivity;
	float _y = (float)rely * m_sensitivity;

	m_rotationX = glm::rotate(m_rotationX, _x, Vector3(0,1,0));
	m_rotationY = glm::rotate(m_rotationY, _y, Vector3(1,0,0));

	updateMatrix();
}

void FreeCamera::onKeyEvent(int key, bool state)
{
	if (key == 'w')
		m_keys[0] = state;

	if (key == 's')
		m_keys[1] = state;

	if (key == 'a')
		m_keys[2] = state;

	if (key == 'd')
		m_keys[3] = state;

	if (key == 'e')
		m_keys[4] = state;

	if (key == 'q')
		m_keys[5] = state;


	if (key == 0x400000E1) // shift
		m_speed = state ? 100.0f : 25.0f;
	else if (key == 0x400000E2) // alt
		m_speed = state ? 5.0f : 25.0f;	
}

void FreeCamera::update(float dt)
{
	Vector3 begin = m_position;
	bool update = false;
	if (m_keys[0])
	{
		m_position += (m_target - begin) * m_speed * dt;
		update = true;
	}

	if (m_keys[1])
	{
		m_position -= (m_target - begin) * m_speed * dt;
		update = true;
	}

	if (m_keys[2])
	{
		Vector3 direction = glm::cross(m_target - begin, Vector3(0,1,0));
		m_position -= direction * m_speed * dt;
		update = true;
	}

	if (m_keys[3])
	{
		Vector3 direction = glm::cross(m_target - begin, Vector3(0, 1, 0));
		m_position += direction * m_speed * dt;
		update = true;
	}

	if (m_keys[4])
	{
		m_position += Vector3(0, 1, 0) * m_speed * dt;
		update = true;
	}

	if (m_keys[5])
	{
		m_position -= Vector3(0, 1, 0) * m_speed * dt;
		update = true;
	}

	Vector3 point;
	if (Game::get()->getPhysicsWorld()->rayTest(begin, m_position + (m_position - begin), &point))
	{
		m_position = begin;
	}

	if (update)
		updateMatrix();
}

void FreeCamera::updateMatrix()
{
	m_viewMatrix = glm::mat4_cast(m_rotationY) * glm::mat4_cast(m_rotationX) * glm::translate(glm::mat4(), Vector3(-m_position.x, -m_position.y, -m_position.z));
	m_target = m_position + Vector3(-m_viewMatrix[0][2], -m_viewMatrix[1][2], -m_viewMatrix[2][2]);
}