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

#include "ListBox.h"
#include "util.h"
#include "Font.h"

using namespace begui;

ListBox::ListBox() : 
	m_curItem(0), m_prevItem(-1), m_selectMode(MULTI_SELECT), m_style(STYLE_FLAT),
	m_bHighlightMouseOver(false), m_mouseOverItem(-1),
	m_textColor(0,0,0),
	m_contentPadding(0,0,0,0),
	m_scrollBarPadding(0,0,0,0)
{
}

ListBox::~ListBox()
{
}

void ListBox::create(int x, int y, int width, int height, SelectionMode selMode, Style lstyle,
					 const std::string &style_name)
{
	setPos(x,y);
	setSize(width, height);
	m_selectMode = selMode;
	m_style = lstyle;
	m_maxHeight = height;

	// load the stylesheet for the listbox
	ResourceManager::Style style = ResourceManager::inst()->getClassDef("ListBox").style(style_name);
	m_bg = ResourceManager::inst()->loadImage(style.get_img("face"));
	m_activeArea = style.get_rect("active_area");
	m_resizableArea = style.get_rect("resizable_area");
	if (style.hasProp("text_color"))
		m_textColor = style.get_c("text_color");
	if (style.hasProp("padding"))
		m_contentPadding = style.get_rect("padding");
	if (style.hasProp("scrollbar_padding"))
		m_scrollBarPadding = style.get_rect("scrollbar_padding");

	m_scroller.create(width, m_scrollBarPadding.top, 
		height-m_scrollBarPadding.top-m_scrollBarPadding.bottom, 
		ScrollBar::SCROLL_VERTICAL);
	m_scroller.setPos(width-m_scroller.getWidth()-m_scrollBarPadding.right, 
		m_scroller.getTop());
}

void ListBox::onUpdate()
{
	// update the scrollbar bounds
	int itemHeight = getItemHeight();
	int content_height = (int)m_items.size()*itemHeight;
	bool bNeedsScrolling = (content_height > getHeight());
	if (bNeedsScrolling) {
		m_scroller.setBounds(0, 
			m_items.size()-(getHeight()-(m_contentPadding.top + m_contentPadding.bottom))/itemHeight, 
			(double)(getHeight()-(m_contentPadding.top + m_contentPadding.bottom))/(content_height));
	}

	// let scrollbar update
	m_scroller.frameUpdate();
	
	// if the listbox AutoHeight option is set, adjust the height to fit
	// contents (or reach the maximum height)
	if (m_bAutoHeight) {
		int content_height = (int)m_items.size()*getItemHeight();
		int height = content_height + m_contentPadding.top + m_contentPadding.bottom +
			m_activeArea.top + (m_bg.m_height-m_activeArea.bottom);
		if (height > m_maxHeight)
			setSize(getWidth(), m_maxHeight);
		else
			setSize(getWidth(), height);
	}
}

int ListBox::getItemHeight() const
{
	Font *pFont = FontManager::getCurFont();
	ASSERT(pFont);
	int lineHeight = pFont->getLineHeight()+2;
	if (m_style == STYLE_BUTTONS)
		lineHeight = pFont->getLineHeight()+4;
	return lineHeight;
}

