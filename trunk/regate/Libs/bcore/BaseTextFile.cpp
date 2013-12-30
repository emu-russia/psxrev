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

#include "BaseTextFile.h"
#include <ctype.h>
#include <stdlib.h>

BaseTextFile::BaseTextFile() : m_fp(0), m_nLines(0),
	m_bReadWholeLines(false), m_bSkipComments(true)
{
}

BaseTextFile::~BaseTextFile()
{
	close();
}

bool BaseTextFile::loadFile(const std::string &filename, bool bBin)
{
	close();

	m_nLines = 0;
	m_fp = fopen(filename.c_str(), (bBin)?"rb":"r");
	if (!m_fp)
		return false;

	return true;
}

void BaseTextFile::close()
{
	if (m_fp) {
		fclose(m_fp);
		m_fp = 0;
	}
}

bool BaseTextFile::isOpen() const
{
	return m_fp!=0;
}

bool BaseTextFile::eof() const
{
	ASSERT(m_fp);
	return feof(m_fp)!=0;
}

std::string BaseTextFile::getline()
{
	std::string str;
	char ch = fgetc(m_fp);
	while (!eof() && ch!='\n')
	{
		str += ch;
		ch = fgetc(m_fp);
	}
	return str;
}

BaseTextFile& BaseTextFile::operator >> (std::string& str)
{
	ASSERT(m_fp);
	
	unsigned char ch;

	while (true) {
		str = "";

		// read whitespace before string
		ch = fgetc(m_fp);
		while (!eof() && isspace(ch)) {
			if (ch == '\n') {
				m_nLines++;
				if (str.length() > 0)
					return *this;
			}
			ch = fgetc(m_fp);
		}
		
		// read string
		while (!eof() && !isspace(ch)) {
			// check if we found a character that indicates a word break
			bool bWordBreak = false;
			for (size_t i=0; i<m_wordBreakChars.size(); ++i)
				if (ch == m_wordBreakChars[i]) {
					bWordBreak = true;
					if (str.length() == 0)
						str += ch;
					else
						ungetc(ch, m_fp);
					break;
				}
			if (bWordBreak) break;
			
			str += ch;
			ch = fgetc(m_fp);
		}

		// check if this is a comment.
		bool bIsComment = false;
		// single-line comments
		for (size_t i=0; i<m_lineCommentPrefixes.size(); ++i)
		{
			if (strncmp(m_lineCommentPrefixes[i].c_str(), str.c_str(), m_lineCommentPrefixes[i].length()) == 0)
			{
				// this is a comment.
				bIsComment = true;

				// read out the rest of the line.
				while (!eof() && ch != '\n')
					ch = fgetc(m_fp);

				break;
			}
		}
		// TODO: block comments

		if (!bIsComment)
			break;
	}

	// skip whitespace after string until new line
	while (!eof() && ch != '\n') {
		ch = fgetc(m_fp);
		if (!isspace(ch) && !m_bReadWholeLines)
		{
			ungetc(ch, m_fp);
			break;
		}
	}
	if (ch == '\n')
		m_nLines++;

	return *this;
}

//WARNING: not thread-safe at all.
BaseTextFile& BaseTextFile::operator >> (double& n)
{
	static std::string buf;
	*this >> buf;
	n = atof(buf.c_str());
	return *this;
}

//WARNING: not thread-safe at all.
BaseTextFile& BaseTextFile::operator >> (float& n)
{
	static std::string buf;
	*this >> buf;
	n = (float)atof(buf.c_str());
	return *this;
}

//WARNING: not thread-safe at all.
BaseTextFile& BaseTextFile::operator >> (int& n)
{
	static std::string buf;
	*this >> buf;
	n = atoi(buf.c_str());
	return *this;
}

void BaseTextFile::addLineCommentDef(const std::string& commentStart)
{
	m_lineCommentPrefixes.push_back(commentStart);
}