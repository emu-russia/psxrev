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

#ifndef _VIEWPORTCOMPONENT_H42631_INCLUDED_
#define _VIEWPORTCOMPONENT_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "../bcore/Trackball.h"
#include "../bcore/Viewport.h"

namespace begui {

class ViewportComponent : public Component
{
private:
	Viewport	m_viewport;
	//void		(*m_pRenderCallback)();
	Functor0	m_renderCallback;
	bool		m_bNavigationEnabled;
	Trackball	m_trackball;
	Matrix4		m_viewMat;

	Vector2		m_lastClickPos;

public:
	ViewportComponent();

	void create(int x, int y, int w, int h, const Viewport& vp, Functor0 &render_callback = Functor0());
	void enableNavigation(bool bEnable)		{ m_bNavigationEnabled = bEnable; }

	Viewport&	getViewport()				{ return m_viewport; }
	bool		isNavigationEnabled() const	{ return m_bNavigationEnabled; }
	Matrix4&	getViewMatrix()				{ return m_viewMat; }
	Vector2		getLastClickPos() const		{ return m_lastClickPos; }

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