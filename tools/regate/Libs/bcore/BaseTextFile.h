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

#ifndef _BASETEXTFILE_H_INCLUDED_
#define _BASETEXTFILE_H_INCLUDED_

#pragma once

#include "common.h"
#include <stdio.h>

class BaseTextFile
{
private:
	FILE *m_fp;
	int	 m_nLines;

	bool m_bReadWholeLines;
	bool m_bSkipComments;
	std::vector<std::string> m_lineCommentPrefixes;

	std::vector<char> m_wordBreakChars;	// characters that indicate the beginning of a new word

public:
	BaseTextFile();
	virtual ~BaseTextFile();

	virtual bool loadFile(const std::string& filename, bool bBinFile = false);
	virtual void close();
	
	bool	isOpen() const;
	bool	eof() const;
	std::string getline();
	int		getCurLine() const	{ return m_nLines+1; }
	FILE*	getFP() { return m_fp; }	// OK, architectural mistake.

	void	goToStart()		{ fseek(m_fp, 0, SEEK_SET); }
	void	goToEnd()		{ fseek(m_fp, 0, SEEK_END); }

	void	setReadWholeLines(bool bEnable)	{ m_bReadWholeLines = bEnable; }
	void	addCommentBlockDef(const std::string& commentStart, const std::string& commentEnd);
	void	addLineCommentDef(const std::string& commentStart);
	void	skipComments(bool bSkip)	{ m_bSkipComments = bSkip; };
	void	addWordBreakChar(char c)	{ m_wordBreakChars.push_back(c); }

	BaseTextFile& operator >> (std::string&);
	BaseTextFile& operator >> (double&);
	BaseTextFile& operator >> (float&);
	BaseTextFile& operator >> (int&);
};

#endif