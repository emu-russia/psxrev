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

#include "RegionSelectGizmo.h"
#include "Font.h"

using namespace begui;

RegionSelectGizmo::RegionSelectGizmo() :
	m_minX(0), m_maxX(0),
	m_minY(0), m_maxY(0),
	m_bClickable(true),
	m_pCallback(0),
	m_labelW(0),
	m_labelH(0),
	m_id(-1),
	m_hoverId(-1),
	m_bDragging(false),
	m_limMinX(0),
	m_limMaxX(0),
	m_limMinY(0),
	m_limMaxY(0)
{
}

void RegionSelectGizmo::create(int minX, int minY, int maxX, int maxY, const std::string& label,
							   int id, void (*callback)(int))
{
	m_minX = minX;
	m_maxX = maxX;
	m_minY = minY;
	m_maxY = maxY;
	m_label = label;
	m_pCallback = callback;
	m_id = id;

	if (m_label.length() > 0) {
		m_labelW = Font::stringLength(m_label) + 10;
		m_labelH = 15;
	}
	else
		m_labelW = m_labelH = 0;
}

void RegionSelectGizmo::setLimits(int minX, int minY, int maxX, int maxY)
{
	ASSERT(minX <= maxX);
	ASSERT(minY <= maxY);

	m_limMinX = minX;
	m_limMaxX = maxX;
	m_limMinY = minY;
	m_limMaxY = maxY;
}

void RegionSelectGizmo::onUpdate()
{
	// preserve limits
	if (m_limMinX != m_limMaxX)
	{
		if (m_minX < m_limMinX)
			m_minX = m_limMinX;
		if (m_maxX > m_limMaxX)
			m_maxX = m_limMaxX;
	}
	if (m_limMinY != m_limMaxY)
	{
		if (m_minY < m_limMinY)
			m_minY = m_limMinY;
		if (m_maxY > m_limMaxY)
			m_maxY = m_limMaxY;
	}
}

void RegionSelectGizmo::onRender()
{
	glEnable(GL_BLEND);

	int cx = (m_minX + m_maxX)/2;
	int cy = (m_minY + m_maxY)/2;

	// draw a quad of lines around the gizmo
	glColor4f(0,0,0, 0.7);
	glBegin(GL_LINES);
		glVertex3f(m_minX, m_minY, 0);
		glVertex3f(m_maxX, m_minY, 0);

		glVertex3f(m_maxX, m_minY, 0);
		glVertex3f(m_maxX, m_maxY, 0);
		
		glVertex3f(m_maxX, m_maxY, 0);
		glVertex3f(m_minX, m_maxY, 0);
		
		glVertex3f(m_minX, m_maxY, 0);
		glVertex3f(m_minX, m_minY, 0);
	glEnd();

	// draw 8 control points
	int ctrlSz = 4;
	glBegin(GL_QUADS);
		ctrlSz++;
		glColor4f(1,1,1, 0.7);
		glVertex3f(cx - ctrlSz, m_minY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_minY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_minY+ctrlSz, 0);
		glVertex3f(cx - ctrlSz, m_minY+ctrlSz, 0);
		glColor4f(0,0,0, 0.8);
		ctrlSz--;
		glVertex3f(cx - ctrlSz, m_minY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_minY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_minY+ctrlSz, 0);
		glVertex3f(cx - ctrlSz, m_minY+ctrlSz, 0);
		
		ctrlSz++;
		glColor4f(1,1,1, 0.7);
		glVertex3f(cx - ctrlSz, m_maxY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_maxY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_maxY+ctrlSz, 0);
		glVertex3f(cx - ctrlSz, m_maxY+ctrlSz, 0);
		ctrlSz--;
		glColor4f(0,0,0, 0.8);
		glVertex3f(cx - ctrlSz, m_maxY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_maxY-ctrlSz, 0);
		glVertex3f(cx + ctrlSz, m_maxY+ctrlSz, 0);
		glVertex3f(cx - ctrlSz, m_maxY+ctrlSz, 0);
		
		ctrlSz++;
		glColor4f(1,1,1, 0.7);
		glVertex3f(m_minX - ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_minX + ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_minX + ctrlSz, cy + ctrlSz, 0);
		glVertex3f(m_minX - ctrlSz, cy + ctrlSz, 0);
		ctrlSz--;
		glColor4f(0,0,0, 0.8);
		glVertex3f(m_minX - ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_minX + ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_minX + ctrlSz, cy + ctrlSz, 0);
		glVertex3f(m_minX - ctrlSz, cy + ctrlSz, 0);
		
		ctrlSz++;
		glColor4f(1,1,1, 0.7);
		glVertex3f(m_maxX - ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_maxX + ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_maxX + ctrlSz, cy + ctrlSz, 0);
		glVertex3f(m_maxX - ctrlSz, cy + ctrlSz, 0);
		ctrlSz--;
		glColor4f(0,0,0, 0.8);
		glVertex3f(m_maxX - ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_maxX + ctrlSz, cy - ctrlSz, 0);
		glVertex3f(m_maxX + ctrlSz, cy + ctrlSz, 0);
		glVertex3f(m_maxX - ctrlSz, cy + ctrlSz, 0);
	glEnd();

	// draw the center
	if (m_labelW > 0) {
		if (m_bClickable && m_hoverId == 0)
			glColor4f(1,1,0.85, 0.9);
		else
			glColor4f(1, 1, 0.9, 0.5);
		glBegin(GL_QUADS);
			glVertex3f(cx - m_labelW, cy - m_labelH, 0);
			glVertex3f(cx + m_labelW, cy - m_labelH, 0);
			glVertex3f(cx + m_labelW, cy + m_labelH, 0);
			glVertex3f(cx - m_labelW, cy + m_labelH, 0);
		glEnd();
		
		if (m_bClickable && m_hoverId == 0)
			glColor4f(0,0,0, 0.7);
		else
			glColor4f(0,0,0, 0.5);
		glBegin(GL_LINES);
			glVertex3f(cx - m_labelW, cy - m_labelH, 0);
			glVertex3f(cx + m_labelW+1, cy - m_labelH, 0);

			glVertex3f(cx + m_labelW, cy - m_labelH, 0);
			glVertex3f(cx + m_labelW, cy + m_labelH, 0);
			
			glVertex3f(cx + m_labelW, cy + m_labelH, 0);
			glVertex3f(cx - m_labelW, cy + m_labelH, 0);
			
			glVertex3f(cx - m_labelW, cy + m_labelH, 0);
			glVertex3f(cx - m_labelW, cy - m_labelH, 0);
		glEnd();

		// and the text
		if (m_bClickable && m_hoverId == 0)
			glColor4f(0,0,0, 0.9);
		else
			glColor4f(0,0,0, 0.5);
		Font::renderString(cx - m_labelW/2+5, cy+3, m_label);
	}

	glDisable(GL_BLEND);
}

