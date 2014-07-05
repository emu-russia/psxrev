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

#include "Menu.h"
#include "ResourceManager.h"
#include "Font.h"
#include "util.h"
#include "FrameWindow.h"

using namespace begui;

Menu::Menu() : m_id(-1),
	m_itemOpen(false),
	m_activeItem(-1),
	m_isMainMenu(false),
	m_contentWidth(100),
	m_contentHeight(25),
	m_pParent(0),
	m_bSeparator(false),
	m_bChecked(false),
	m_menuFaceResizableArea(0,0,0,0),
	m_textColor(0.0f,0.0f,0.0f)
{
	setAlwaysOnTop(true);
}

Menu::~Menu()
{
	for (size_t i=0; i<m_menuItems.size(); ++i)
		SAFE_DELETE(m_menuItems[i]);
}

void Menu::clear()
{
	for (size_t i=0; i<m_menuItems.size(); ++i)
		SAFE_DELETE(m_menuItems[i]);
	m_menuItems.clear();
}

void Menu::createMainMenu(const std::string &style_name)
{
	m_isMainMenu = true;
	m_right = display::getWidth();
	m_bottom = 25;

	// get the style properties
	ResourceManager::Style style = ResourceManager::inst()->getClassDef("Menu").style("std");
	ASSERT(style.hasProp("menubar_face"));
	m_menuFace = ResourceManager::inst()->loadImage(style.get_img("menubar_face"));
	m_menuFaceResizableArea = style.get_rect("resizable_area");
	if (style.hasProp("active_area"))
		m_menuFaceActiveArea = style.get_rect("active_area");
	else
		m_menuFaceActiveArea = Rect<int>(0, 0, m_menuFace.m_width, m_menuFace.m_height);
	if (style.hasProp("mainmenu_text_color"))
		m_textColor = style.get_c("mainmenu_text_color");
}

void Menu::close()
{
	m_activeItem = -1;
	m_itemOpen = false;
}

Menu* Menu::addMenuItem(const std::string &title, int id, Functor1<int> &callback)
{
	Menu *menu = new Menu();
	menu->m_title = title;
	menu->m_id = id;
	menu->m_pParent = this;
	menu->m_bSeparator = false;
	menu->m_onItemClick = callback;

	// set the region covered by the contracted menu
	if (m_isMainMenu)
	{
		int x_offs = 8;
		for (size_t i=0; i<m_menuItems.size(); ++i)
		{
			x_offs += Font::stringLength(m_menuItems[i]->m_title) + 20;
		}
		menu->m_left = x_offs - 5;
		menu->m_right = menu->m_left + Font::stringLength(title) + 10;
		menu->m_top = 1;
		menu->m_bottom = 14;
	}
	else
	{
		int w = Font::stringLength(title);
		menu->m_left = 5;//m_left+5;
		menu->m_right = menu->m_left + w + 10;
		menu->m_top = 5 + m_bottom + (int)m_menuItems.size()*16;
		menu->m_bottom = menu->m_top + 13;

		w += 55;
		m_contentWidth = (w>m_contentWidth)?w:m_contentWidth;
		m_contentHeight += 16;
	}

	m_menuItems.push_back(menu);
	
	return m_menuItems[m_menuItems.size()-1];
}


void Menu::addSeparator()
{
	Menu *menu = new Menu();
	menu->m_title = "------------";
	menu->m_id = -1;
	menu->m_pParent = this;
	menu->m_bSeparator = true;
	menu->setEnabled(false);

	// set the region covered by the contracted menu
	int w = Font::stringLength(menu->m_title);
	menu->m_left = m_left+5;
	menu->m_right = menu->m_left + w + 10;
	menu->m_top = 5 + m_bottom + (int)m_menuItems.size()*16;
	menu->m_bottom = menu->m_top + 13;

	w += 55;
	m_contentWidth = (w>m_contentWidth)?w:m_contentWidth;
	m_contentHeight += 16;

	// add the menu item
	m_menuItems.push_back(menu);
}

void Menu::onUpdate()
{
	// update the width of the menu, if it is a main menu attached
	// to a frame window
	if (m_isMainMenu && Component::m_pParent) {
		try {
			Container *pContainer = dynamic_cast<Container*>(Component::m_pParent);
			Window *pWnd = dynamic_cast<Window*>(pContainer->getParent());

			if (pWnd) {
				m_right = m_left + pWnd->getClientArea().getWidth();
			}
		}
		catch (std::bad_cast&) {
		}
	}
}

