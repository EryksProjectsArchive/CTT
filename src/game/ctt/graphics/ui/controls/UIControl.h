//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/ui/control/UIControl.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////

#pragma once

#include <Prerequisites.h>
#include <core/DynString.h>
#include <core/List.h>

namespace UI
{
	class Control
	{
	private:
		DynString m_name;
	protected:
		Vector2 m_position;
		Vector2 m_size;
		bool m_hover;

		List<Control *> m_children;
		Control *m_parent;
		Skin *m_skin;
	public:
		Control(const DynString& name, Vector2 position = Vector2(), Vector2 size = Vector2());
		virtual ~Control();

		DynString getName();

		virtual Control * getParent();

		// NOTE: This function overrides all user defined style changes for child.
		virtual void setParent(Control *control);

		// NOTE: This function overrides all user defined style changes for child.
		virtual void addChild(Control *control);
		virtual void removeChild(Control *control);

		virtual bool isChildOf(Control *control);
		virtual bool isParentOf(Control *control);

		virtual bool handleInput();

		virtual void setSkin(Skin *skin);
		virtual Skin * getSkin();

		virtual void render(RenderContext& context);
	};
}