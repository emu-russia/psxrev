/* 
// Copyright 2009 Alexandros Panagopoulos
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

#include "TabContainer.h"
#include "Font.h"
#include "util.h"

using namespace begui;

TabContainer::TabContainer() :
	m_curTab(0),
	m_bCanCreateTabs(false),
	m_bCanCloseTabs(false),
	m_bCanDragTabs(false),
	m_activeTabTextColor(0,0,0),
	m_inactiveTabTextColor(1,1,1),
	m_tabTextPadding(5),
	m_minTabWidth(60)
{
}

TabContainer::~TabContainer()
{
	clear();
}

void TabContainer::clear()
{
	for (size_t i=0; i<m_tabs.size(); ++i)
		SAFE_DELETE(m_tabs[i]);
	m_tabs.clear();
}

void TabContainer::create(int left, int top, int width, int height, const std::string &style_name,
							bool bCanCreateTabs, bool bCanCloseTabs)
{
	setPos(left, top);
	setSize(width, height);

	m_bCanCreateTabs = bCanCreateTabs;
	m_bCanCloseTabs = bCanCloseTabs;

	// load the associated style
	ResourceManager::Style style = ResourceManager::inst()->getClassDef("TabContainer").style(style_name);
	ASSERT(style.hasProp("tab_active"));
	m_tabActiveImg = ResourceManager::inst()->loadImage(style.get_img("tab_active"));
	ASSERT(style.hasProp("tab_inactive"));
	m_tabInactiveImg = ResourceManager::inst()->loadImage(style.get_img("tab_inactive"));
	ASSERT(style.hasProp("client_area_bg"));
	m_clientAreaImg = ResourceManager::inst()->loadImage(style.get_img("client_area_bg"));
	if (style.hasProp("resizable_area"))
		m_resizableArea = style.get_rect("resizable_area");
	if (style.hasProp("active_area"))
		m_activeArea = style.get_rect("active_area");
	if (style.hasProp("tab_resizable_area"))
		m_tabResizableArea = style.get_rect("tab_resizable_area");
	if (style.hasProp("tab_active_area"))
		m_tabActiveArea = style.get_rect("tab_active_area");
	if (style.hasProp("active_tab_text"))
		m_activeTabTextColor = style.get_c("active_tab_text");
	if (style.hasProp("inactive_tab_text"))
		m_inactiveTabTextColor = style.get_c("inactive_tab_text");
	if (style.hasProp("tab_text_padding"))
		m_tabTextPadding = style.get_i("tab_text_padding");
	if (style.hasProp("tab_min_width"))
		m_minTabWidth = style.get_i("tab_min_width");
	if (style.hasProp("active_bottom_img"))
		m_activeBtmImg = ResourceManager::inst()->loadImage(style.get_img("active_bottom_img"));
}

void TabContainer::addTab(const std::string &title)
{
	Tab *pTab = new Tab;
	pTab->m_title = title;
	pTab->m_icon = 0;
	pTab->setParent(this);
	m_tabs.push_back(pTab);
}

void TabContainer::remTab(int tab)
{
	ASSERT(tab>=0 && tab<m_tabs.size());
	SAFE_DELETE(m_tabs[tab]);
	m_tabs.erase(m_tabs.begin()+tab);
}

void TabContainer::remAllTabs()
{
	for (size_t i=0; i<m_tabs.size(); ++i)
		SAFE_DELETE(m_tabs[i]);
	m_tabs.clear();
}

void TabContainer::addComponent(Component *pC, int tab)
{
	ASSERT(tab>=0 && tab<m_tabs.size());
	ASSERT(pC);
	m_tabs[tab]->addComponent(pC);
}

void TabContainer::remComponent(Component *pC)
{
	ASSERT(pC);

	// find in which tab this component is
	for (size_t i=0; i<m_tabs.size(); ++i) {
		if (tabContains((int)i, pC)) {
			m_tabs[i]->remComponent(pC);
		}
	}
}

void TabContainer::setTabTitle(int tab, const std::string &title)
{
	ASSERT(tab>=0 && tab<m_tabs.size());
	m_tabs[tab]->m_title = title;
}

void TabContainer::setTabIcon(int tab, ResourceManager::ImageRef *icon)
{
	ASSERT(tab>=0 && tab<m_tabs.size());
	m_tabs[tab]->m_icon = icon;
}

std::string TabContainer::getTabTitle(int tab) const
{
	ASSERT(tab>=0 && tab<m_tabs.size());
	return m_tabs[tab]->m_title;
}

bool TabContainer::tabContains(int tab, Component *pC)
{
	ASSERT(tab>=0 && tab<m_tabs.size());
	ASSERT(pC);
	return m_tabs[tab]->contains(pC);
}

bool TabContainer::contains(Component *pC)
{
	for (size_t i=0; i<m_tabs.size(); ++i) {
		if (tabContains((int)i, pC))
			return true;
	}
	return false;
}

void TabContainer::frameUpdate()
{
	for (size_t i=0; i<m_tabs.size(); ++i)
		m_tabs[i]->frameUpdate();
	Container::frameUpdate();
}

void TabContainer::frameRender()
{
	glEnable(GL_BLEND);

	Font *pFont = FontManager::getCurFont();
	int header_h = m_tabActiveArea.getHeight();

	// Render the tab bg
	glColor4f(1,1,1,1);
	Component::drawImageWtBorders(m_clientAreaImg, getLeft()-m_activeArea.left, 
		header_h + getTop()-m_activeArea.top, 
		getWidth()+(m_clientAreaImg.m_width - m_activeArea.right)+m_activeArea.left, 
		getHeight()+(m_clientAreaImg.m_height - m_activeArea.bottom)+m_activeArea.top, m_resizableArea);

	// Render the tab headers
	int tab_x = getLeft()-m_tabActiveArea.left + m_resizableArea.left;
	for (size_t i=0; i<m_tabs.size(); ++i)
	{
		int tab_w = m_minTabWidth;
		int text_w = pFont->stringLength(m_tabs[i]->m_title) + 2*m_tabTextPadding;
		if (text_w > tab_w)
			tab_w = text_w;

		// save tab coords (local coords)
		m_tabs[i]->m_headerLeft = tab_x - getLeft();
		m_tabs[i]->m_headerRight = tab_x+tab_w - getLeft();

		if (i == m_curTab) {
			glColor4f(1,1,1,1);
			Component::drawImageWtBorders(m_tabActiveImg, tab_x, 
				getTop()-m_tabActiveArea.top, 
				tab_w, 
				m_tabActiveImg.m_height,
				m_tabResizableArea);

			// render the text
			glColor4f(m_activeTabTextColor.r, m_activeTabTextColor.g, m_activeTabTextColor.b,1);
			Font::renderString(tab_x + m_tabTextPadding, getTop()+header_h-4, m_tabs[i]->m_title);

			// render a small indicator that the tab is open
			if (m_activeBtmImg.m_texture) {
				glColor4f(1,1,1,1);
				Component::drawImage(m_activeBtmImg, tab_x + tab_w/2 - m_activeBtmImg.m_width/2, getTop()+header_h);
			}
		}
		else {
			glColor4f(1,1,1,1);
			Component::drawImageWtBorders(m_tabInactiveImg, tab_x, 
				getTop()-m_tabActiveArea.top, 
				tab_w, 
				m_tabInactiveImg.m_height,
				m_tabResizableArea);
			
			// render the text
			glColor4f(m_inactiveTabTextColor.r, m_inactiveTabTextColor.g, m_inactiveTabTextColor.b,1);
			Font::renderString(tab_x + m_tabTextPadding, getTop()+header_h-4, m_tabs[i]->m_title);
		}

		tab_x += tab_w+1;
	}
	
	// let the container call onRender()
	Container::frameRender();
}

void TabContainer::onRender()
{
	// render the contents of the active tab
	if (m_curTab >= 0 && m_curTab < (int)m_tabs.size()) {
		// mask the client area
		Vector2i wpos = m_tabs[m_curTab]->localToWorld(Vector2i(0,0));
		display::pushMask(wpos.x, wpos.y, getWidth(), getHeight());

		m_tabs[m_curTab]->setPos(0, m_tabActiveArea.getHeight());
		m_tabs[m_curTab]->frameRender();
		
		// unmask the client area
		display::popMask();
	}
}

bool TabContainer::onMouseDown(int x, int y, int button)
{
	Vector2i lP = parentToLocal(Vector2i(x,y));

	// check for events on the tab headers
	if (lP.y<m_tabActiveArea.getHeight()) {
		for (size_t i=0; i<m_tabs.size(); ++i) {
			if (lP.x>=m_tabs[i]->m_headerLeft && lP.x<=m_tabs[i]->m_headerRight) {
				m_curTab = (int)i;
				return true;
			}
		}
	}

	if (m_curTab >= 0 && m_curTab < (int)m_tabs.size())
		return m_tabs[m_curTab]->onMouseDown(lP.x,lP.y,button);
	return Container::onMouseDown(x,y,button);
}

bool TabContainer::onMouseMove(int x, int y, int prevx, int prevy)
{
	Vector2i lP = parentToLocal(Vector2i(x,y));
	Vector2i lpP = parentToLocal(Vector2i(prevx,prevy));

	// check for events on the tab headers
	// TODO

	if (m_curTab >= 0 && m_curTab < (int)m_tabs.size())
		return m_tabs[m_curTab]->onMouseMove(lP.x,lP.y,lpP.x,lpP.y);
	return Container::onMouseMove(x,y,prevx,prevy);
}

bool TabContainer::onMouseUp(int x, int y, int button)
{
	Vector2i lP = parentToLocal(Vector2i(x,y));

	// check for events on the tab headers
	// TODO

	if (m_curTab >= 0 && m_curTab < (int)m_tabs.size())
		return m_tabs[m_curTab]->onMouseUp(lP.x,lP.y,button);
	return Container::onMouseUp(x,y,button);
}

void TabContainer::onKeyDown(int key)
{
	//TODO: handle tab/space to switch tabs

	if (m_curTab >= 0 && m_curTab < (int)m_tabs.size())
		return m_tabs[m_curTab]->onKeyDown(key);
	return Container::onKeyDown(key);
}

void TabContainer::onKeyUp(int key)
{
	if (m_curTab >= 0 && m_curTab < (int)m_tabs.size())
		return m_tabs[m_curTab]->onKeyUp(key);
	return Container::onKeyUp(key);
}

void TabContainer::handleTabToFront(Functor1<int> &callback)
{
	m_onTabToFront = callback;
}

void TabContainer::handleTabToBack(Functor1<int> &callback)
{
	m_onTabToBack = callback;
}

void TabContainer::handleTabCreate(Functor1<int> &callback)
{
	m_onTabCreate = callback;
}

void TabContainer::handleTabClose(Functor1<int> &callback)
{
	m_onTabClose = callback;
}

void TabContainer::handleTabDragStart(Functor1<int> &callback)
{
	m_onTabDragStart = callback;
}

void TabContainer::handleTabDragEnd(Functor1<int> &callback)
{
	m_onTabDragEnd = callback;
}