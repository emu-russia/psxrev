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

#include "CheckBox.h"
#include "Font.h"
#include "ResourceManager.h"
#include "Container.h"

using namespace begui;

CheckBox::CheckBox() : m_state(false), m_bHover(false),
	m_id(-1)
{
}

CheckBox::~CheckBox()
{
}

void CheckBox::create(int x, int y, const std::string &title, int id, const Functor1<int> &callback, 
				const std::string &style_name)
{
	m_title = title;
	m_state = false;
	m_onClick = callback;
	m_id = id;
	m_bHover = false;

	// load the rest of the properties from the property manager
	ResourceManager::Style style = ResourceManager::inst()->getClassDef("CheckBox").style(style_name);
	ASSERT(style.hasProp("unchecked_face"));
	m_faceUnchecked = ResourceManager::inst()->loadImage(style.get_img("unchecked_face"));
	ASSERT(style.hasProp("checked_face"));
	m_faceChecked = ResourceManager::inst()->loadImage(style.get_img("checked_face"));
	ASSERT(style.hasProp("active_area"));
	m_activeArea = style.get_rect("active_area");

	// set position and size
	setPos(x-m_activeArea.left, y-m_activeArea.top);
	setSize(m_faceUnchecked.m_width + 4 + Font::stringLength(title), m_faceUnchecked.m_height);
}

void CheckBox::onUpdate()
{
}

void CheckBox::onRender()
{
	glEnable(GL_BLEND);

	// render the icon
	if (!isEnabled())
		glColor4f(1.0f,1.0f,1.0f,0.5f);
	else if (m_bHover)
		glColor4f(1.0f,1.0f,1.0f,0.8f);
	else
		glColor4f(1,1,1,1);
	if (m_state == true) {
		Component::drawImage(m_faceChecked, 0, 0);
	}
	else {
		Component::drawImage(m_faceUnchecked, 0, 0);
	}

	// render the text
	glColor4f(0.3f,0.3f,0.3f,1);
	if (!isEnabled())
		glColor4f(0.6f, 0.6f, 0.6f, 0.5f);
	Font::renderString(m_faceChecked.m_width+3, getHeight() - (m_faceChecked.m_height - m_activeArea.bottom)-1, m_title);
}

bool CheckBox::onMouseDown(int x, int y, int button)
{
	return true;
}

bool CheckBox::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (isPtInside(x,y))
		m_bHover = true;
	else
		m_bHover = false;
	return true;
}

bool CheckBox::onMouseUp(int x, int y, int button)
{
	if (!isEnabled())
		return false;
	m_state = !m_state;

	// Call the callback, if any
	m_onClick(m_id);

	return true;
}

void CheckBox::onKeyDown(int key)
{
}

void CheckBox::onKeyUp(int key)
{
}

