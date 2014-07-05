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

#ifndef _REGIONSELECTGIZMO_H42631_INCLUDED_
#define _REGIONSELECTGIZMO_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"

namespace begui {

class RegionSelectGizmo : public Component
{
private:
	int		m_minX, m_maxX;
	int		m_minY, m_maxY;
	std::string	m_label;
	bool	m_bClickable;
	int		m_id;
	void	(*m_pCallback)(int);
	int		m_labelW, m_labelH;

	int		m_hoverId;	// num of part under mouse
	bool	m_bDragging;

	int		m_limMinX, m_limMinY;
	int		m_limMaxX, m_limMaxY;

public:
	RegionSelectGizmo();

	void create(int minX, int minY, int maxX, int maxY, const std::string& label = "", int id=-1, void (*callback)(int)=0);
	void setLimits(int minX, int minY, int maxX, int maxY);

	int	getMinX() const	{ return m_minX; }
	int	getMaxX() const	{ return m_maxX; }
	int	getMinY() const	{ return m_minY; }
	int	getMaxY() const	{ return m_maxY; }

	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual bool isPtInside(int x, int y);
};

};

#endif