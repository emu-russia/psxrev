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

#ifndef _LISTBOX_H42631_INCLUDED_
#define _LISTBOX_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "ScrollBar.h"
#include "EditableText.h"
#include "callback.h"
#include "ResourceManager.h"

namespace begui {

class ListBox : public Component
{
	class Item {
	public:
		EditableText	m_text;
		bool			m_bSelected;
		bool			m_bEnabled;
		Rect<int>		m_rect;

		Item() : m_bSelected(false), m_bEnabled(true) { }
		Item(const std::string &str) : m_bSelected(false), m_bEnabled(true) { m_text.setText(str); }
	};

public:
	enum Style {
		STYLE_FLAT,
		STYLE_STRIPES,
		STYLE_BUTTONS
	};
	enum SelectionMode {
		SINGLE_SELECT,
		MULTI_SELECT,		// multiple selection with ctrl
		MULTI_SELECT_SINGLECLICK	// multiple selection with single click (click again to deselect)
	};

protected:
	std::vector<Item>	m_items;
	Style				m_style;
	int					m_curItem, m_prevItem;
	SelectionMode		m_selectMode;
	ScrollBar			m_scroller;
	bool				m_bEditable;	// text in the list items is editable
	bool				m_bHighlightMouseOver;	// highlights the item that is under the mouse cursor
	int					m_mouseOverItem;	// item under the current position of the mouse cursor

	Functor1<int>		m_onItemSelect;	// arg1: the id of the *LAST* selected item. If multiple selection
										// is enabled, user has to check which items are actually selected

	ResourceManager::ImageRef	m_bg, m_selection;
	Rect<int>					m_activeArea, m_resizableArea;
	Rect<int>					m_selectionActiveArea, m_selectionResizableArea;
	Rect<int>					m_contentPadding;
	Rect<int>					m_scrollBarPadding;
	Color						m_textColor;
	bool						m_bAutoHeight;
	int							m_maxHeight;

public:
	ListBox();
	virtual ~ListBox();

	virtual void create(int x, int y, int width, int height, SelectionMode selMode, Style style=STYLE_FLAT,
		const std::string &style_name = "std");
	void setAutoHeight(bool bAuto)	{ m_bAutoHeight = bAuto; }

	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	void		addItem(const std::string &item)	{ m_items.push_back(Item(item)); }
	int			itemsNum() const					{ return (int)m_items.size(); }
	std::string	itemText(size_t i) const			{ return m_items[i].m_text.getText(); }
	bool		itemEnabled(size_t i) const			{ return m_items[i].m_bEnabled; }
	bool		itemSelected(size_t i) const		{ return m_items[i].m_bSelected; }
	void		remItem(size_t pos)					{ m_items.erase(m_items.begin()+pos); }
	void		remAllItems()						{ m_items.clear(); m_curItem = 0; m_prevItem = -1; }
	int			getCurrentItem() const				{ return m_curItem; }
	void		setCurrentItem(int i)				{ m_curItem = i; }
	void		enableItem(size_t i)				{ m_items[i].m_bEnabled = true; }
	void		disableItem(size_t i)				{ m_items[i].m_bEnabled = false; }
	void		selectItem(size_t i, bool bSel)		{ m_items[i].m_bSelected = bSel; }
	void		setHighlightOnMouseOver(bool b)		{ m_bHighlightMouseOver = b; }

	void	handleOnItemSelect(Functor1<int> &f)		{ m_onItemSelect = f; }

private:
	void selectRange(size_t start, size_t end);
	void deselectAll();
	void makeItemVisible(int item);	// scroll the list so that item is visible

	int	getItemHeight() const;
};

};

#endif