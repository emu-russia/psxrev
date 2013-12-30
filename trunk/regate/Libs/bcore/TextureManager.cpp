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

#include "TextureManager.h"
#include "Texture.h"
#include "Image.h"

#if (_MSC_VER >= 1310)
	stdext::hash_map<std::string, GLuint> TextureManager::m_loadedFilenames;
	stdext::hash_map<GLuint, TextureManager::texRef> TextureManager::m_textures;
#else
	std::hash_map<std::string, GLuint> TextureManager::m_loadedFilenames;
	std::hash_map<GLuint, TextureManager::texRef> TextureManager::m_textures;
#endif

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	// free all managed textures
	for (tTexRefIter it=m_textures.begin(); it!=m_textures.end(); ++it)
	{
		glDeleteTextures(1, &(*it).second.texId);
	}
	m_textures.clear();
	m_loadedFilenames.clear();
}

Texture* TextureManager::loadTexture(const std::string& filename, bool bUnique)
{
	Texture *pTex = 0;

	// check the hash table to see if this texture file has already been loaded
	tFileRefIter it = m_loadedFilenames.find(filename);
	if (it != m_loadedFilenames.end() && !bUnique)
	{
		// get the texture id
		GLuint texid = (*it).second;

		// get the texture reference
		tTexRefIter rit = m_textures.find(texid);
		if (rit == m_textures.end())
		{
			Console::print("WTF ERROR: TextureManager::loadTexture - texture not in hash map\n");
			return 0;
		}
		texRef &tref = (*rit).second;

		// the texture has already been loaded. Just wrap it in a new
		// texture object
		pTex = new Texture();
		pTex->m_texture = texid;
		pTex->m_width = tref.w;
		pTex->m_height = tref.h;
		pTex->m_depth = tref.d;
		pTex->m_bIsManaged = true;

		Console::print("\t-texture already exists - duplicated\n");
	}
	else
	{
		// load the image
		Image image;
		if (!image.load(filename))
			return 0;

		// create a texture object
		pTex = new Texture();

		// load the data to the texture object
		pTex->create(image);
		pTex->m_bIsManaged = true;

		// create a new texture reference
		texRef ref;
		ref.nRefs = 1;
		ref.texId = pTex->getGLTex();
		
		// add the new texture to the hashmap of managed textures
		m_textures.insert(tTexRefVal(ref.texId, ref));

		// store the texture id to the hash table
		m_loadedFilenames.insert(tFileRefVal(filename, ref.texId));
	}

	return pTex;
}

Texture3D* TextureManager::loadTexture3D(const std::string& filename)
{
	return 0;
}

CubeTexture* TextureManager::loadTextureCube(const std::string& filename)
{
	return 0;
}

void TextureManager::freeTexture(BaseTexture *pTex)
{
	GLuint texid = pTex->getGLTex();

	// find the tex ref for this texture
	tTexRefIter it = m_textures.find(texid);
	if (it != m_textures.end() && pTex->isManaged())
	{
		texRef &ref = (*it).second;

		// if there is an entry, the texture is managed, so reduce the ref counter
		ref.nRefs--;

		// if the ref counter is zero, the texture is no longer used. Delete it.
		if (ref.nRefs <= 0)
		{
			glDeleteTextures(1, &texid);

			// remove the texture from the m_loadedFilenames hash, by searching with the tex id
			// The texture might not be listed in this list
			for (tFileRefIter fit=m_loadedFilenames.begin(); fit!=m_loadedFilenames.end(); ++fit)
			{
				if ((*fit).second == texid)
				{
					m_loadedFilenames.erase(fit);
					break;
				}
			}

			// finally, remove the entry from the managed textures hash table
			m_textures.erase(it);

			Console::print("\t-texture deleted\n");
		}
	}
	else
	{
		// if there is no entry, then the texture is unmanaged, so just delete it
		glDeleteTextures(1, &texid);
	}
}
