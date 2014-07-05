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

#include "ResourceManager.h"
#include "../bcore/BaseTextFile.h"
#include "../bcore/Image.h"
#include <direct.h>

#pragma warning (disable : 4996)

using namespace begui;

// The singleton instance
ResourceManager *ResourceManager::m_instance;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	freeResources();
}

void ResourceManager::loadResources()
{
	freeResources();

	// load all textures
	// Initialize the window manager and load resources

	if (!loadPropertyFile(getResourceDir() + "style.txt"))
		return;
	
	Image img;
	img.load(getResourceDir() + "bg.png");
	Texture *bgtex = new Texture();
	bgtex->create(img);
	m_loadedTextures.push_back(bgtex);

	Texture *wnd_controls = new Texture();
	img.loadWithAlpha(getResourceDir() + "widgets.bmp", getResourceDir() + "widgets_alpha.bmp");
	wnd_controls->create(img);
	m_loadedTextures.push_back(wnd_controls);
}

void ResourceManager::freeResources()
{
	// Free all allocated resources
	m_images.clear();
	for (size_t i=0; i<m_loadedTextures.size(); ++i)
		SAFE_DELETE(m_loadedTextures[i]);
	m_loadedTextures.clear();
}

Texture* ResourceManager::getStockMap(ResourceManager::StockMap i)
{
	switch (i) {
		case FRAME_BACKGROUND: return m_loadedTextures[0];
		case WINDOW_RES: return m_loadedTextures[1];
		case STD_CONTROLS: return m_loadedTextures[1];
	}
	return 0;
}

ResourceManager::ImageRef ResourceManager::loadImage(const std::string &filename, bool bPack, bool bForceDuplicate)
{
	ASSERT(!(bPack && bForceDuplicate));	// sanity check

	ImageRef iref;

	// check if the image is already loaded in one of the textures
	// if bForceDuplicate is true, then we have to reload the image anyway
	if (!bForceDuplicate)
	{
		stdext::hash_map<std::string, ImageRef>::const_iterator it = m_images.find(filename);
		if (it != m_images.end())
			return it->second;
	}

	// load the texture
	Image img;
	if (!img.load(getResourceDir() + filename))	// TEMP! should check for full path/relative path, and append backslashes
		return iref;
	Texture *tex = new Texture; // TEMP! should try to pack image in other textures, if possible
	tex->create(img);
	m_loadedTextures.push_back(tex);
	
	iref.m_texture = tex;
	iref.m_topLeft = Vector2(0,0);
	iref.m_bottomRight = Vector2((float)img.getWidth()/tex->getWidth(), (float)img.getHeight()/tex->getHeight());
	iref.m_width = (int)img.getWidth();
	iref.m_height = (int)img.getHeight();

	m_images.insert(std::pair<std::string, ImageRef>(filename, iref));

	return iref;
}

ResourceManager::ImageRef ResourceManager::loadImage(const ResourceManager::ImageDesc &desc)
{
	ImageRef iref = loadImage(desc.filename, false, false);
	
	iref.m_topLeft = Vector2((float)desc.left/iref.m_width, (float)desc.top/iref.m_height);
	iref.m_bottomRight = Vector2((float)desc.right/iref.m_width, (float)desc.bottom/iref.m_height);
	iref.m_width = desc.right - desc.left;
	iref.m_height = desc.bottom - desc.top;

	return iref;
}

std::string ResourceManager::getResourceDir() const
{
	if (m_resourceDir.length() == 0) {
		char currentPath[2048];
		_getcwd(currentPath, 2000);
		return std::string(strcat(currentPath, "/../../../begui/resources/"));
	}
	return m_resourceDir;
}

void ResourceManager::setResourceDir(const std::string &resdir)
{
	m_resourceDir = resdir;
}

bool ResourceManager::loadPropertyFile(const std::string& fname)
{
	BaseTextFile file;
	if (!file.loadFile(fname))
		throw std::exception(("could not open file: " + fname).c_str());
	file.addLineCommentDef("#");
	file.skipComments(true);
	file.addWordBreakChar('{');
	file.addWordBreakChar('}');
	file.addWordBreakChar(',');
	file.addWordBreakChar('=');

	try {
		while (!file.eof()) {
			std::string token;

			file >> token;
			if (file.eof()) break;	// handle trailing whitespace in the file

			if (token == "class")
			{
				// read the class name
				std::string class_name;
				file >> class_name;

				// create the new class
				ClassDef cls;
				cls.m_name = class_name;

				file >> token;
				if (token != "{")
					throw std::exception("'{' expected");

				// read the class definition
				while (!file.eof())
				{
					file >> token;
					if (token == "}")
						break;

					if (token == "style") {
						// read a style for this class

						// read the style name
						std::string style_name;
						file >> style_name;

						file >> token;
						if (token != "{")
							throw std::exception("'{' expected");
						
						// create the new style
						Style style;
						style.m_name = style_name;

						// read the class definition
						while (!file.eof())
						{
							file >> token;
							if (token == "}")
								break;

							std::string var_type = token;
							std::string var_name;
							file >> var_name;

							file >> token;
							if (token != "=")
								throw std::exception("'=' expected");
							if (var_type == "int") {
								int var_val;
								file >> var_val;
								style.m_iVals.insert(std::pair<std::string, int>(var_name, var_val));
							}
							else if (var_type == "float") {
								double var_val;
								file >> var_val;
								style.m_fVals.insert(std::pair<std::string, double>(var_name, var_val));
							}
							else if (var_type == "string") {
								std::string var_val = file.getline();
								style.m_sVals.insert(std::pair<std::string, std::string>(var_name, var_val));
							}
							else if (var_type == "color") {
								Color cl;
								file >> cl.r;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> cl.g;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> cl.b;
								style.m_cVals.insert(std::pair<std::string, Color>(var_name, cl/255.0f));
							}
							else if (var_type == "image") {
								ImageDesc desc;
								file >> desc.left;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> desc.top;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> desc.right;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> desc.bottom;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								desc.filename = file.getline();

								style.m_imgVals.insert(std::pair<std::string, ImageDesc>(var_name, desc));
							}
							else if (var_type == "rect") {
								Rect<int> rect;
								file >> rect.left;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> rect.top;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> rect.right;
								file >> token;
								if (token != ",") throw std::exception(("expected ',', found: " + token).c_str());
								file >> rect.bottom;

								style.m_riVals.insert(std::pair<std::string, Rect<int> >(var_name, rect));
							}
							else
								throw std::exception(("unknown variable type: " + var_type).c_str());
						}

						// add the style to the class
						cls.m_styles.insert(std::pair<std::string, ResourceManager::Style>(style_name, style));
					}
				}

				// add the class definition to the list
				m_classes.insert(std::pair<std::string, ResourceManager::ClassDef>(class_name, cls));
			}
			else
				throw std::exception("'class' expected");
		}
	}
	catch (std::exception e)
	{
		char str[1024];
		sprintf(str, "parsing error (file %s, line %d): %s\n", fname.c_str(), file.getCurLine(), e.what());
		Console::error(str);
		file.close();
		return false;
	}

	file.close();

	return true;
}