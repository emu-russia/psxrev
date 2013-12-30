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

#ifndef _FONT_H42631_INCLUDED_
#define _FONT_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "../bcore/Rect.h"

// FreeType
extern "C" {
	#include <ft2build.h>
	#include FT_FREETYPE_H
}


namespace begui {

class Font
{
private:
	class Character {
	public:
		int m_char;
		int m_left, m_right, m_top, m_bottom;
		int m_horiBearingX;
		int m_horiBearingY;
		int m_horiAdvance;
	};

	Texture				m_texture;
	std::vector<Character> m_character;
	int					m_startChar;	// the first char in the letters
	int					m_lineHeight;
	int					m_tabSize;	// size of tabs in spaces (not pixels)

public:
	Font();
	virtual ~Font();

	static void renderString(int x, int y, const std::string& str, 
							std::vector< Rect<int> > *char_pos_out = 0,
							bool bRender = true,
							int vpWidth=-1, int vpHeight=-1);
	static int	stringLength(const std::string& str);
	
	bool createFont(const std::string &font_file, int font_size);
	void renderStringMultiline(int x, int y, int lineWidth, const std::string& str,
							std::vector< Rect<int> > *char_pos_out = 0, bool bRender = true);
	void renderChar(const Rect<int> &rect, wchar_t c);

	int	getLineHeight() const	{ return m_lineHeight; }
	Character*	getChar(int c)	{ if (c<m_startChar || c>=(int)m_character.size()) return 0; return &m_character[c - m_startChar]; }

	//temp:
	Texture* getTexture() { return &m_texture; }

private:
	void renderString_i(int x, int y, const std::string& str, std::vector< Rect<int> > *char_pos_out, bool bRender);
};

class FontManager
{
	friend class Font;
private:
	static std::vector<Font> m_fonts;
	static int			m_curFont;
	static FT_Library	m_freetype;
	static bool			m_ftInitialized;

public:
	static bool initialize();
	static bool isInitialized()	{ return m_ftInitialized; }

	static bool setFont(const std::string &font_name, int font_size);
	static Font* getCurFont()	{ if (m_curFont < 0) return NULL; return &m_fonts[m_curFont]; }

protected:
	static FT_Library	getFTLib()	{ return m_freetype; }
};

};

#endif