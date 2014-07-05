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

#ifndef _CONTAINER_H42631_INCLUDED_
#define _CONTAINER_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"

namespace begui {

class Container : public Component
{
protected:
	// The list of child components. The children are stored in inverse z-order (the top
	// component is the last in the list)
	std::vector<Component*>	m_children;
	Component				*m_pActiveComponent;
	Component				*m_pModalComponent;

public:
	Container();
	virtual ~Container();
	
	virtual void		addComponent(Component *pC);
	virtual int			getChildrenNum() const	{ return (int)m_children.size(); }
	virtual Component*	getChild(int pos)		{ return m_children[pos]; }
	virtual void		remComponent(Component *pC);
	virtual bool		contains(Component *pC);
	virtual void		bringToFront(Component *pC);
	virtual void		showModal(Component *pC);
	virtual void		hideModal();

	// overridables
	virtual void onMouseDownEx(int x, int y) { };
	virtual void onMouseMoveEx(int x, int y, int prevx, int prevy) { };
	virtual void onMouseUpEx(int x, int y) { };
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onRender() { };

	virtual void frameUpdate();
	virtual void frameRender();
	
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onMouseWheel(int zDelta);

protected:
	void	bringChildToFront(int id);
	int		findChildId(Component *pC);
};

};

#endif