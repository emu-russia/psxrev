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

#ifndef _COMBOBOX_H42631_INCLUDED_
#define _COMBOBOX_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "ListBox.h"
#include "Container.h"
#include "ResourceManager.h"

namespace begui {

class ComboBox : public Container
{
protected:
	ListBox				m_listbox;
	int					m_curItem;
	std::string			m_text;
	bool				m_bEditable;	// text in the text area is editable

	bool				m_bIsOpen;	// true if the list is open

	ResourceManager::ImageRef	m_face, m_expandIcon;
	Rect<int>					m_activeArea, m_resizableArea;
	Color						m_textColor;
	Vector2i					m_textPos;

public:
	ComboBox();
	virtual ~ComboBox();

	virtual void create(int x, int y, int width, int list_height, const std::string &style_name = "std");

	// event handlers
	void handleOnSelect(const Functor1<int> &onSelect);
	void handleOnExpand(const Functor1<int> &onExpand);
	void handleOnType(const Functor1<int> &onType);

	// overrides
	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onMouseWheel(int zDelta);
	virtual bool isPtInside(int x, int y);
	virtual void onDeactivate();

	// manage items
	void		addItem(const std::string &item)	{ m_listbox.addItem(item); }
	int			itemsNum() const					{ return m_listbox.itemsNum(); }
	bool		itemEnabled(size_t i) const			{ return m_listbox.itemEnabled(i); }
	void		remItem(int pos)					{ m_listbox.remItem(pos); }
	void		remAllItems()						{ m_listbox.remAllItems(); }
	int			getCurrentItem() const				{ return m_curItem; }
	void		setCurrentItem(int i);
	void		enableItem(size_t i)				{ m_listbox.enableItem(i); }
	void		disableItem(size_t i)				{ m_listbox.disableItem(i); }

private:
	void	onItemClick(int i);	// called when the user makes a selection
};

};

#endif