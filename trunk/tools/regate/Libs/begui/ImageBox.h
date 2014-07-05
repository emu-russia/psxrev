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

#ifndef _IMAGEBOX_H42631_INCLUDED_
#define _IMAGEBOX_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "../bcore/Image.h"

namespace begui {

class ImageBox : public Component
{
protected:
	Image	*m_pImage;
	Texture	m_texture;
	bool	m_bResizeImg;	// stretch image to fill the ImageBox area

	std::vector<Vector2> m_selLine;
	bool	m_bSelectable;
	
	Functor1<Vector2i>	m_onMouseDown, m_onMouseUp;
	Functor1<int>		m_onKeyDown, m_onKeyUp;
	Functor0			m_onRender;

public:
	ImageBox();

	virtual void create(int x, int y, int width, int height, Image *pImg, bool bResizeImg = false);

	virtual void setImage(Image *pImg);
	virtual void handleMouseDown(const Functor1<Vector2i> fun)	{ m_onMouseDown = fun; }
	virtual void handleMouseUp(const Functor1<Vector2i> fun)	{ m_onMouseUp = fun; }
	virtual void handleKeyDown(const Functor1<int> fun)			{ m_onKeyDown = fun; }
	virtual void handleKeyUp(const Functor1<int> fun)			{ m_onKeyUp = fun; }
	virtual void handleRender(const Functor0 fun)				{ m_onRender = fun; }

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