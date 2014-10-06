//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/fonts/Font.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>

#include <core/String.h>
#include <core/DynString.h>
#include <core/SharedPtr.h>

class Font
{
private:
	uint32 m_textureId;

	SharedPtr<Material> m_material;

	bool m_loaded;
public:
	Font(FilePath fontPath, uint32 size, flags32 flags);
	~Font();

	void render(DynString string, const Rect& rect, const Color& color, flags32 flags);

public:
	struct CreationFlags
	{
		enum Type
		{
			None = 0, // 0b0
			Bold = 1, // 0b1
			Italic = 2, // 0b10
		};
	};

	struct DrawFlags
	{
		enum Type
		{
			None = 0,
			NoClip = 1,

			// Separated position enum?
			VerticalCenter = 2,
			HorizontalCenter = 3,
		};
	};

	friend class Renderer;
};