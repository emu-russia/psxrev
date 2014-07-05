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

#include "Group.h"
#include "Font.h"
#include "ResourceManager.h"

using namespace begui;

Group::Group() : m_frameStyle(Group::CUSTOM),
	m_frameColor(1, 1, 1),
	m_textColor(0.2f,0.2f,0.2f),
	m_resizableArea(0,0,0,0)
{
}

void Group::create(int left, int top, int width, int height, const std::string &title,
				   const std::string &style_name)
{
	m_left = left;
	m_top = top;
	m_right = m_left+width;
	m_bottom = m_top + height;
	m_title = title;

	ResourceManager::Style style = ResourceManager::inst()->getClassDef("GroupBox").style(style_name);
	ASSERT(style.hasProp("bg"));
	m_bg = ResourceManager::inst()->loadImage(style.get_img("bg"));
	if (style.hasProp("resizable_area"))
		m_resizableArea = style.get_rect("resizable_area");
	if (style.hasProp("active_area"))
		m_activeArea = style.get_rect("active_area");
	else
		m_activeArea = Rect<int>(0,0,m_bg.m_width, m_bg.m_height);
	if (style.hasProp("main_color"))
		m_frameColor = style.get_c("main_color");
	if (style.hasProp("text_color"))
		m_textColor = style.get_c("text_color");
}

void Group::onRender()
{
	glEnable(GL_BLEND);
	glColor4f(m_frameColor.r, m_frameColor.g, m_frameColor.b, 1.0f);
	Component::drawImageWtBorders(m_bg, -m_activeArea.left, -m_activeArea.top, 
		getWidth()+(m_bg.m_width - m_activeArea.right)+m_activeArea.left, 
		getHeight()+(m_bg.m_height - m_activeArea.bottom)+m_activeArea.top, m_resizableArea);

/*	// set the texture of a window
	Texture *pTex = ResourceManager::inst()->getStockMap(ResourceManager::STD_CONTROLS);
	pTex->set();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double ul=382;
	double ut=34;
	double ur=406;
	double ub=58;
	double tw=512;
	double th=512;
	int left = 0, top=0;	//NOTE: rendering is done in LOCAL coordinate system!!
	int right = m_right-m_left, bottom=m_bottom-m_top;
	glColor4f(0,0,0,0.17);
	Component::drawBorderedQuad(left, top, right, bottom,
						left+4, top+4, right-4, bottom-4,
						ul/tw, (ul+4)/tw, ur/tw, (ur-4)/tw,
						ut/th, (ut+4)/th, ub/th, (ub-4)/th);

	glBindTexture(GL_TEXTURE_2D, 0);

	
	glDisable(GL_BLEND);*/
	
	// render the text
	Font *pFont = FontManager::getCurFont();
	int center = getWidth()/2;
	glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, 0.5f);
	Font::renderString(center - Font::stringLength(m_title)/2, pFont->getLineHeight()+1, m_title);
}


bool Group::isPtInside(int x, int y)
{
	if (x<m_left || x>m_right)
		return false;
	if (y<m_top || y>m_bottom)
		return false;
	return true;
}