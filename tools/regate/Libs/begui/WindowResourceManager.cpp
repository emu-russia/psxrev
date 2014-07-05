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

#include "WindowResourceManager.h"
#include "../../b3d_lib/src/BaseTextFile.h"
#include "../../b3d_lib/src/Image.h"
#include <direct.h>

#pragma warning (disable : 4996)

using namespace begui;

// The singleton instance
WindowResourceManager *WindowResourceManager::m_instance;

WindowResourceManager::WindowResourceManager()
{
}

WindowResourceManager::~WindowResourceManager()
{
	freeResources();
}

void WindowResourceManager::loadResources()
{
	freeResources();

	// load all textures
	// Initialize the window manager and load resources
	Image img, img2;
	img.load(getResourceDir() + "bg.bmp");
	m_mainWindowBg.create(img);
	img.loadWithAlpha(getResourceDir() + "widgets.bmp", getResourceDir() + "widgets_alpha.bmp");
	m_childWindowTemplate.create(img);
}

void WindowResourceManager::freeResources()
{
	// Free all allocated resources
}

void WindowResourceManager::loadPropertyFile(const std::string& fname)
{
	// Load the property file
	BaseTextFile file;
	file.addLineCommentDef("//");
	//file.addCommentBlockDef("/*", "*/");
	if (!file.loadFile(fname))
	{
		Console::print("ERROR: failed to load property file: %s\n", fname.c_str());
		return;
	}

	// Parse the file, loading resources as they are encountered and
	// also writing properties into the class/property hash maps
	std::string token;
	while (!file.eof())
	{
		file >> token;
		if (token[0] == '#')
		{
			// read the definition of a new resource class
		}
		else
		{
			// parsing error
			Console::print("ERROR: error parsing property file %s:\n\tclass name expected (should start with #)\n",
				fname.c_str());
			return;
		}
	}

	file.close();
}

std::string WindowResourceManager::getResourceDir() const
{
	if (m_resourceDir.length() == 0) {
		char currentPath[2048];
		_getcwd(currentPath, 2000);
		return std::string(strcat(currentPath, "/../../../begui/resources/"));
	}
	return m_resourceDir;
}

void WindowResourceManager::setResourceDir(const std::string &resdir)
{
	m_resourceDir = resdir;
}