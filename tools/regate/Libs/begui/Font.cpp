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
#include "util.h"
#include "../bcore/Image.h"

using namespace begui;

Font::Font() : m_tabSize(5)
{
}

Font::~Font()
{
}

void Font::renderString(int x, int y, const std::string &str,
						  std::vector< Rect<int> > *char_pos_out,
						  bool bRender,
						  int vpWidth, int vpHeight)
{
	if (FontManager::m_curFont >= 0)
	{
		Font *curFont = FontManager::getCurFont();
		ASSERT(curFont);
		curFont->renderString_i(x, y, str, char_pos_out, bRender);
	}
	else
	{
		// fall back to rendering text with GLUT

/*		// UGLY hack to cope with different viewports
		if (vpWidth == -1)
			vpWidth = display::getWidth();
		if (vpHeight == -1)
			vpHeight = display::getHeight();

		Matrix4 mat;
		mat.loadGLMatrix(GL_MODELVIEW_MATRIX);
		Vector3 t = mat.getTranslation();

		double tx = 2.0*((double)(x+t.x)/vpWidth) - 1.0;
		double ty = 2.0*((double)(y+t.y)/vpHeight) - 1.0;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glRasterPos2f(tx, -ty);
		for (size_t i=0; i<str.length(); ++i)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, str.at(i));

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();*/
	}
}