bool RegionSelectGizmo::onMouseDown(int x, int y, int button)
{
	if (m_hoverId > 0)
		m_bDragging = true;
	else if (m_hoverId == 0)
	{
		if (m_bClickable && m_pCallback)
			m_pCallback(m_id);
	}
	return true;
}

bool RegionSelectGizmo::onMouseMove(int x, int y, int prevx, int prevy)
{
	int cx = (m_minX + m_maxX)/2;
	int cy = (m_minY + m_maxY)/2;
	int ctrlSz = 4;

	if (!m_bDragging)
	{
		if (x >= m_minX-ctrlSz && x <= m_minX+ctrlSz &&
			y >=cy-ctrlSz && y <= cy+ctrlSz)
		{
			m_hoverId = 1;
		}
		else if (x >= cx-ctrlSz && x <= cx+ctrlSz &&
			y >=m_minY-ctrlSz && y <= m_minY+ctrlSz)
		{
			m_hoverId = 2;
		}
		else if (x >= m_maxX-ctrlSz && x <= m_maxX+ctrlSz &&
			y >=cy-ctrlSz && y <= cy+ctrlSz)
		{
			m_hoverId = 3;
		}
		else if (x >= cx-ctrlSz && x <= cx+ctrlSz &&
			y >=m_maxY-ctrlSz && y <= m_maxY+ctrlSz)
		{
			m_hoverId = 4;
		}
		else if (x >= cx-m_labelW && x <= cx+m_labelW &&
			y >= cy-m_labelH && y <= cy+m_labelH)
		{
			m_hoverId = 0;
		}
		else
			m_hoverId = -1;
	}
	else
	{
		// drag the appropriate slider
		switch (m_hoverId)
		{
		case 1: if (x<m_maxX-2) m_minX = x; break;
		case 2: if (y<m_maxY-2) m_minY = y; break;
		case 3: if (x>m_minX+2) m_maxX = x; break;
		case 4: if (y>m_minY+2) m_maxY = y; break;
		}

		// preserve limits
		if (m_limMinX != m_limMaxX)
		{
			if (m_minX < m_limMinX)
				m_minX = m_limMinX;
			if (m_maxX > m_limMaxX)
				m_maxX = m_limMaxX;
		}
		if (m_limMinY != m_limMaxY)
		{
			if (m_minY < m_limMinY)
				m_minY = m_limMinY;
			if (m_maxY > m_limMaxY)
				m_maxY = m_limMaxY;
		}
	}

	return true;
}

bool RegionSelectGizmo::onMouseUp(int x, int y, int button)
{
	m_bDragging = false;
	return true;
}

void RegionSelectGizmo::onKeyDown(int key)
{
	Component::onKeyDown(key);
}

void RegionSelectGizmo::onKeyUp(int key)
{
	Component::onKeyUp(key);
}

bool RegionSelectGizmo::isPtInside(int x, int y)
{
	int cx = (m_minX + m_maxX)/2;
	int cy = (m_minY + m_maxY)/2;
	int ctrlSz = 4;

	if (x >= cx-m_labelW && x <= cx+m_labelW &&
		y >= cy-m_labelH && y <= cy+m_labelH)
	{
		return true;
	}
	else if (x >= m_minX-ctrlSz && x <= m_minX+ctrlSz &&
		y >=cy-ctrlSz && y <= cy+ctrlSz)
	{
		return true;
	}
	else if (x >= cx-ctrlSz && x <= cx+ctrlSz &&
		y >=m_minY-ctrlSz && y <= m_minY+ctrlSz)
	{
		return true;
	}
	else if (x >= m_maxX-ctrlSz && x <= m_maxX+ctrlSz &&
		y >=cy-ctrlSz && y <= cy+ctrlSz)
	{
		return true;
	}
	else if (x >= cx-ctrlSz && x <= cx+ctrlSz &&
		y >=m_maxY-ctrlSz && y <= m_maxY+ctrlSz)
	{
		return true;
	}
	return false;
}
