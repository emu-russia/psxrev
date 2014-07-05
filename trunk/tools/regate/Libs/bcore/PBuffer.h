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

#ifndef _PBUFFER_H45631_INCLUDED_
#define _PBUFFER_H45631_INCLUDED_

#pragma once

#include "common.h"

class PBuffer
{
private:
	HPBUFFERARB	m_hpbuffer;      // Handle to a pbuffer.
    HDC			m_hdc;           // Handle to a device context.
    HGLRC		m_hglrc;         // Handle to a GL rendering context.
    int			m_width;         // Width of the pbuffer
    int			m_height;        // Height of the pbuffer
	bool		m_bShareContext;
	bool		m_bShareObjects;

public:
	PBuffer();
	virtual ~PBuffer();

	bool create(int w, int h, int pixel_format, bool bShareContext=false, bool bShareObjects=true);
	void free();

	HPBUFFERARB	getHPBuffer() const	{ return m_hpbuffer; }
	HDC			getDC()	const		{ return m_hdc; }
	HGLRC		getRenderingContext() const	{ return m_hglrc; }
	int			getWidth() const	{ return m_width; }
	int			getHeight() const	{ return m_height; }

	void makeCurrent();
};

#endif