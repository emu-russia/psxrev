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

#ifndef _EDITABLETEXT_H42631_INCLUDED_
#define _EDITABLETEXT_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "../bcore/Rect.h"
#include "timeseries.h"
#include "LiveVar.h"

namespace begui {

class EditableText
{
public:
	EditableText();

	void create(int x, int y, int lineWidth, bool bMultiLine = true, bool bEditable = true,
				bool bTextSelectable = true);
	void update();
	void renderString();
	void setText(const std::string &text);
	void setCursorVisible(bool bVisible)			{ m_bRenderCursor = bVisible; }
	void setSelectionColor(Color cl, float alpha)	{ m_selectionColor = cl; m_selectionAlpha = alpha; }
	void setTextColor(Color cl, float alpha)		{ m_textColor = cl; m_textAlpha = alpha; }
	void setCursorColor(Color cl)					{ m_cursorColor = cl; }
	void setEditable(bool b)						{ m_bEditable = b; }
	void setTextSelectable(bool b)					{ m_bTextSelectable = b; }
	void setMultiline(bool b)						{ m_bMultiLine = b; setText(m_text); }
	void setTextHidden(bool b)						{ m_bTextHidden = b; }

	const std::string& getText() const				{ return m_text; }
	std::string	getSelectedText() const;
	int  getSelectionStart() const					{ return (m_selectStart < m_selectEnd)?m_selectStart:m_selectEnd; }
	int  getSelectionEnd() const					{ return (m_selectStart < m_selectEnd)?m_selectEnd:m_selectStart; }
	bool isEditable() const							{ return m_bEditable; }
	bool isMultiLine() const						{ return m_bMultiLine; }
	bool isTextHidden() const						{ return m_bTextHidden; }

	// live variable access
	LiveVar<std::string>&	text()	{ return m_text; }

	bool onMouseDown(int x, int y, int button);	// WC means world coordinates
	bool onMouseMove(int x, int y, int prevx, int prevy);
	bool onMouseUp(int x, int y, int button);
	void onKeyDown(int key);
	void onKeyUp(int key);

private:
	LiveVar<std::string> m_text;
	int		m_x, m_y;
	int		m_lineWidth;
	std::vector< Rect<int> > m_charPos;		// character positions
	bool	m_bMultiLine;
	bool	m_bEditable;
	bool	m_bTextHidden;					// used for password fields (characters are replaced by * if true)
	bool	m_bRenderCursor;
	bool	m_bTextSelectable;
	Color	m_textColor, m_selectionColor;
	float	m_textAlpha, m_selectionAlpha;
	Color	m_cursorColor;
	int		m_cursorX, m_cursorY, m_cursorH;
	int		m_cursorPos;			// cursor position in the string
	TimeSeries<float> m_cursorAlpha;
	int		m_selectStart, m_selectEnd;

	void setCursorPos(int cursorPos);
	void getStringPos(int x, int y, int *cursorX, int *cursorY, int *cursorH, int *char_pos) const;
	void cursorUp(int *cursorX, int *cursorY, int *cursorH, int* char_pos) const;
	void cursorDown(int *cursorX, int *cursorY, int *cursorH, int* char_pos) const;
	int getLineStart(int cursorPos) const;
	int getLineEnd(int cursorPos) const;
};

};

#endif