void ListBox::onRender()
{
	// get the offset corresponding to the position of the scrollbar
	int lineHeight = getItemHeight();
	int content_height = (int)m_items.size()*lineHeight;
	bool bNeedsScrolling = (content_height > getHeight());
	int content_y_offs = 0;
	if (bNeedsScrolling) {
		content_y_offs = -(int)(m_scroller.getScrollPos()*lineHeight);
	}

	glEnable(GL_BLEND);

	// draw the listbox background
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	Component::drawImageWtBorders(m_bg, -m_activeArea.left, -m_activeArea.top,
		getWidth()+m_activeArea.left + (m_bg.m_width-m_activeArea.right), 
		getHeight()+m_activeArea.top + (m_bg.m_height-m_activeArea.bottom), 
		m_resizableArea);
	
	Vector2i wpos = Component::localToWorld(Vector2i(0, 0));
	display::pushMask(wpos.x-1, wpos.y+m_contentPadding.top, 
		getWidth()+2-((bNeedsScrolling)? m_scroller.getWidth() : 0), 
		getHeight()-m_contentPadding.bottom-m_contentPadding.top+1);

	int w = getWidth()-((bNeedsScrolling)? m_scroller.getWidth()+1 : 0);
	int h = getHeight();

	// draw items
	for (size_t i=0; i<m_items.size(); ++i)
	{
		float top = 2 + (float)i*lineHeight;
		float bottom = top + lineHeight - 2;
		float left = 2;
		float right = (float)w - 2;

		Color textCl(0.2f,0.2f,0.2f);
		float textAlpha = 0.9f;
		if (!m_items[i].m_bEnabled)
			textAlpha = 0.2f;
		Color bgCl(1,1,1);
		float bgAlpha = 0.8f;

		switch (m_style) {
			case STYLE_FLAT:
				top = (float)i*lineHeight;
				bottom = top + lineHeight;
				left = 0;
				right = (float)w-1;
				if (m_items[i].m_bSelected)
					bgCl = Color(0.5f,0.7f,1);
				if (i == m_mouseOverItem && m_bHighlightMouseOver)
					bgCl = Color(0.7f,0.85f,1);
				break;
			case STYLE_STRIPES:
				top = (float)i*lineHeight;
				bottom = top + lineHeight;
				left = 0;
				right = (float)w-1;
				if (m_items[i].m_bEnabled && m_items[i].m_bSelected)
					bgCl = Color(0.5f,0.7f,1);
				else if (i%2 == 0)
					bgCl = Color(1,1,1);
				else
					bgCl = Color(0.83f, 0.90f, 0.95f);
				bgAlpha = 0.5f;
				break;
			case STYLE_BUTTONS:
				if (!m_items[i].m_bEnabled) {
					bgCl = Color(1,1,1);
					bgAlpha = 0.2f;
				}
				else if (m_items[i].m_bSelected)
					bgCl = Color(1,0.7f,0.2f);
				else
					bgCl = Color(0.5f,0.7f,1);
				break;
		}

		// add the offset from scrolling
		top += content_y_offs + m_contentPadding.top;
		bottom += content_y_offs + m_contentPadding.top;
		left += m_contentPadding.left;
		right -= m_contentPadding.right;

		m_items[i].m_rect = Rect<int>((int)left, (int)top, (int)right, (int)bottom);

		// do some culling of invisible items to reduce rendering needs
		if (bottom < 0)
			continue;
		if (top > h)
			continue;
		
		// draw a rectange as item background
		glColor4f(bgCl.r, bgCl.g, bgCl.b, bgAlpha);
		if (m_style != STYLE_FLAT ||
			(m_items[i].m_bEnabled && m_items[i].m_bSelected) ||
			(i==m_mouseOverItem && m_bHighlightMouseOver)) {
			glBegin(GL_QUADS);
				glVertex2f(left, top);
				glVertex2f(right, top);
				glVertex2f(right, bottom);
				glVertex2f(left, bottom);
			glEnd();
		}
		
		// draw a line frame for the item
		if (m_style == STYLE_BUTTONS) {
			glColor4f(0,0,0,0.5f);
			glBegin(GL_LINES);
				glVertex2f(left, top);
				glVertex2f(right, top);
				glVertex2f(right, top);
				glVertex2f(right, bottom);
				glVertex2f(right, bottom);
				glVertex2f(left, bottom);
				glVertex2f(left, bottom);
				glVertex2f(left, top);
			glEnd();
		}
		else if (m_style == STYLE_FLAT && i!=m_items.size()-1) {
			// draw lines separating the items
			glColor4f(0.4f,0.4f,0.4f,0.3f);
			glBegin(GL_LINES);
				glVertex2f(right, bottom);
				glVertex2f(left, bottom);
			glEnd();
		}

		// if this is the current item, draw a line frame to indicate that
		if (i == m_curItem) {
			glColor4f(0.1f, 0.1f, 0.1f, 0.3f);
			glBegin(GL_LINES);
				glVertex2f(left+2, top+2);
				glVertex2f(right-2, top+2);
				glVertex2f(right-2, top+2);
				glVertex2f(right-2, bottom-2);
				glVertex2f(right-2, bottom-2);
				glVertex2f(left+2, bottom-2);
				glVertex2f(left+2, bottom-2);
				glVertex2f(left+2, top+2);
			glEnd();
		}

		glColor4f(textCl.r, textCl.g, textCl.b, textAlpha);
		Font::renderString((int)left + 2, (int)bottom-3, m_items[i].m_text.getText());
	}
	
	display::popMask();

	if (bNeedsScrolling) {
		glColor4f(1,1,1,1);
		m_scroller.frameRender();
	}
}

