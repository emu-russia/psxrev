/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of BeGUI library.
//
//    BeGUI is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    BeGUI is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with BeGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BUTTON_H42631_INCLUDED_
#define _BUTTON_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "ResourceManager.h"

namespace begui {

class Button : public Component
{
public:
	enum State {
		DOWN = 0,
		UP,
		INACTIVE,
		MOUSE_OVER,
		STATES_NUM
	};
	enum IconPlacement {
		FAR_LEFT,
		NEAR_LEFT,
		FAR_RIGHT,
		NEAR_RIGHT,
		TOP,
		BOTTOM,
		CUSTOM
	};

private:
	std::string		m_title;
	int				m_id;
	Functor1<int>	m_onClick, m_onDragStart, m_onDragEnd;	// arg1: the id of the button
	Functor2<int, const Vector2i&>	m_onButtonDown, m_onButtonUp, m_onButtonDrag; // arg1: id, arg2: position (offset for drag)
	State			m_status;
	bool			m_bAutoSzX, m_bAutoSzY;				// true to determine the button size automatically
	bool			m_bCanDrag;							// true if the button can be dragged around with the mouse
	bool			m_bRepeatClick;						// true to generated repeatitive onClick events when button is held down
	Vector2i		m_lastClickPos;
	unsigned long	m_lastClickTime;
	unsigned int	m_repeatClickInterval;				// the interval between repeated onClick events, when the button is held down

	ResourceManager::ImageRef	m_faces[STATES_NUM];	// the faces corresponding to each state of the button
	ResourceManager::ImageRef	m_icon;
	int							m_iconSzX, m_iconSzY;
	IconPlacement				m_iconPlacement;
	Rect<int>					m_activeArea, m_resizableArea;
	Color						m_textColor, m_inactiveTextColor, m_btnColor;

public:
	Button();

	void create(int x, int y, const std::string &title, int id, Functor1<int> &callback = Functor1<int>(), 
				const std::string &style = "std");
	void create(int x, int y, int w, int h, const std::string &title, int id, 
				Functor1<int> &callback = Functor1<int>(), 
				const std::string &style = "std");

	virtual void onUpdate();
	virtual void onRender();

	// event hooks
	void	handleClick(Functor1<int> &callback)						{ m_onClick = callback; }
	void	handleDragStart(Functor1<int> &callback)					{ m_onDragStart = callback; }
	void	handleDragEnd(Functor1<int> &callback)						{ m_onDragEnd = callback; }
	void	handleButtonDown(Functor2<int, const Vector2i&> &callback)	{ m_onButtonDown = callback; }
	void	handleButtonUp(Functor2<int, const Vector2i&> &callback)	{ m_onButtonUp = callback; }
	void	handleDrag(Functor2<int, const Vector2i&> &callback)		{ m_onButtonDrag = callback; }

	virtual	void	setState(State state)										{ m_status = state; }
	virtual	State	getState() const											{ return m_status; }
	virtual	void	setTitle(const std::string& title)							{ m_title = title; }
	virtual	void	setFace(State state, const ResourceManager::ImageRef &img, 
							const Rect<int> *active_area = 0, const Rect<int> *resizeable_area = 0);
	virtual	void	setIcon(const ResourceManager::ImageRef &icon, IconPlacement place = NEAR_LEFT, int x_sz=0, int y_sz=0);
	virtual	void	setCanDrag(bool bDrag)										{ m_bCanDrag = bDrag; }
	virtual	bool	canDrag() const												{ return m_bCanDrag; }
	virtual	void	setColor(const Color &cl)									{ m_btnColor = cl; }
	virtual	void	setTextColor(const Color &cl)								{ m_textColor = cl; }
	virtual	void	setInactiveTextColor(const Color &cl)						{ m_inactiveTextColor = cl; }

	// if true, when the button is held down, multiple onClick events will be generated. The
	// rate is defined by the frame rate for synchronous apps, and by the click repeat rate
	// for asynchronous apps. The default is false.
	virtual void	repeatClickOnHold(bool bEnable)								{ m_bRepeatClick = bEnable; }
	virtual void	setRepeatClickInterval(unsigned int interv)					{ m_repeatClickInterval = interv; }
	
	Rect<int>	getActiveBorders() const	{ return Rect<int>(m_activeArea.left, m_activeArea.top, m_faces[UP].m_width-m_activeArea.right, m_faces[UP].m_height-m_activeArea.bottom); }

	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	virtual bool isPtInside(int x, int y);
	virtual void disable();
	virtual void enable();

};

};

#endif