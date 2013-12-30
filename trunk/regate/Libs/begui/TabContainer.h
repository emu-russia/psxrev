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

#ifndef _TABCONTAINER_H42631_INCLUDED_
#define _TABCONTAINER_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Container.h"
#include "ResourceManager.h"

namespace begui {

/**
 * TabContainer: implements a container with multiple tabs. The user can click
 *		on the tabs to change the active page, add new tabs, close tabs or drag
 *		the tabs around.
 *
 */
class TabContainer : public Container
{
public:
	TabContainer();
	virtual ~TabContainer();

	virtual void	create(int left, int top, int width, int height, const std::string &style_name = "std",
							bool bCanCreateTabs=false, bool bCanCloseTabs=false);
	virtual void	clear();
	virtual void	addTab(const std::string &title);
	virtual void	remTab(int tab);
	virtual void	remAllTabs();
	virtual size_t	getTabsNum() const			{ return m_tabs.size(); }
	virtual void	addComponent(Component *pC, int tab);
	virtual void	setTabTitle(int tab, const std::string &title);
	virtual void	setTabIcon(int tab, ResourceManager::ImageRef *icon);
	virtual std::string	getTabTitle(int tab) const;
	virtual bool	tabContains(int tab, Component *pC);

	// properties:
	virtual void	setCanCreateTabs(bool b)	{ m_bCanCreateTabs = b; }
	virtual void	setCanCloseTabs(bool b)		{ m_bCanCloseTabs = b; }
	virtual void	setCanDragTabs(bool b)		{ m_bCanDragTabs = b; }
			bool	canCreateTabs() const		{ return m_bCanCreateTabs; }
			bool	canCloseTabs() const		{ return m_bCanCloseTabs; }
			bool	canDragTabs() const		{ return m_bCanDragTabs; }

	// overrides:
	virtual void remComponent(Component *pC);
	virtual bool contains(Component *pC);
	virtual void frameUpdate();
	virtual void frameRender();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	// event hooks:
	void handleTabToFront(Functor1<int> &callback = Functor1<int>());	// called when a tab is selected and made visible
	void handleTabToBack(Functor1<int> &callback = Functor1<int>());	// called when the previous active tab is made invisible
	void handleTabCreate(Functor1<int> &callback = Functor1<int>());
	void handleTabClose(Functor1<int> &callback = Functor1<int>());
	void handleTabDragStart(Functor1<int> &callback = Functor1<int>());
	void handleTabDragEnd(Functor1<int> &callback = Functor1<int>());

protected:
	class Tab : public Container
	{
	public:
		std::string m_title;
		ResourceManager::ImageRef *m_icon;
		int m_headerLeft, m_headerRight;

		Tab() : m_icon(0), m_headerLeft(0), m_headerRight(0) { }
	};

	std::vector<Tab*>	m_tabs;
	int					m_curTab;
	bool				m_bCanCreateTabs;
	bool				m_bCanCloseTabs;
	bool				m_bCanDragTabs;

	// event hooks:
	Functor1<int> m_onTabToFront;
	Functor1<int> m_onTabToBack;
	Functor1<int> m_onTabCreate;
	Functor1<int> m_onTabClose;
	Functor1<int> m_onTabDragStart;
	Functor1<int> m_onTabDragEnd;

	// appearance
	ResourceManager::ImageRef m_tabActiveImg, m_tabInactiveImg, m_clientAreaImg, m_activeBtmImg;
	Rect<int>			m_resizableArea, m_activeArea;
	Rect<int>			m_tabResizableArea, m_tabActiveArea;
	Color				m_activeTabTextColor, m_inactiveTabTextColor;
	int					m_tabTextPadding, m_minTabWidth;

private:
	// hide the original addComponent method
	virtual void addComponent(Component *pC)	{ ASSERT(0); }
};

};

#endif