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

#include "Font.h"

using namespace begui;

std::vector<Font> FontManager::m_fonts;
FT_Library	FontManager::m_freetype;
bool		FontManager::m_ftInitialized = false;
int			FontManager::m_curFont = -1;

bool FontManager::initialize()
{
	// initialize freetype
	int error = FT_Init_FreeType( &m_freetype );
	if ( error ) {
		Console::print("ERROR: failed to initialize FreeType\n");
		return false;
	}
	Console::print("FreeType library initialized ok\n");

	return true;
}

bool FontManager::setFont(const std::string &font_name, int font_size)
{
	m_fonts.push_back(Font());
	if (!m_fonts.back().createFont(font_name, font_size))
	{
		m_fonts.pop_back();
		return false;
	}
	m_curFont = (int)m_fonts.size()-1;
	return true;
}