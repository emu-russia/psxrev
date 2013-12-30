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

#ifndef _RADIOBUTTON_H42631_INCLUDED_
#define _RADIOBUTTON_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "ResourceManager.h"

namespace begui {

class RadioButton : public Component
{
public:
	enum State {
		SELECTED = 0,
		NOT_SELECTED,
		INACTIVE,
		STATES_NUM
	};

private:
	State		m_state;
	std::string m_title;
	int			m_id;
	bool		m_bIsRadio;
	void		(*m_pCallback)(int);
	bool		m_bHover;

	ResourceManager::ImageRef	m_faces[STATES_NUM];
	Rect<int>					m_activeArea;

public:
	RadioButton();

	void create(int x, int y, const std::string &title, int id=0, void (*callback)(int) = 0, 
				const std::string &style_name = "std", bool bIsRadio = true);
	void setState(State state)	{ m_state = state; }
	State getState() const		{ return m_state; }
	void setCaption(const std::string& title)	{ m_title = title; }

	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual bool isPtInside(int x, int y);
};

};

#endif