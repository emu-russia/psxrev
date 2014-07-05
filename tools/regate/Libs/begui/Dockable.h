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

#ifndef _DOCKABLE_H42631_INCLUDED_
#define _DOCKABLE_H42631_INCLUDED_

#pragma once

#include "Component.h"

namespace begui
{

class Dockable : public Component
{
public:
	enum DockSide {
		DOCK_LEFT,
		DOCK_RIGHT,
		DOCK_TOP,
		DOCK_BOTTOM
	};
	enum Align {
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_LEFT_RIGHT,
		ALIGN_TOP,
		ALIGN_BOTTOM,
		ALIGN_TOP_BOTTOM,
		ALIGN_MIDDLE_HORZ,
		ALIGN_MIDDLE_VERT
	};

public:
	virtual void attachTo(Component *pC, DockSide sideOfTarget, Align align, bool bAllowResize);
	virtual void detachFrom(Component *pC);
	virtual bool isDocked(DockSide side);

	virtual void frameUpdate();

	// behavior to implement in the future:
	// 1- dock to the client area of windows (for toolbars)
	// 2- dock to other components when dragged by the mouse (auto-snap)
	// 3- a rigorous definition of rules for global solution in general graphs
	//    of dependencies.

private:
	struct DockRelation {
		Component*	dockTarget;
		DockSide	sideOfTarget;
		Align		alignment;
		bool		bAllowResize;	// allow this component to resize to satisfy the constraints
	};

	// relations (a list of components to which this component is attached)
	std::vector<DockRelation>	m_relations;
	// components that are attached to this component (so, they should be updated when this component changes)
	std::vector<Component*>		m_dependents;

	virtual void addDependent(Component *pC);
	virtual void remDependent(Component *pC);
};

};

#endif