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

#ifndef _COMPONENT_H42631_INCLUDED_
#define _COMPONENT_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "timeseries.h"
#include "callback.h"
#include "ResourceManager.h"

namespace begui {

class Component
{
protected:
	TimeSeries<int>	m_left, m_right, m_top, m_bottom;
	Component	*m_pParent;

	bool	m_bAlwaysOnTop;
	bool	m_bVisible;
	bool	m_bHasMouseFocus;
	bool	m_bHasKeybFocus;
	bool	m_bActive;
	bool	m_bEnabled;
	bool	m_bFixedZOrder;

	TimeSeries<float>	m_alpha;

public:
	Component();

	int getLeft() const		{ return m_left; }
	int getTop() const		{ return m_top; }
	int getRight() const	{ return m_right; }
	int getBottom() const	{ return m_bottom; }
	int getWidth() const	{ return m_right - m_left; }
	int getHeight() const	{ return m_bottom - m_top; }

	virtual void setPos(int x, int y)	{ m_right=x+getWidth(); m_bottom=y+getHeight(); m_left=x; m_top=y; }
	virtual void setSize(int w, int h)	{ m_right = m_left+w; m_bottom = m_top + h; }
	void setFixedZOrder(bool fixed)		{ m_bFixedZOrder = fixed; }

	bool hasFixedZOrder() const	{ return m_bFixedZOrder; }
	bool isAlwaysOnTop() const	{ return m_bAlwaysOnTop; }
	bool isVisible() const		{ return m_bVisible; }
	bool isActive() const		{ return m_bActive; }
	bool isEnabled() const		{ return m_bEnabled; }

	float	getAlpha() const		{ return m_alpha; }
	void	setAlpha(float alpha);
	float	getHierarchyAlpha() const;

	bool hasMouseFocus() const	{ return m_bHasMouseFocus; }
	bool hasKeybFocus() const	{ return m_bHasKeybFocus; }
	virtual void getFocus();
	virtual void releaseFocus();
	virtual void getMouseFocus();
	virtual void getKeybFocus();
	virtual void releaseMouseFocus();
	virtual void releaseKeybFocus();

	virtual void setParent(Component *pParent)	{ m_pParent = pParent; }
	virtual Component* getParent() const		{ return m_pParent; }
	virtual void setAlwaysOnTop(bool ontop)	{ m_bAlwaysOnTop = ontop; }
	virtual void setVisible(bool visible)	{ m_bVisible = visible; }
	virtual void enable()					{ m_bEnabled = true; }
	virtual void disable()					{ m_bEnabled = false; }

	virtual void frameUpdate();
	virtual void frameRender();
	
	// event handlers. Override these methods to define how your
	// component behaves. Coordinates are in the PARENT COORDINATE SPACE
	virtual void onUpdate() { };
	virtual void onRender() = 0;
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onMouseWheel(int zDelta);
	virtual void onActivate() { m_bActive = true; };
	virtual void onDeactivate() { m_bActive = false; };

	// x,y in PARENT COORDINATES!
	virtual bool isPtInside(int x, int y);

	// Coordinate transformation
	Vector2i worldToLocal(const Vector2i& v) const;
	Vector2i parentToLocal(const Vector2i& v) const;
	Vector2i localToWorld(const Vector2i& v) const;
	Vector2i localToParent(const Vector2i& v) const;

	//helpers
	static void drawBorderedQuad(int left, int top, int right, int bottom,		// position to appear on screen
						int lB, int tB, int rB, int bB,							// border positions
						float texL, float texLB, float texR, float texRB,	// left to right positions in tex
						float texT, float texTB, float texB, float texBB);	// top to bottom
	static void drawImage(ResourceManager::ImageRef &image, int x, int y, int w=0, int h=0);

	// draw an image, using non-resizable borders and a resizable center area. Arguments
	// are the same as in drawImage(..), and resizable_area gives the coordinates in pixels
	// of the rectangle indicating the resizable area, relative to the top left corner of
	// the face described in the image.
	static void drawImageWtBorders(ResourceManager::ImageRef &image, int x, int y, 
						int w, int h, const Rect<int> &resizable_area);
};

};

#endif