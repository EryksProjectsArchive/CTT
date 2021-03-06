//////////////////////////////////////////////
//
//		   City Transport Tycoon
//	   Copyright (C) Black Ice Mountains
//		 	All rights reserved
//
// File		: graphics/ui/controls/UIButton.h
// Author	: Eryk Dwornicki
//
//////////////////////////////////////////////
#pragma once

#include <Prerequisites.h>

#include <core/WDynString.h>
#include <core/DynString.h>

#include <graphics/fonts/Font.h>

#include "UIControl.h"
#include "UILabel.h"
#include "../UIEventHandler.h"

#include <graphics/Geometry.h>
#include <list>

namespace UI
{
	class Button : public Label
	{
	protected:
		bool m_pressed;

		struct OnPressData
		{
			EventHandler* handler;
			EventHandler::eventCallback callback;
			EventHandler::eventCallbackEmpty callbackEmpty;

			OnPressData()
			{
				handler = 0;
				callback = 0;
				callbackEmpty = 0;
			}

			OnPressData(const OnPressData& data)
			{
				handler = data.handler;
				callback = data.callback;
				callbackEmpty = data.callbackEmpty;
			}

			OnPressData& operator=(const OnPressData& data)
			{	
				handler = data.handler;
				callback = data.callback;
				callbackEmpty = data.callbackEmpty;
				return *this;
			}

			void call(UI::Control * control)
			{
				if (handler)
				{
					if (callback)
						(handler->*callback)(control);

					if (callbackEmpty)
						(handler->*callbackEmpty)();
				}
			}
		};
		List<OnPressData> m_onPressData;

		void onPressInternal(EventHandler * handler, EventHandler::eventCallback fn);
		void onPressInternal(EventHandler * handler, EventHandler::eventCallbackEmpty fn);
	public:
		Button(const DynString& name, Vector2 position = Vector2(), Vector2 size = Vector2());
		virtual ~Button();

		virtual void render(RenderContext& context);

		virtual bool handleInput();

		template <typename T>
		void onPressSubscribe(EventHandler * handler, void (T::*fn)()) { onPressInternal(handler, static_cast<EventHandler::eventCallbackEmpty>(fn)); }		

		template <typename T>
		void onPressSubscribe(EventHandler * handler, void (T::*fn)(Control* control)) { onPressInternal(handler, static_cast<EventHandler::eventCallback>(fn)); }
	};
}