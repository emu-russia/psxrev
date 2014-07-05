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

#ifndef _CHECKBOX_H42631_INCLUDED_
#define _CHECKBOX_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "LiveVar.h"
#include "ResourceManager.h"

namespace begui {

class CheckBox : public Component
{
public:
	CheckBox();
	virtual ~CheckBox();

	void create(int x, int y, const std::string &title, int id, const Functor1<int> &callback = Functor1<int>(),
				const std::string &style_name="std");

	// change the state of the check box
	void setState(bool state)	{ m_state = state; }
	bool getState() const		{ return m_state; }
	LiveVar<bool>& state()		{ return m_state; }

	// set the event handlers
	void handleOnClick(const Functor1<int> &fun)	{ m_onClick = fun; }

	// overriden methods
	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	
private:
	std::string		m_title;
	int				m_id;
	LiveVar<bool>	m_state;	// the state of the checkbox, true for checked
	Functor1<int>	m_onClick;	// arg1: the id of the checkbutton
	bool			m_bHover;

	ResourceManager::ImageRef	m_faceChecked, m_faceUnchecked;
	Rect<int>		m_activeArea;

};

};

#endif