void Font::renderString_i(int x, int y, const std::string &str,
						  std::vector< Rect<int> > *char_pos_out,
						  bool bRender)
{
	// set the texture with the font
	m_texture.set();

	glEnable(GL_BLEND);

	glBegin(GL_QUADS);
	int xpos = x;
	int ypos = y;
	for (size_t i=0; i<str.length(); ++i)
	{
		// handle new line characters
		if (str[i] == '\n')
		{
			xpos = x;
			ypos += m_lineHeight;
			continue;
		}

		unsigned char cc = str[i];
		if (cc < m_startChar || cc > 255)
			continue;
		Character& charInfo = m_character[cc - m_startChar];

		// get character metrics
		int fw = charInfo.m_right-charInfo.m_left; // font width
		int fh = charInfo.m_bottom-charInfo.m_top; // font height
		float tx = (float)charInfo.m_left/m_texture.getWidth();
		float ty = (float)(charInfo.m_top)/m_texture.getHeight();
		float tw = (float)fw/m_texture.getWidth();
		float th = (float)fh/m_texture.getHeight();

		// render the character
		int left = xpos+charInfo.m_horiBearingX;
		int right = xpos+fw+charInfo.m_horiBearingX;
		int top = ypos - charInfo.m_horiBearingY;
		int bottom = ypos+fh - charInfo.m_horiBearingY;
		if (bRender) {
			glTexCoord2f(tx,ty+th);		glVertex2f((float)left, (float)top);
			glTexCoord2f(tx+tw,ty+th);	glVertex2f((float)right, (float)top);
			glTexCoord2f(tx+tw,ty);		glVertex2f((float)right, (float)bottom);
			glTexCoord2f(tx,ty);		glVertex2f((float)left, (float)bottom);
		}

		// store the position of the rendered character
		if (char_pos_out) {
			char_pos_out->push_back(Rect<int>(left, top, right, bottom));
		}
		
		// advance horizontal position
		xpos += charInfo.m_horiAdvance;
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
glEnable(GL_BLEND);
}

void Font::renderStringMultiline(int x, int y, int lineWidth, const std::string &str,
								 std::vector< Rect<int> > *char_pos_out, bool bRender)
{
	int curLinePos = 0;
	int ypos = y;
	size_t i = 0;

	while (i < str.length())
	{
		// scan the remaining string until the first non-space, measuring width
		for (; i<str.length(); ++i)
		{
			unsigned char cc = str[i];

			if (!isspace(cc))//isprint(str[i]))
				break;

			if (cc == '\n' || (cc >= m_startChar && 
				curLinePos + m_character[cc - m_startChar].m_horiAdvance > lineWidth))
			{
				if (char_pos_out)
					char_pos_out->push_back(Rect<int>(x+curLinePos, ypos, x+curLinePos, ypos));	// a zero rect indicates a line break
				ypos += m_lineHeight;
				curLinePos = 0;
				++i;
				cc = str[i];
				break;
			}
			if (cc == '\t') {
				if (char_pos_out)
					char_pos_out->push_back(Rect<int>(x+curLinePos,  
											ypos-m_lineHeight, 
											x+curLinePos+m_tabSize*m_character[' ' - m_startChar].m_horiAdvance,
											ypos));
				curLinePos += m_tabSize*m_character[' ' - m_startChar].m_horiAdvance;
			}
			else {	
				// add an empty entry for non-printable characters
				if (char_pos_out)
					char_pos_out->push_back(Rect<int>(x+curLinePos, ypos-m_lineHeight, x+curLinePos, ypos));

				if (cc - m_startChar < 0)
					continue;
				curLinePos += m_character[cc - m_startChar].m_horiAdvance;
			}
		}
		// find the next word
		std::string word = "";
		for (; i<str.length(); ++i)
		{
			unsigned char cc = str[i];
			if (isspace(cc) )//!isprint(str[i]))
				break;
			word += cc;
		}
		// get the length of the word
		int wordLen = stringLength(word);

		// if the word gets beyond the end of this line, start a new line.
		// if we are at the beginning of the line but still the word doesnt fit, render
		// it anyway on this line.
		if (curLinePos != 0 && curLinePos + wordLen > lineWidth)
		{
			curLinePos = 0;
			ypos += m_lineHeight;
		}
		// render the word
		renderString(x+curLinePos, ypos, word, char_pos_out, bRender);
		curLinePos += wordLen;
	}
}

int Font::stringLength(const std::string &str)
{
	ASSERT(FontManager::m_curFont >= 0);

	int len = 0;
	Font *curFont = FontManager::getCurFont();
	ASSERT(curFont);
	for (size_t i=0; i<str.length(); ++i)
	{
		unsigned char cc = str[i];
		if (cc < curFont->m_startChar || cc > 255)
			continue;
		Character& charInfo = curFont->m_character[cc - curFont->m_startChar];
		len += charInfo.m_horiAdvance;
	}
	return len;
}

bool Font::createFont(const std::string &font_file, int font_size)
{
	FT_Face face; // handle to face object

	// load the font
	int error = FT_New_Face( FontManager::getFTLib(), font_file.c_str(), 0, &face );
	if ( error == FT_Err_Unknown_File_Format ) {
		// the font file could be opened and read, but it appears 
		// that its font format is unsupported
		Console::print("ERROR: unsupported font file format (file : " + font_file + " )\n");
		return false;
	}
	else if ( error ) {
		// another error code means that the font file could not
		// be opened or read, or simply that it is broken...
		Console::print("ERROR: could not load font file " + font_file + "\n");
		return false;
	}
	Console::print("\t-loaded font: " + font_file + "\n");

	// set the font size
	error = FT_Set_Char_Size( face, /* handle to face object */ 
								0, /* char_width in 1/64th of points */
								font_size*64, /* char_height in 1/64th of points */
								0, /* horizontal device resolution (0 for default 72dpi) */
								0 ); /* vertical device resolution (0 for default 72dpi) */

	// create the texture with the fonts
	int texW = 512;
	int texH = 512;
	std::vector<unsigned char> pixels(texW*texH*4, 0);

	int xpos = 2;
	int ypos = 2;
	int maxH = 0;
	int maxHFont = 0;
	m_character.clear();
	m_startChar = 20;
	for (int i=m_startChar; i<=255; ++i)
	{
		unsigned char char_id = (unsigned char)i;
		
		// load glyph image into the slot (erase previous one)
		error = FT_Load_Char( face, char_id, FT_LOAD_RENDER );
		if ( error )
			continue; /* ignore errors */
		FT_GlyphSlot slot = face->glyph;

		// check where we should draw the new character
		if (xpos + slot->bitmap.width+1 > texW-1)
		{
			xpos = 2;
			ypos += maxH;
			maxH = 0;
		}
		
		// now, draw to our target surface
		int bitmapPos = 0;
		for (int dy=0; dy<slot->bitmap.rows; ++dy) {
			for (int dx=0; dx<slot->bitmap.width; ++dx)
			{
				int x = xpos + dx;
				int y = ypos + (slot->bitmap.rows - dy - 1);
				pixels[4*(y*texW + x)  ] = 255;//slot->bitmap.buffer[bitmapPos + dx];
				pixels[4*(y*texW + x)+1] = 255;//slot->bitmap.buffer[bitmapPos + dx];
				pixels[4*(y*texW + x)+2] = 255;//slot->bitmap.buffer[bitmapPos + dx];
				pixels[4*(y*texW + x)+3] = slot->bitmap.buffer[bitmapPos + dx];//0;
			}
			bitmapPos += abs(slot->bitmap.pitch);
		}
		
		Font::Character charInfo;
		charInfo.m_char = char_id;
		charInfo.m_left = xpos;
		charInfo.m_right = xpos + slot->bitmap.width;
		if (char_id == ' ')
			charInfo.m_right = xpos + int(font_size/2.6);
		charInfo.m_horiBearingX = face->glyph->metrics.horiBearingX/64;
		charInfo.m_horiBearingY = face->glyph->metrics.horiBearingY/64;
		charInfo.m_horiAdvance  = face->glyph->metrics.horiAdvance/64;
		charInfo.m_top = ypos;
		charInfo.m_bottom = ypos + slot->bitmap.rows;
		m_character.push_back(charInfo);
		
		// increment rendering position
		xpos += slot->bitmap.width + 1;
		if ((slot->bitmap.rows+1) > maxH)
			maxH = slot->bitmap.rows+1;
		if (maxH > maxHFont)
			maxHFont = maxH;
	}
	
	m_texture.create(texW, texH, GL_RGBA, &pixels[0]);

	// DEBUG
/*	Image img;
	img.create(texW, texH, 3);
	for (size_t i=0; i<texW; ++i)
		for (size_t j=0; j<texH; ++j)
			img(i,j)[0] = img(i,j)[1] = img(i,j)[2] = pixels[4*(j*texW + i) + 3];
	img.savePPM("font_debug.ppm");
	FILE *fp = fopen("font_debug.txt", "w+");
	for (size_t i=0; i<m_character.size(); ++i)
		fprintf(fp, "%d - %d %d %d %d\n", i, m_character[i].m_left, m_character[i].m_top, m_character[i].m_right, m_character[i].m_bottom);
	fclose(fp);*/

	m_lineHeight = maxHFont;

	return true;
}