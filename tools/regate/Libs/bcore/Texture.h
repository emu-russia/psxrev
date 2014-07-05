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

#ifndef _TEXTURE_H45631_INCLUDED_
#define _TEXTURE_H45631_INCLUDED_

#pragma once

#include "common.h"
#include "BaseTexture.h"
class Image;

#define TEXTURE_RECTANGLE_ARB            0x84F5

#define GL_TEXTURE_CUBE_MAP_ARB		    0x8513

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB     0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB     0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB     0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB     0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB     0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB     0x851A

#define GL_RGBA32F_ARB                      0x8814
#define GL_RGB32F_ARB                       0x8815
#define GL_ALPHA32F_ARB                     0x8816
#define GL_INTENSITY32F_ARB                 0x8817
#define GL_LUMINANCE32F_ARB                 0x8818
#define GL_LUMINANCE_ALPHA32F_ARB           0x8819
#define GL_RGBA16F_ARB                      0x881A
#define GL_RGB16F_ARB                       0x881B
#define GL_ALPHA16F_ARB                     0x881C
#define GL_INTENSITY16F_ARB                 0x881D
#define GL_LUMINANCE16F_ARB                 0x881E
#define GL_LUMINANCE_ALPHA16F_ARB           0x881F

#define WGL_TYPE_RGBA_FLOAT_ARB                 0x21A0	// for wglChoosePixelFormat

#define FLOAT_RGBA16_NV                                 0x888A

class Texture : public BaseTexture
{
	friend class TextureManager;
private:
	std::vector<unsigned char> m_data;

	std::vector<float>	m_fData;

public:
	Texture();
	virtual ~Texture();

	void create(int width, int height, GLenum format, unsigned char* data = 0);
	void create(const Image &image, bool bResize = true);
	void toImage(Image *image);

	void set();
	void createMipMaps();

	inline int	getWidth() const	{ return m_width; }
	inline int	getHeight() const	{ return m_height; }

	bool loadMatrixFile(const std::string& filename, int sz);
	bool loadMatrixFileCSV(const std::string& filename);

	void	convolveWithNPSF(Texture *F, double xScale, double yScale);
	void	multWithBeta(float beta);
	void	reload();

	bool	loadFloatRaw(const std::string& filename, int width, int height);

	bool	storeFloatMat(const std::string& filename);	// store as matlab file

private:
	double evalF(Texture *F, double x, double y, double xScale, double yScale);
};

class Texture3D : public BaseTexture
{
	friend class TextureManager;
public:
	Texture3D();
	virtual ~Texture3D();

	void create(int format, int width, int height, int depth, unsigned char* voxels);
	bool load(const std::string &filename);
	void set();
	void createMipMaps();
	
	inline int		getWidth() const	{ return m_width; }
	inline int		getHeight() const	{ return m_height; }
	inline int		getDepth() const	{ return m_depth; }
	inline int	getMaxDim() const	{ int maxDim = m_width; if (maxDim < m_height) maxDim = m_height; if (maxDim < m_depth) maxDim = m_depth; return maxDim; }
};

class CubeTexture : public BaseTexture
{
	friend class TextureManager;
public:
	CubeTexture();
	virtual ~CubeTexture();

	void set();
	bool loadCubePPM(const std::string (&fnames)[6]);
	bool loadCubeHDR(const std::string (&fnames)[6]);
};

#endif