bool ListBox::onMouseDown(int _x, int _y, int button)
{
	int x = _x-m_left;
	int y = _y-m_top;

	if (m_scroller.isPtInside(x, y)) {
		m_scroller.onMouseDown(x, y, button);
		return true;
	}

	if (button == MOUSE_BUTTON_LEFT) {
		for (size_t i=0; i<m_items.size(); ++i)
		{
			if (m_items[i].m_bEnabled && m_items[i].m_rect.contains(x,y))
			{
				// item i clicked

				if (m_selectMode == MULTI_SELECT_SINGLECLICK) {
					m_items[i].m_bSelected = !m_items[i].m_bSelected;
					m_prevItem = (int)i;
				}
				else if (m_selectMode == SINGLE_SELECT)
				{
					deselectAll();
					m_items[i].m_bSelected = true;
					m_prevItem = (int)i;
				}
				else if (m_selectMode == MULTI_SELECT)
				{
					if (input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)) {
						m_items[i].m_bSelected = !m_items[i].m_bSelected;
						m_prevItem = (int)i;
					}
					else if (input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT))
					{
						if (m_prevItem == -1)
							m_prevItem = (int)i;
						selectRange(m_prevItem, i);
					}
					else {
						deselectAll();
						if (m_items[i].m_bEnabled)
							m_items[i].m_bSelected = true;
						m_prevItem = (int)i;
					}
				}

				m_curItem = (int)i;

				// call the event handler for item click
				m_onItemSelect((int)i);
				
				return true;
			}
		}
	}
	return true;
}

bool ListBox::onMouseMove(int _x, int _y, int _prevx, int _prevy)
{
	int x = _x-m_left;
	int y = _y-m_top;
	int prevx = _prevx-m_left;
	int prevy = _prevy-m_top;

	// update the scrollbar
	m_scroller.onMouseMove(x, y, prevx, prevy);
	if (m_scroller.hasMouseFocus())
		return true;
	
	// find the item under the mouse cursor
	if (x>=0 && x<getWidth() && y>=0 && y<getHeight()) {
		m_mouseOverItem = -1;
		for (size_t i=0; i<m_items.size(); ++i)
		{
			if (m_items[i].m_bEnabled && m_items[i].m_rect.contains(x,y))
			{
				// item i clicked
				m_mouseOverItem = (int)i;
				break;
			}
		}
	}
	return true;
}

bool ListBox::onMouseUp(int x, int y, int button)
{
	x -= m_left;
	y -= m_top;

	m_scroller.onMouseUp(x, y, button);
	
	return false;
}

