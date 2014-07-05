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

#include "Roller.h"
#include "Font.h"
#include "ResourceManager.h"

using namespace begui;

Roller::Roller() : m_pCallback(0),
	m_id(-1),
	m_min(0),
	m_max(1),
	m_curValue(0),
	m_nSteps(10),
	m_bShowSteps(true),
	m_bShowValue(true),
	m_bDragging(false),
	m_pBoundValue(0),
	m_bDispPercentage(false),
	m_valuePrintFormat("%.2f")
{
}

void Roller::create(int x, int y, int width, int id, void (*callback)(int id))
{
	m_pCallback = callback;
	m_id = id;

	m_left = x;
	m_right = x + width;
	m_top = y;
	m_bottom = y+13;
}

void Roller::setBounds(double min, double max, double initVal, int nSteps)
{
	ASSERT(max > min);
	m_min = min;
	m_max = max;
	m_curValue = initVal;
	m_nSteps = nSteps;
}

void Roller::bindValue(double *val)
{
	m_pBoundValue = val;
}

void Roller::onUpdate()
{
	if (m_pBoundValue && !m_bDragging)
		m_curValue = *m_pBoundValue;
}

void Roller::onRender()
{
	int w = getWidth();
	int h = getHeight();

	// position of the marker
	double f = (m_curValue - m_min)/(m_max - m_min);
	// sanitize
	if (f > 1) f = 1;
	if (f < 0) f = 0;
	int spos = f*w;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// render the slider
	if (!m_bActive)
		glColor4f(1,1,1, 0.5);
	else
		glColor4f(1,1,1,1);
	
	int l = (int)(w*0.3);
	int r = (int)(w*0.7);

	glBegin(GL_QUADS);
		glColor3f(0.35, 0.35, 0.37);	glVertex3f(0, 0, 0);
		glColor3f(1, 1,1);				glVertex3f(l, 0, 0);
		glColor3f(1, 1,1);				glVertex3f(l, h, 0);
		glColor3f(0.35, 0.35, 0.37);	glVertex3f(0, h, 0);
	
		glColor3f(1, 1, 1);				glVertex3f(l, 0, 0);
		glColor3f(0.78, 0.78, 0.74);	glVertex3f(r, 0, 0);
		glColor3f(0.78, 0.78, 0.74);	glVertex3f(r, h, 0);
		glColor3f(1, 1, 1);				glVertex3f(l, h, 0);
	
		glColor3f(0.78, 0.78, 0.74);	glVertex3f(r, 0, 0);
		glColor3f(0.3, 0.3, 0.3);		glVertex3f(w, 0, 0);
		glColor3f(0.3, 0.3, 0.3);		glVertex3f(w, h, 0);
		glColor3f(0.78, 0.78, 0.74);	glVertex3f(r, h, 0);
	glEnd();

	glColor4f(0.3, 0.3, 0.3, 0.6);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(w, 0, 0);
		glVertex3f(w, 0, 0);
		glVertex3f(w, h, 0);
		glVertex3f(w, h, 0);
		glVertex3f(0, h, 0);
		glVertex3f(0, h, 0);
		glVertex3f(0, 0, 0);
	glEnd();

	// if using steps, draw them
	if (m_nSteps > 0 && m_bShowSteps)
	{
		glColor4f(0.3, 0.3, 0.3, 0.2);
		glBegin(GL_LINES);
		int steps = m_nSteps;
		if (steps > 50)
			steps = 50;
		for (int i=0; i<steps; ++i)
		{
			int lx = i*w/steps;
			int offs = spos % (w/steps);
			lx += offs;
			glVertex3f(lx, 1, 0);
			glVertex3f(lx, h-2, 0);
		}
		glEnd();
	}
	
	// render the min/max values
	glColor4f(0.3, 0.3, 0.3, 0.8);
	char valStr[64];
	if (m_bDispPercentage)
	{
		sprintf(valStr, m_valuePrintFormat.c_str(), m_min*100);
		Font::renderString(5, h-3, valStr);
		sprintf(valStr, m_valuePrintFormat.c_str(), m_max*100);
		Font::renderString(w - Font::stringLength(valStr)-5, h-3, valStr);
	}
	else
	{
		sprintf(valStr, m_valuePrintFormat.c_str(), m_min);
		Font::renderString(5, h-3, valStr);
		sprintf(valStr, m_valuePrintFormat.c_str(), m_max);
		Font::renderString(w - Font::stringLength(valStr)-5, h-3, valStr);
	}
	
	// render the current value next to the slider
	if (m_bShowValue)
	{
		glColor4f(0.3, 0.3, 0.3, 0.8);
		sprintf(valStr, m_valuePrintFormat.c_str(), m_curValue);
		Font::renderString(w+5, h-3, valStr);
	}
}

bool Roller::onMouseDown(int x, int y, int button)
{
	// start dragging the marker
	m_bDragging = true;
	return true;
}

bool Roller::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (m_bDragging)
	{
		float f = (float)(x-prevx)/(m_right - m_left);
		m_curValue += f*(m_max - m_min);

		if (m_curValue < m_min)
			m_curValue += (m_max - m_min);
		if (m_curValue > m_max)
			m_curValue -= (m_max - m_min);
			
		if (m_pBoundValue)
			*m_pBoundValue = m_curValue;
		
		return true;
	}
	return false;
}

bool Roller::onMouseUp(int x, int y, int button)
{
	m_bDragging = false;

	if (m_pCallback && m_bActive)
		m_pCallback(m_id);
	if (m_pBoundValue)
		*m_pBoundValue = m_curValue;

	return true;
}

void Roller::onKeyDown(int key)
{
}

void Roller::onKeyUp(int key)
{
}

bool Roller::isPtInside(int x, int y)
{
	// check marker region
	double f = (m_curValue - m_min)/(m_max - m_min);
	int spos = (1-f)*m_left + f*m_right;
	int t = m_top - 12;
	if (x > spos-4 && x<spos+5 && y>t && y<t+18)
		return true;

	// check the bar
	if (x<m_left || x>m_right)
		return false;
	if (y<m_top || y>m_bottom)
		return false;
	return true;
}
