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

#ifndef _BASETEXTURE_H23487_INCLUDED_
#define _BASETEXTURE_H23487_INCLUDED_

#pragma once

#include "common.h"

/** BaseTexture:
	This class is meant to provide a common base for all texture
	types, and help hide the integration of textures with the texture
	manager. It implements a create(..) and a free() method, which
	use the texture manager to create a new texture or free it.

	create(..) is used to create textures skipping the TextureManager::load*
		methods. It should be used to create textures that will be filled in
		by the program, or textures that should not be managed by the texture
		manager.

	free() is used to free up the resources allocated for a texture. It
		in fact reduces by one the ref counter for this texture file in the
		texture manager. If the ref counter reaches zero, the texture manager
		will free the allocated resources.
*/
class BaseTexture
{
	friend class TextureManager;
protected:
	GLuint	m_texture;
	int		m_width, m_height, m_depth;
	bool	m_bIsManaged;

private:
	double	m_priority;

public:
	BaseTexture() : m_texture(0), m_width(0), m_height(0), m_depth(0), m_bIsManaged(false), m_priority(1.0) { };
	BaseTexture(const BaseTexture& t) : m_texture(t.m_texture), m_width(t.m_width), m_height(t.m_height), m_depth(t.m_depth), m_bIsManaged(false), m_priority(1.0) { }
	BaseTexture(GLuint texid, int w, int h=0, int d=0) : m_texture(texid), m_width(w), m_height(h), m_depth(d), m_bIsManaged(false), m_priority(1.0) { }
	virtual ~BaseTexture() { free(); }

	virtual void free();

	void setPriority(double priority);	// set a priority from 0 to 1, to keep this texture in vidmem

	inline bool isLoaded() const	{ return m_texture != 0; }
	inline bool isManaged() const	{ return m_bIsManaged; }
	inline GLuint	getGLTex()		{ return m_texture; }
};

#endif