void Menu::onRender()
{
	int w = m_right-m_left;
	int h = 25;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,1);

	if (m_isMainMenu)
	{
		Component::drawImageWtBorders(m_menuFace, 0, 0, w, m_menuFace.m_height, m_menuFaceResizableArea);
	}
	else
	{
		// set the texture of a window
		Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::STD_CONTROLS);
		pTex->set();

		// This is a submenu or a menuitem
		if (m_menuItems.size() > 0)
		{
			glColor4f(1,1,1,0.9f);
			float tW = 512;	// texture width;
			float tH = 512;	// texture width;
			float wtL = 0;		// window left in pixels in texture
			float wtR = 511;		// window right
			float wtT = 76;		// top
			float wtB = 504;		// bottom
			float offs = 32;	// the size of the border
		//	int left = m_left - 12;
			int left = -12;
			int top = m_bottom + 3;
			int right = left + m_contentWidth;
			int bottom = top + m_contentHeight;
			Component::drawBorderedQuad(left, top, right, bottom,
										left+(int)offs, top, right-(int)offs, bottom-(int)offs,
										wtL/tW, (wtL+offs)/tW, wtR/tW, (wtR-offs)/tW,
										wtT/tH, (wtT+offs)/tH, wtB/tH, (wtB-offs)/tH);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// highlight selected menu item
	if (m_activeItem != -1 && !m_menuItems[m_activeItem]->m_bSeparator)
	{
		Menu *mi = m_menuItems[m_activeItem];
		if (m_itemOpen || !m_isMainMenu)
			glColor3f(0.9f, 0.5f, 0);
		else
			glColor3f(0.6f, 0.6f, 0.6f);
		int hl_right = mi->m_right;
		if (!m_isMainMenu)
			hl_right = mi->m_left + m_contentWidth - 40;
		glBegin(GL_QUADS);
			glVertex3f((float)mi->m_left, (float)mi->m_top, 0);
			glVertex3f((float)mi->m_right, (float)mi->m_top, 0);
			glVertex3f((float)mi->m_right, (float)mi->m_bottom, 0);
			glVertex3f((float)mi->m_left, (float)mi->m_bottom, 0);
			
			glColor4f(0.9f, 0.5f, 0, 1); glVertex3f((float)mi->m_right, (float)mi->m_top, 0);
			glColor4f(0.7f, 0.4f, 0, 1); glVertex3f((float)hl_right, (float)mi->m_top, 0);
			glColor4f(0.7f, 0.4f, 0, 1); glVertex3f((float)hl_right, (float)mi->m_bottom, 0);
			glColor4f(0.9f, 0.5f, 0, 1); glVertex3f((float)mi->m_right, (float)mi->m_bottom, 0);
		glEnd();
	}
	
	glDisable(GL_BLEND);

	// render menu item text
	for (size_t i=0; i<m_menuItems.size(); ++i)
	{
		// set the text color
		glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, 1.0f);
		if (m_menuItems[i]->m_bSeparator)
			glColor3f(0.6f,0.6f,0.6f);
		else if (!m_menuItems[i]->isEnabled())
			glColor3f(0.5f, 0.5f, 0.5f);
		else if (i == m_activeItem)
			glColor3f(1,1,1);

		// render the menu item text
		Font::renderString(m_menuItems[i]->m_left+5, m_menuItems[i]->m_top + 11, m_menuItems[i]->m_title);
	}
	glColor3f(1,1,1);

	Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::STD_CONTROLS);
	pTex->set();
	glEnable(GL_BLEND);
	
	// render the check marks next to menu items
	for (size_t i=0; i<m_menuItems.size(); ++i)
	{
		// if this menu item is checked, render the check mark
		if (m_menuItems[i]->isChecked())
		{
			Menu *mi = m_menuItems[i];

			// size of the checkmark
			float chW = 17;
			float chH = 20;
			// pos in the texture file.
			float chU = 405;
			float chV = 4;

			if (!m_menuItems[i]->isEnabled())
				glColor4f(0, 0, 0, 0.25f);
			else
				glColor4f(0,0,0, 0.8f);

			glBegin(GL_QUADS);
				glTexCoord2f(chU/512.0f, chV/512.0f);				glVertex3f((float)mi->m_right, (float)mi->m_bottom-11, 0);
				glTexCoord2f((chU+chW)/512.0f, chV/512.0f);			glVertex3f((float)mi->m_right+8, (float)mi->m_bottom-11, 0);
				glTexCoord2f((chU+chW)/512.0f, (chV+chH)/512.0f);	glVertex3f((float)mi->m_right+8, (float)mi->m_bottom-1, 0);
				glTexCoord2f(chU/512.0f, (chV+chH)/512.0f);			glVertex3f((float)mi->m_right, (float)mi->m_bottom-1, 0);
			glEnd();
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);

	// render the rolled-down submenu, if any
	if (m_itemOpen)
	{
		m_menuItems[m_activeItem]->frameRender();
	}
}

