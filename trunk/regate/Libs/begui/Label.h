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

#ifndef _LABEL_H42631_INCLUDED_
#define _LABEL_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "Font.h"

namespace begui {

class Label : public Component
{
private:
	std::string	m_text;
	Color		m_textColor;
	bool		m_bMultiLine;
	int			m_maxWidth;
	Font		*m_pFont;

public:
	Label();

	void create(int x, int y, const std::string& text);
	void createMultiline(int x, int y, int max_width, const std::string& text);

	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual bool isPtInside(int x, int y);

	virtual void setText(const std::string& text);
	virtual	void setTextColor(const Color &cl)		{ m_textColor = cl; }
};

};

#endif