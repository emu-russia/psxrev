/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of Be3D library.
//
//    Be3D is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Be3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with Be3D.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "BBox.h"

void BBox::render()
{
	glPushAttrib(GL_POLYGON_BIT | GL_ENABLE_BIT/* | GL_COLOR_BIT*/);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glColor4f(0, 0.5f, 0.2f, 0);
	glBegin(GL_QUADS);
		glVertex3f(m_min.x, m_min.y, m_min.z);
		glVertex3f(m_min.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_min.y, m_min.z);

		glVertex3f(m_min.x, m_min.y, m_max.z);
		glVertex3f(m_min.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_min.y, m_max.z);
		
		glVertex3f(m_min.x, m_min.y, m_min.z);
		glVertex3f(m_min.x, m_min.y, m_max.z);
		glVertex3f(m_max.x, m_min.y, m_max.z);
		glVertex3f(m_max.x, m_min.y, m_min.z);
		
		glVertex3f(m_min.x, m_max.y, m_min.z);
		glVertex3f(m_min.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_max.y, m_min.z);
	glEnd();

	glPopAttrib();
}