bool Menu::onMouseDown(int x, int y, int button)
{
	for (size_t i=0; i<m_menuItems.size(); ++i)
	{
		if (m_menuItems[i]->isPtInside(x, y))
		{
			m_activeItem = (int)i;
			if (m_menuItems[i]->m_menuItems.size() > 0)
			{
				// it's a submenu, open it
				m_itemOpen = !m_itemOpen;
			}
			else if (!m_menuItems[i]->m_bSeparator)
			{
				// the user clicked on a menu item
				// close the tree of submenus
				Menu *parent = m_pParent;
				while (parent)
				{
					parent->m_itemOpen = false;
					parent->m_activeItem = -1;
					parent = parent->m_pParent;
				}

				// call the callback associated with this item
				// to perform the associated action
				if (m_menuItems[i]->isEnabled())
					m_menuItems[i]->m_onItemClick(m_menuItems[i]->m_id);
			}
			return true;
		}
	}

	if (m_activeItem != -1 && m_itemOpen)
	{
		m_menuItems[m_activeItem]->onMouseDown(x,y,button);
	}
	return true;
}

bool Menu::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (!m_itemOpen && !isPtInside(x,y) && !isPtInsideSubmenu(x, y))
	{
		m_activeItem = -1;
		return false;
	}

	// Check if the mouse is over a menu item
	int oldActiveItem = m_activeItem;
	for (size_t i=0; i<m_menuItems.size(); ++i)
	{
		if (m_menuItems[i]->isPtInside(x, y))
		{
			m_activeItem = (int)i;
			break;
		}
	}

	// Check if the active item changed to update.
	if (oldActiveItem != m_activeItem)
	{
		m_menuItems[m_activeItem]->m_activeItem = -1;
	}
	
	// Let the active menu item handle mouse events for its contents
	if (m_activeItem != -1 && m_itemOpen)
	{
		m_menuItems[m_activeItem]->onMouseMove(x, y, prevx, prevy);
	}

	return true;
}

bool Menu::onMouseUp(int x, int y, int button)
{
	return true;
}

void Menu::onKeyDown(int key)
{
}

void Menu::onKeyUp(int key)
{
}

void Menu::onDeactivate()
{
	m_activeItem = -1;
	m_itemOpen = false;
}

bool Menu::isPtInside(int x, int y)
{
	if (m_activeItem != -1 && m_itemOpen)
	{
		if (m_menuItems[m_activeItem]->isPtInsideSubmenu(x, y))
			return true;
	}

	if (m_isMainMenu)
	{
		if (x < m_left+m_menuFaceActiveArea.left || x > m_right-(m_menuFace.m_width-m_menuFaceActiveArea.getWidth()))
			return false;
		if (y < m_top+m_menuFaceActiveArea.top || y > m_bottom-(m_menuFace.m_height-m_menuFaceActiveArea.getHeight()))
			return false;
		return true;
	}
	else
	{
		int r = m_right;
		if (!m_pParent->m_isMainMenu) {
			r = m_left + m_pParent->m_contentWidth;
		}

		if (x < m_left || x > r)
			return false;
		if (y < m_top || y > m_bottom)
			return false;
		return true;
	}
}

bool Menu::isPtInsideSubmenu(int x, int y)
{
	//ASSERT(!m_isMainMenu);

	if (x < m_left || x > m_left + m_contentWidth)
		return false;
	if (y < m_bottom || y > m_bottom + m_contentHeight)
		return false;
	return true;
}

Menu* Menu::getMenuItem(const std::string& title)
{
	if (m_title == title)
		return this;

	for (size_t i=0; i<m_menuItems.size(); ++i)
	{
		Menu* pItem = m_menuItems[i]->getMenuItem(title);
		if (pItem)
			return pItem;
	}
	return 0;
}

Menu* Menu::getMenuItem(int id)
{
	if (m_id == id)
		return this;

	for (size_t i=0; i<m_menuItems.size(); ++i)
	{
		Menu* pItem = m_menuItems[i]->getMenuItem(id);
		if (pItem)
			return pItem;
	}
	return 0;
}
