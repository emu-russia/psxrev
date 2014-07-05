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

#include "ComboBox.h"
#include "Font.h"
#include "ResourceManager.h"

using namespace begui;

ComboBox::ComboBox() : m_bIsOpen(false), m_bEditable(false), m_curItem(-1), 
	m_textPos(0,0), m_textColor(255,255,255)
{
}

ComboBox::~ComboBox()
{
}

void ComboBox::create(int x, int y, int width, int list_height, const std::string &style_name)
{
	// load the combo box style sheet
	ResourceManager::Style style = ResourceManager::inst()->getClassDef("ComboBox").style(style_name);
	m_face = ResourceManager::inst()->loadImage(style.get_img("face"));
	m_expandIcon = ResourceManager::inst()->loadImage(style.get_img("expand_icon"));
	m_activeArea = style.get_rect("active_area");
	m_resizableArea = style.get_rect("resizable_area");
	m_textColor = style.get_c("text_color");
	if (style.hasProp("text_pos_x"))
		m_textPos.x = style.get_i("text_pos_x");
	else
		m_textPos.x = 0;
	if (style.hasProp("text_pos_y"))
		m_textPos.y = style.get_i("text_pos_y");
	else
		m_textPos.y = 0;
	int height;
	if (style.hasProp("default_height"))
		height = style.get_i("default_height");
	else
		height = m_activeArea.getHeight();
	
	// set the component dimensions
	setPos(x,y);
	setSize(width, height);

	m_listbox.create(0, height+2, width, list_height, ListBox::SINGLE_SELECT, ListBox::STYLE_FLAT, "std_shadow");
	m_listbox.setHighlightOnMouseOver(true);
	m_listbox.setAutoHeight(true);
	m_listbox.handleOnItemSelect(makeFunctor(*this, &ComboBox::onItemClick));
	m_bIsOpen = false;

	m_curItem = -1;
	m_text = "<select>";
}

void ComboBox::onUpdate()
{
}

void ComboBox::onRender()
{
	Font *pFont = FontManager::getCurFont();
	ASSERT(pFont);
	int lineHeight = pFont->getLineHeight()+2;
	
	glEnable(GL_BLEND);
	glColor4f(1,1,1,1);

	// draw the background
	Component::drawImageWtBorders(m_face, -m_activeArea.left, -m_activeArea.top,
		getWidth()+m_activeArea.left + (m_face.m_width-m_activeArea.right), 
		getHeight()+m_activeArea.top + (m_face.m_height-m_activeArea.bottom), 
		m_resizableArea);

	// draw the icon
	Component::drawImage(m_expandIcon, getWidth() - m_expandIcon.m_width - 5,
		getHeight()/2 - m_expandIcon.m_height/2);

	glColor4f(m_textColor.r*255, m_textColor.g*255, m_textColor.b*255, 1.0f);
	pFont->renderString(m_textPos.x, m_textPos.y, m_text);
}

bool ComboBox::onMouseDown(int x, int y, int button)
{
	// if the expand button was clicked:
	if (x>=m_left && x<=m_right && y>=m_top && y<=m_bottom) {
		if (!m_bIsOpen) {
			m_bIsOpen = true;
			addComponent(&m_listbox);
		}
		else {
			m_bIsOpen = false;
			remComponent(&m_listbox);
		}
	}
	bool bHandled = Container::onMouseDown(x,y,button);
	if (m_bIsOpen) {
		m_pActiveComponent = &m_listbox;
		m_listbox.getFocus();
	}
	else {
		m_listbox.releaseFocus();
		m_pActiveComponent = 0;
	}
	return bHandled;
}

bool ComboBox::onMouseMove(int x, int y, int prevx, int prevy)
{
	return Container::onMouseMove(x,y,prevx,prevy);
}

bool ComboBox::onMouseUp(int x, int y, int button)
{
	return Container::onMouseUp(x,y,button);
}

void ComboBox::onKeyDown(int key)
{
	Container::onKeyDown(key);
}

void ComboBox::onKeyUp(int key)
{
	Container::onKeyUp(key);
}

void ComboBox::onMouseWheel(int zDelta)
{
	Container::onMouseWheel(zDelta);
}

bool ComboBox::isPtInside(int x, int y)
{
	if (m_bIsOpen) {
		Vector2i lPt = Component::parentToLocal(Vector2i(x,y));
		if (m_listbox.isPtInside(lPt.x,lPt.y))
			return true;
	}
	if (x<m_left || x>m_right || y<m_top || y>m_bottom)
		return false;
	return true;
}

void ComboBox::onDeactivate()
{
	// close the listbox
	m_bIsOpen = false;
	remComponent(&m_listbox);
}

void ComboBox::onItemClick(int i)
{
	// set the new selected item
	if (i >= 0) {
		//m_text.setText(m_listbox.itemText(i));
		m_text = m_listbox.itemText(i);
		m_curItem = i;
	}

	// close the listbox
	m_bIsOpen = false;
	remComponent(&m_listbox);
}

void ComboBox::setCurrentItem(int i)
{
	ASSERT(i>=-1 && i<m_listbox.itemsNum());

	// set the current item
	m_curItem = i;

	// make this item current in the listbox
	m_listbox.setCurrentItem(i);

	// deselect all items, select i, in the listbox
	for (int j=0; j<m_listbox.itemsNum(); ++j)
		m_listbox.selectItem(j, false);
	m_listbox.selectItem(i, true);

	// set the text of the combobox
	if (i > -1)
		m_text = m_listbox.itemText(i);
}