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

#include "BaseTexture.h"
#include "TextureManager.h"

void BaseTexture::free()
 {
	if (m_texture && !m_bIsManaged)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
	else if (m_texture)
	{
		TextureManager::freeTexture(this);	// let the texture manager know that this one is no longer used
		m_texture = 0;
	}
	m_width = m_height = m_depth = 0;
}

void BaseTexture::setPriority(double priority)
{
	GLclampf pr = (float)priority;
	glPrioritizeTextures(1, &m_texture, &pr);
}