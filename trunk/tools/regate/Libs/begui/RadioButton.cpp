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

#include "RadioButton.h"
#include "Font.h"
#include "ResourceManager.h"
#include "Container.h"

using namespace begui;

RadioButton::RadioButton() : m_state(RadioButton::NOT_SELECTED), m_pCallback(0), m_bHover(false),
	m_id(-1), m_bIsRadio(true),
	m_activeArea(0,0,0,0)
{
}

void RadioButton::create(int x, int y, const std::string &title, int id, void (*callback)(int), 
						 const std::string &style_name, bool bIsRadio)
{
	m_title = title;
	m_state = RadioButton::NOT_SELECTED;
	m_pCallback = callback;
	m_id = id;
	m_bHover = false;

	// load style
	ResourceManager::Style style = ResourceManager::inst()->getClassDef("RadioButton").style(style_name);
	ASSERT(style.hasProp("face_up"));
	m_faces[SELECTED] = ResourceManager::inst()->loadImage(style.get_img("face_up"));
	ASSERT(style.hasProp("face_down"));
	m_faces[NOT_SELECTED] = ResourceManager::inst()->loadImage(style.get_img("face_down"));
	if (style.hasProp("active_area"))
		m_activeArea = style.get_rect("active_area");
	else
		m_activeArea = Rect<int>(0,0,m_faces[SELECTED].m_width, m_faces[SELECTED].m_height);

	setPos(x,y);
	m_right = x + m_faces[SELECTED].m_width/2 + Font::stringLength(title);
	m_bottom = y + m_faces[SELECTED].m_height;
	m_bIsRadio = bIsRadio;
}

void RadioButton::onUpdate()
{
}

void RadioButton::onRender()
{
	ResourceManager::ImageRef &img = m_faces[m_state];
	ASSERT(img.m_texture);
	glEnable(GL_BLEND);
	if (m_bHover)
		glColor4f(1,1,1,0.7f);
	else
		glColor4f(1,1,1,1);
	Component::drawImage(img, -m_activeArea.left, -m_activeArea.top);

	// render the text
	glColor4f(0.3f,0.3f,0.3f,1);
	if (m_state == RadioButton::INACTIVE)
		glColor4f(0.6f, 0.6f, 0.6f, 1);
	Font::renderString(m_activeArea.getWidth() + 6, FontManager::getCurFont()->getLineHeight()-1, m_title);
}

bool RadioButton::onMouseDown(int x, int y, int button)
{
	if (m_state == RadioButton::INACTIVE)
		return false;

	// Deselect all other radio buttons in the group
	if (m_bIsRadio && m_pParent)
	{
		//Container *pCont = dynamic_cast<Container*>(m_pParent);
		//if (pCont)	// if parent is a container
		// dynamic_cast does not work here...
		Container *pCont = (Container*)m_pParent;

		int nc = pCont->getChildrenNum();
		for (int i=0; i<nc; ++i)
		{
			try {
				RadioButton *pRB = dynamic_cast<RadioButton*>(pCont->getChild(i));
				if (pRB && pRB!=this)	// if its a radio button
				{
					// deselect it
					if (pRB->getState() == RadioButton::SELECTED)
						pRB->setState(RadioButton::NOT_SELECTED);
				}
			}
			catch (std::bad_cast) {
			}
		}
	}
	
	if (m_bIsRadio)
		m_state = RadioButton::SELECTED;
	else
		m_state = (m_state == RadioButton::SELECTED)?RadioButton::NOT_SELECTED:RadioButton::SELECTED;

	// Call the callback, if any
	if (m_pCallback)
		m_pCallback(m_id);
	
	return true;
}

bool RadioButton::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (isPtInside(x,y))
		m_bHover = true;
	else
		m_bHover = false;
	return true;
}

bool RadioButton::onMouseUp(int x, int y, int button)
{
	return true;
}

void RadioButton::onKeyDown(int key)
{
}

void RadioButton::onKeyUp(int key)
{
}

bool RadioButton::isPtInside(int x, int y)
{
	if (x<m_left || x>m_left+m_activeArea.getWidth())
		return false;
	if (y<m_top || y>m_top+m_activeArea.getHeight())
		return false;
	return true;
}
