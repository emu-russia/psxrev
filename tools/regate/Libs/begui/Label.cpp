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

#include "Label.h"
#include "Font.h"

using namespace begui;

Label::Label() : m_textColor(0.3f,0.3f,0.3f), m_pFont(0), m_bMultiLine(false)
{
}

void Label::create(int x, int y, const std::string& text)
{
	m_text = text;
	m_bMultiLine = false;

	m_pFont = FontManager::getCurFont();
	ASSERT(m_pFont);

	setPos(x,y);
	setSize(m_pFont->stringLength(text), m_pFont->getLineHeight());
}

void Label::createMultiline(int x, int y, int max_width, const std::string &text)
{
	m_text = text;
	m_maxWidth = max_width;
	m_bMultiLine = true;

	m_pFont = FontManager::getCurFont();
	ASSERT(m_pFont);

	// measure the display size of the string to set the label dimensions correctly
	int height = m_pFont->getLineHeight();
	if (m_text.length() > 0) {
		std::vector<Rect<int> > rects;
		m_pFont->renderStringMultiline(0, m_pFont->getLineHeight(), m_maxWidth, m_text, &rects, false); // does NOT render the string
		height = rects.back().bottom;
	}

	setPos(x,y);
	setSize(m_maxWidth, height);
}

void Label::setText(const std::string &text)
{
	m_text = text;

	if (m_bMultiLine) {
		// measure the display size of the string to set the label dimensions correctly
		int height = m_pFont->getLineHeight();
		if (m_text.length() > 0)
		{
			std::vector<Rect<int> > rects;
			m_pFont->renderStringMultiline(0, m_pFont->getLineHeight(), m_maxWidth, m_text, &rects, false); // does NOT render the string
			height = rects.back().bottom;
		}

		// update the label size
		setSize(m_maxWidth, height);
	}
}

void Label::onUpdate()
{
}

void Label::onRender()
{	
	ASSERT(m_pFont);

	// set the text color
	if (isEnabled())
		glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, 0.8f);
	else
		glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, 0.5f);

	// render the string
	if (m_bMultiLine)
		m_pFont->renderStringMultiline(0, m_pFont->getLineHeight(), m_maxWidth, m_text);
	else
		m_pFont->renderString(0, m_pFont->getLineHeight(), m_text);
}

bool Label::onMouseDown(int x, int y, int button)
{
	return false;
}

bool Label::onMouseMove(int x, int y, int prevx, int prevy)
{
	return false;
}

bool Label::onMouseUp(int x, int y, int button)
{
	return false;
}

void Label::onKeyDown(int key)
{
}

void Label::onKeyUp(int key)
{
}

bool Label::isPtInside(int x, int y)
{
	// a label is passive, does not respond to mouse movements etc.
	// so, just dont bother..
	return false;
}
