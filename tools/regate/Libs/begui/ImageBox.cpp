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

#include "ImageBox.h"
#include "util.h"

using namespace begui;

ImageBox::ImageBox() : m_pImage(0), m_bResizeImg(false), m_bSelectable(false)
{
}

void ImageBox::create(int x, int y, int width, int height, Image *pImg, bool bResizeImg)
{
	setPos(x,y);
	setSize(width, height);
	setImage(pImg);
	m_bResizeImg = bResizeImg;
}

void ImageBox::setImage(Image *pImg)
{
	m_pImage = pImg;
	if (m_pImage)
		m_texture.create(*m_pImage, false);
	else
		m_texture.free();
	m_selLine.clear();
}

void ImageBox::onUpdate()
{
}

void ImageBox::onRender()
{
	int w = getWidth();
	int h = getHeight();

	glColor4f(0,0,0,0.5f);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f((float)w, 0);
		glVertex2f((float)w, (float)h);
		glVertex2f(0, (float)h);
	glEnd();

	// Render the image
	if (m_texture.isLoaded())
	{
		float left = 0;	// change if centering the image..
		float top = 0;

		float u = 1.0f, v = 1.0f;
		float iw = (float)w, ih=(float)h;
	/*	if (!m_bResizeImg)
		{
			iw = (w > m_pImage->getWidth()) ? m_pImage->getWidth() : w;
			ih = (h > m_pImage->getHeight()) ? m_pImage->getHeight() : h;
			u = (float)iw/m_texture.getWidth();
			v = (float)ih/m_texture.getHeight();
		}*/
		m_texture.set();
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(left, top);
			glTexCoord2f(u,0); glVertex2f(left+iw, top);
			glTexCoord2f(u,v); glVertex2f(left+iw, top+ih);
			glTexCoord2f(0,v); glVertex2f(left, top+ih);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// draw the selection here
	if (m_selLine.size() > 2 && m_bSelectable)
	{
		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
			glVertex2f(m_selLine[0].x, m_selLine[0].y);
			for (size_t i=1; i<m_selLine.size(); ++i) {
				glVertex2f(m_selLine[i].x, m_selLine[i].y);
				glVertex2f(m_selLine[i].x, m_selLine[i].y);
			}
			glVertex2f(m_selLine[0].x, m_selLine[0].y);
		glEnd();
	}

	// additional rendering, controlled by the application
	m_onRender();
}

bool ImageBox::onMouseDown(int x, int y, int button)
{
	if (button == MOUSE_BUTTON_LEFT)
	{
		if (m_pImage && m_bSelectable)
		{
			m_selLine.clear();
			
			Vector2 pt(float(x-m_left), float(y-m_top));
			if (pt.x < 0) pt.x = 0;
			if (pt.y < 0) pt.y = 0;
			if (pt.x > m_pImage->getWidth()) pt.x = (float)m_pImage->getWidth();
			if (pt.y > m_pImage->getHeight()) pt.y = (float)m_pImage->getHeight();
			m_selLine.push_back(pt);
		}

		m_onMouseDown(Vector2i(x-m_left,y-m_top));
	}

	return Component::onMouseDown(x,y,button);
}

bool ImageBox::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (input::isMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		if (m_pImage && m_bSelectable)
		{
			Vector2 pt(float(x-m_left), float(y-m_top));
			if (pt.x < 0) pt.x = 0;
			if (pt.y < 0) pt.y = 0;
			if (pt.x > m_pImage->getWidth()) pt.x = (float)m_pImage->getWidth();
			if (pt.y > m_pImage->getHeight()) pt.y = (float)m_pImage->getHeight();
			m_selLine.push_back(pt);
		}
	}

	return Component::onMouseMove(x,y,prevx,prevy);
}

bool ImageBox::onMouseUp(int x, int y, int button)
{
	if (button == MOUSE_BUTTON_LEFT)
	{
		if (m_pImage && m_bSelectable)
		{
			Vector2 pt(float(x-m_left), float(y-m_top));
			if (pt.x < 0) pt.x = 0;
			if (pt.y < 0) pt.y = 0;
			if (pt.x > m_pImage->getWidth()) pt.x = (float)m_pImage->getWidth();
			if (pt.y > m_pImage->getHeight()) pt.y = (float)m_pImage->getHeight();
			m_selLine.push_back(pt);

			if (m_selLine.size() <= 2)	// only endpoints, no mouse move in between
			{
				m_selLine.clear();
				return true;
			}
		}
		
		m_onMouseUp(Vector2i(x-m_left,y-m_top));
	}

	return Component::onMouseUp(x,y,button);
}

void ImageBox::onKeyDown(int key)
{
	m_onKeyDown(key);
}

void ImageBox::onKeyUp(int key)
{
	m_onKeyUp(key);
}

bool ImageBox::isPtInside(int x, int y)
{
	if (x<m_left || x>m_right)
		return false;
	if (y<m_top || y>m_bottom)
		return false;
	return true;
}
