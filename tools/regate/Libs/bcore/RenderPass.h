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

#ifndef _RENDERPASS_H45631_INCLUDED_
#define _RENDERPASS_H45631_INCLUDED_

#pragma once

#include "common.h"
#include "PBuffer.h"
#include "Texture.h"

class RenderPass
{
public:
	enum PixelFormat {
		PIXEL_RGBA8 = 0,
		PIXEL_RGBA16F,
		PIXEL_RGBA32F,
		PIXEL_FORMATS_NUM
	};

private:
	Texture		*m_pFrameTexture;
	Texture		*m_pDepthTex;
	bool		m_bOwnsTexture;
	int			m_width;
	int			m_height;
	PixelFormat	m_format;
	bool		m_bGenMipmaps;

	bool		m_bUsePBuffer;
	
	GLuint		m_FBO;			// frame buffer object for rendering
	GLuint		m_FBODepthBuffer;

	PBuffer		m_pbuffer;	// pbuffer for rendering
	HDC			m_last_hdc;
	HGLRC		m_last_hglrc;

public:
	RenderPass();
	~RenderPass();

	void setMipmapAutoGen(bool bGenMipmaps)		{ m_bGenMipmaps; }	// should be called BEFORE creating the renderpass!!!
	bool setup(PixelFormat pixelFormat, int frameW, int frameH, Texture *pTarget = 0, bool bDepthBuffer=false);
	void free();
	void beginPass();
	void endPass();
	
	Texture*	getFrameData()			{ return m_pFrameTexture; }
	Texture*	getDepthData()			{ return m_pDepthTex; }
	PixelFormat	getPixelFormat() const	{ return m_format; }
	int			getWidth() const		{ return m_width; }
	int			getHeight() const		{ return m_height; }
	bool		isGenMipmapsEnabled() const	{ return m_bGenMipmaps; }
	bool		isUsingPBuffer() const		{ return m_bUsePBuffer; }

private:
	void makePBufferCurrent();
};

#endif