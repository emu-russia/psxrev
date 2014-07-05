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

#ifndef _TEXTUREMANAGER_H45631_INCLUDED_
#define _TEXTUREMANAGER_H45631_INCLUDED_

#pragma once

#include "common.h"
#include <hash_map>
class Texture;
class Texture3D;
class CubeTexture;
class BaseTexture;

class TextureManager
{
private:
	struct texRef {
		int nRefs;
		GLuint texId;
		int w, h, d;
	};
#if (_MSC_VER >= 1310)	// if using visual studio .NET 2003 or later (yeah, thanks MS...)
	static stdext::hash_map<std::string, GLuint> m_loadedFilenames;
	typedef stdext::hash_map<std::string, GLuint>::iterator tFileRefIter;
	typedef stdext::hash_map<std::string, GLuint>::value_type tFileRefVal;
	
	static stdext::hash_map<GLuint, texRef> m_textures;
	typedef stdext::hash_map<GLuint, texRef>::iterator tTexRefIter;
	typedef stdext::hash_map<GLuint, texRef>::value_type tTexRefVal;
#else
	static std::hash_map<std::string, GLuint> m_loadedFilenames;
	typedef std::hash_map<std::string, GLuint>::iterator tFileRefIter;
	typedef std::hash_map<std::string, GLuint>::value_type tFileRefVal;
	
	static std::hash_map<GLuint, texRef> m_textures;
	typedef std::hash_map<GLuint, texRef>::iterator tTexRefIter;
	typedef std::hash_map<GLuint, texRef>::value_type tTexRefVal;
#endif

public:
	TextureManager();
	virtual ~TextureManager();

	// Every time a loadTexture* method is called, it creates a new Texture(/Texture3D/CubeTexture)
	// object. However, the underlying OpenGL texture is loaded only once for the same texture
	// file. A hash map is used to find already loaded files and avoid reloading them. Reference
	// counting is used to free the textures when they are no longer used.
	static Texture* loadTexture(const std::string& filename, bool bUnique=false);
	static Texture3D* loadTexture3D(const std::string& filename);
	static CubeTexture* loadTextureCube(const std::string& filename);

	static void freeTexture(BaseTexture*);
};

#endif