void ListBox::onKeyDown(int key)
{
	switch (key) {
		case KEY_UP:
			if (m_curItem > 0) {
				m_curItem--;
				if (input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)) {
					if (m_prevItem == -1) m_prevItem = m_curItem;
					selectRange(m_prevItem, m_curItem);
					m_onItemSelect(m_curItem);
				}
				else {
					if (m_selectMode != MULTI_SELECT_SINGLECLICK &&
						!(input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)))
						deselectAll();
					m_prevItem = m_curItem;
				}
			}
			break;
		case KEY_DOWN:
			if (m_curItem < (int)m_items.size()-1)
				m_curItem++;
			if (input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)) {
				if (m_prevItem == -1) m_prevItem = m_curItem;
				selectRange(m_prevItem, m_curItem);
			}
			else {
				if (m_selectMode != MULTI_SELECT_SINGLECLICK &&
					!(input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)))
					deselectAll();
				m_prevItem = m_curItem;
			}
			break;
		case KEY_HOME:
			m_curItem = 0;
			if (input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)) {
				if (m_prevItem == -1) m_prevItem = m_curItem;
				selectRange(m_prevItem, m_curItem);
			}
			else {
				if (m_selectMode != MULTI_SELECT_SINGLECLICK &&
					!(input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)))
					deselectAll();
				m_prevItem = m_curItem;
			}
			break;
		case KEY_END:
			if (m_items.size() > 0) {
				m_curItem = (int)m_items.size()-1;
				if (input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)) {
					if (m_prevItem == -1) m_prevItem = m_curItem;
					selectRange(m_prevItem, m_curItem);
				}
				else {
					if (m_selectMode != MULTI_SELECT_SINGLECLICK &&
						!(input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)))
						deselectAll();
					m_prevItem = m_curItem;
				}
			}
			break;
		case KEY_PAGEUP:
			{
				m_curItem -= (int)m_items.size()-(getHeight()-(m_contentPadding.top + m_contentPadding.bottom))/getItemHeight();
				if (m_curItem < 0)
					m_curItem = 0;
				break;
			}
		case KEY_PAGEDOWN:
			{
				m_curItem += (int)m_items.size()-(getHeight()-(m_contentPadding.top + m_contentPadding.bottom))/getItemHeight();
				if (m_curItem >= (int)m_items.size())
					m_curItem = (int)m_items.size()-1;
				break;
			}
		case ' ':
			switch (m_selectMode) {
			case SINGLE_SELECT:
				for (size_t j=0; j<m_items.size(); ++j)
					m_items[j].m_bSelected = false;
				m_items[m_curItem].m_bSelected = true;
				m_prevItem = m_curItem;
				m_onItemSelect(m_curItem);
				break;
			case MULTI_SELECT:
				if (input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)) {
					m_items[m_curItem].m_bSelected = !m_items[m_curItem].m_bSelected;
					m_prevItem = m_curItem;
				}
				else {
					if (m_selectMode != MULTI_SELECT_SINGLECLICK &&
						!(input::isKeyDown(KEY_LSHIFT) || input::isKeyDown(KEY_RSHIFT)))
						deselectAll();
					m_items[m_curItem].m_bSelected = true;
					m_prevItem = m_curItem;
				}
				m_onItemSelect(m_curItem);
				break;
			case MULTI_SELECT_SINGLECLICK:
				m_items[m_curItem].m_bSelected = !m_items[m_curItem].m_bSelected;
				m_prevItem = m_curItem;
				m_onItemSelect(m_curItem);
				break;
			}
			break;
		case 'A':
		case 'a':	// Ctrl+A
			if (m_selectMode != SINGLE_SELECT && (input::isKeyDown(KEY_LCTRL) || input::isKeyDown(KEY_RCTRL)))
			{			
				for (size_t j=0; j<m_items.size(); ++j) {
					if (!m_items[j].m_bEnabled) continue;
					m_items[j].m_bSelected = true;
				}
				m_onItemSelect(0);
			}
			break;
	}
	
	// make the currently active item visible
	makeItemVisible(m_curItem);

	Component::onKeyDown(key);
}

void ListBox::onKeyUp(int key)
{
	Component::onKeyUp(key);
}

void ListBox::selectRange(size_t start, size_t end)
{
	if (start > end)
		std::swap(start, end);
	
	for (size_t j=0; j<m_items.size(); ++j)
	{
		// only select enabled items
		if (!m_items[j].m_bEnabled) continue;

		// enable items in the selection range
		if (j<start || j>end)
			m_items[j].m_bSelected = false;
		else
			m_items[j].m_bSelected = true;
	}
}

void ListBox::deselectAll()
{
	for (size_t j=0; j<m_items.size(); ++j)
	{
		// only select enabled items
		if (!m_items[j].m_bEnabled) continue;

		m_items[j].m_bSelected = false;
	}
}

void ListBox::makeItemVisible(int item)
{
	// sanity check
	if (item < 0 || item >= (int)m_items.size())
		return;

	// get the position of the current item
	int itemHeight = getItemHeight();
	int curPosTop = itemHeight*item;
	int curPosBottom = itemHeight*(item+1);

	// get the visible area
	double nItems = m_items.size()-(getHeight()-(m_contentPadding.top + m_contentPadding.bottom))/itemHeight;
	double curScrollPos = (m_scroller.getScrollPos() - m_scroller.getMinPos())/(m_scroller.getMaxPos() - m_scroller.getMinPos());
	int topVisible = (int)(itemHeight * curScrollPos * nItems);
	int bottomVisible = topVisible + getHeight()-(m_contentPadding.top + m_contentPadding.bottom);

	// set the scrollbar position
	if (curPosTop < topVisible)
		m_scroller.setScrollPos(curPosTop / itemHeight);
	else if (curPosBottom >= bottomVisible) {
		double a = nItems;
		double b = (double)(curPosBottom)/ itemHeight;
		m_scroller.setScrollPos(b - a);
	}
}