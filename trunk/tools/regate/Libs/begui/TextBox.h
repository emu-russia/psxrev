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

#ifndef _TEXTBOX_H42631_INCLUDED_
#define _TEXTBOX_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "EditableText.h"
#include "ResourceManager.h"

namespace begui {

class TextBox : public Component
{
private:
	EditableText m_text;
	bool		m_bTextSelectable;

	ResourceManager::ImageRef	m_bg;
	Rect<int>					m_activeArea, m_resizableArea;
	Rect<int>					m_textPadding;
	Color						m_textColor;

	ResourceManager::ImageRef	m_frame;
	Rect<int>					m_frameResizableArea, m_frameOffs;

public:
	TextBox();

	virtual void create(int x, int y, int width, int height, bool bEditable=true, bool bMultiline=true,
		const std::string &style_name = "std");
	virtual void createPasswordField(int x, int y, int width, int height, const std::string &style_name = "std");

	virtual void onUpdate();
	virtual void onRender();	
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	bool		isEditable() const					{ return m_text.isEditable(); }
	bool		isMultiline() const					{ return m_text.isMultiLine(); }
	void		setEditable(bool bEnable)			{ m_text.setEditable(bEnable); }
	void		setMultiline(bool bEnable)			{ m_text.setMultiline(bEnable); }
	std::string	getSelectedText() const;
	std::string getText() const						{ return m_text.getText(); }
	void		setText(const std::string &text)	{ m_text.setText(text); }
	void		hideText(bool bHide)				{ m_text.setTextHidden(bHide); }

};

};

#endif