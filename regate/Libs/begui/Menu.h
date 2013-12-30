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

#ifndef _MENU_H42631_INCLUDED_
#define _MENU_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "callback.h"
#include "ResourceManager.h"

namespace begui {

class Menu : public Component
{
private:
	std::vector<Menu*>	m_menuItems;
	Menu				*m_pParent;
	bool				m_itemOpen;
	int					m_activeItem;
	int					m_isMainMenu;

	std::string			m_title;
	int					m_id;
	Functor1<int>		m_onItemClick;	// arg1: the id of the clicked item
	bool				m_bSeparator;	// this menu item is a seperator
	bool				m_bChecked;		// a check mark is displayed next to the menu item

	int m_contentWidth, m_contentHeight;

	ResourceManager::ImageRef	m_menuFace;
	Rect<int>					m_menuFaceResizableArea;
	Rect<int>					m_menuFaceActiveArea;
	Color						m_textColor;

public:
	Menu();
	virtual ~Menu();

	void createMainMenu(const std::string &style = "std");
	void close();
	void clear();

	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual bool isPtInside(int x, int y);
			bool isPtInsideSubmenu(int x, int y);
	virtual void onDeactivate();

	Menu*	addMenuItem(const std::string &title, int id, Functor1<int>& callback);
	void	addSeparator();
	Menu*	getMenuItem(const std::string &title);
	Menu*	getMenuItem(int id);
	void	removeMenuItem(Menu*);

	void	setChecked(bool bChecked)	{ m_bChecked = bChecked; }
	void	setEnabled(bool bEnabled)	{ m_bEnabled = bEnabled; }
	bool	isChecked() const			{ return m_bChecked; }
	void	setTitle(const std::string& title);
};

};

#endif