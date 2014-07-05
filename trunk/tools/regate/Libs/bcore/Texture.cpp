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

#include "Texture.h"
#include "BaseTextFile.h"
#include "Image.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	free();
}

void Texture::create(int width, int height, GLenum format, unsigned char* data)
{
	// create the texture object
	if (!m_texture) {
		glGenTextures(1, &m_texture);
	}
	if (m_texture == 0)
		Console::print("ERROR: failed to create OpenGL texture\n");

	// get the correct formats
	int dataformat = GL_UNSIGNED_BYTE;
	switch (format)
	{
	case GL_RGBA32F_ARB:
	case GL_RGB32F_ARB:
	case GL_ALPHA32F_ARB:
	case GL_INTENSITY32F_ARB:
	case GL_LUMINANCE32F_ARB:
	case GL_LUMINANCE_ALPHA32F_ARB:
	case GL_RGBA16F_ARB:
	case GL_RGB16F_ARB:
	case GL_ALPHA16F_ARB:
	case GL_INTENSITY16F_ARB:
	case GL_LUMINANCE16F_ARB:
	case GL_LUMINANCE_ALPHA16F_ARB:
		dataformat = GL_FLOAT;
		break;
	}
	
	int texformat = format;
	switch (format)
	{
	case GL_RGB8:
		texformat = GL_RGB;
		break;
	case GL_RGBA8:
		texformat = GL_RGBA;
		break;

	case GL_RGBA32F_ARB:
	case GL_RGBA16F_ARB:
		texformat = GL_RGBA;
		break;
	case GL_RGB32F_ARB:
	case GL_RGB16F_ARB:
		texformat = GL_RGB;
		break;
	case GL_ALPHA32F_ARB:
	case GL_INTENSITY32F_ARB:
	case GL_LUMINANCE32F_ARB:
	case GL_LUMINANCE_ALPHA32F_ARB:
	case GL_ALPHA16F_ARB:
	case GL_INTENSITY16F_ARB:
	case GL_LUMINANCE16F_ARB:
	case GL_LUMINANCE_ALPHA16F_ARB:
		Console::print("Texture::create: ERROR - unable to handle format\n");
		break;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, texformat, dataformat, data);
	
	//GLboolean res;
	//bool bres = glAreTexturesResident(1, &m_texture, &res) != 0;
	//Console::print("texture resident: %d\n", bres);

	m_width = width;
	m_height = height;
}

void Texture::create(const Image &image, bool bResize)
{
	// create the texture object
	if (!m_texture) {
		glGenTextures(1, &m_texture);
	}
	if (m_texture == 0)
		Console::print("ERROR: failed to create OpenGL texture\n");

	// find the best texture format to represent the image
	GLenum format, dataformat, imgformat;
	bool bNotSupported = false;
	switch (image.getFormat())
	{
	case Image::I8BITS:
		dataformat = GL_UNSIGNED_BYTE;
		if (image.getChannelsNum() == 1)
			format = GL_LUMINANCE8;
		else if (image.getChannelsNum() == 3)
			format = GL_RGB8;
		else if (image.getChannelsNum() == 4)
			format = GL_RGBA8;
		else
			bNotSupported = true;
		break;
	/*case Image::I16BITS:
		dataformat = GL_UNSIGNED_WORD;
		if (image.getChannelsNum() == 1)
			format = GL_LUMINANCE16;
		else if (image.getChannelsNum() == 3)
			format = GL_RGB16;
		else if (image.getChannelsNum() == 4)
			format = GL_RGBA16;
		else
			bNotSupported = true;
		break;*/
	/*case Image::F16BITS:
		dataformat = GL_FLOAT;
		if (image.getChannelsNum() == 1)
			format = GL_LUMINANCE16F_ARB;
		else if (image.getChannelsNum() == 3)
			format = GL_RGB16F_ARB;
		else if (image.getChannelsNum() == 4)
			format = GL_RGBA16F_ARB;
		else
			bNotSupported = true;
		break;*/
	case Image::F32BITS:
		dataformat = GL_FLOAT;
		if (image.getChannelsNum() == 1)
			format = GL_LUMINANCE32F_ARB;
		else if (image.getChannelsNum() == 3)
			format = GL_RGB32F_ARB;
		else if (image.getChannelsNum() == 4)
			format = GL_RGBA32F_ARB;
		else
			bNotSupported = true;
		break;
	default:
		bNotSupported = true;
	}
	if (image.getChannelsNum() == 1)
		imgformat = GL_LUMINANCE;
	else if (image.getChannelsNum() == 3)
		imgformat = GL_RGB;
	else if (image.getChannelsNum() == 4)
		imgformat = GL_RGBA;
	else
		bNotSupported = true;
	if (bNotSupported)
	{
		Console::error("Texture::create(): image format is not supported (%d)\n", image.getFormat());
		return;
	}

	// get a ptr to the data to use
	void *data = (void*)image(0,0);

	// if the image doesnt have power-of-2 dimensions, then
	// either resize the image or use OpenGL extensions
	size_t w=1;
	size_t h=1;
	while (w<image.getWidth())
		w *= 2;
	while (h<image.getHeight())
		h *= 2;
	if (w>2048)
		w=2048;
	if (h>2048)
		h=2048;
	Image tmp_img;
	if (w != image.getWidth() || h != image.getHeight())
	{
		//bResize = true;
		if (bResize) {
			tmp_img.copy(image);
			tmp_img.resize(w, h);
			data = (void*)tmp_img(0,0);
		}
		else {
			//TODO: check if non-power-of-2 textures are supported
			// if not, create a padded power-of-2 image
			w = image.getWidth();
			h = image.getHeight();
		}
	}

	// now create the texture
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, imgformat, dataformat, data);
	
	m_width = w;
	m_height = h;
}

void Texture::set()
{
	if (m_texture) {
		ASSERT(glIsTexture(m_texture));
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}

void Texture::createMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, &m_data[0]);
	Console::print("\tGenerated texture mipmaps\n");
}

bool Texture::loadMatrixFile(const std::string& filename, int sz)
{
	// we suppose a szxsz matrix, comma delimited

	Console::print("\t-loading %s (Matlab matrix text file)\n", filename.c_str());

	// Open the file
	FILE *fp = fopen(filename.c_str(), "r");
	if (!fp)
		return false;

	// Create the texture
	float *data = new float[sz*sz];

	for (int i=0; i<sz; ++i)
		for (int j=0; j<sz; ++j)
		{
			// read until next comma or new line
			std::string str;
			char ch;
			do {
				ch = getc(fp);
				if (isspace(ch))
					continue;
				str += ch;
			} while (ch != ',' && ch != '\n' && !feof(fp));

			double val = atof(str.c_str());

			// Write this value to the texture.
			data[j*sz+i] = (float)val;
		}

	fclose(fp);

	Console::print("\t\tok\n");
	
	glGetError();

	// create the opengl texture object
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, sz, sz, 0, GL_LUMINANCE, GL_FLOAT, data);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		Console::print("ERROR: %s\n", gluErrorString(error));
	else
		Console::print("\t\t-texture created ok\n");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLboolean res;
	bool bres = glAreTexturesResident(1, &m_texture, &res) != 0;
	Console::print("\t\t-texture resident: %d\n", bres);

	m_width = sz;
	m_height = sz;

	delete[] data;

	return true;
}


bool Texture::loadMatrixFileCSV(const std::string& filename)
{
	// we suppose a szxsz matrix, comma delimited

	Console::print("\t-loading %s (CSV matrix text file)\n", filename.c_str());

	// load data.
	BaseTextFile file;
	file.setReadWholeLines(false);
	//file.skipComments(true);
	//file.addLineCommentDef("#");

	if (!file.loadFile(filename, true))
		return false;

	// read header.
	int xResolution, yResolution;
	int xBegin, xEnd;
	int yBegin, yEnd;
	file >> xResolution;
	file >> yResolution;
	file >> xBegin;
	file >> xEnd;
	file >> yBegin;
	file >> yEnd;

	Console::print("\t\txResolution = %d, yResolution = %d\n\t\txBegin = %d, xEnd = %d\n\t\tyBegin = %d, yEnd = %d\n",
					xResolution, yResolution, xBegin, xEnd, yBegin, yEnd);

	// Manual control from this point on
	FILE *fp = file.getFP();

	// Create the texture
	m_fData.resize(xResolution*yResolution);


	for (int i=0; i<xResolution; ++i)
		for (int j=0; j<yResolution; ++j)
		{
			// read until next comma or new line
			std::string str;
			char ch;
			do {
				ch = getc(fp);
				if (isspace(ch))
					continue;
				str += ch;
			} while (ch != ',' && ch != '\n' && !feof(fp));

			double val = atof(str.c_str());

			// Write this value to the texture.
			m_fData[j*xResolution+i] = (float)val;
		}

//	fclose(fp);
	file.close();

	Console::print("\t\tok\n");
	
	glGetError();

	// create the opengl texture object
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, xResolution, yResolution, 0, GL_LUMINANCE, GL_FLOAT, &m_fData[0]);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		Console::print("ERROR: %s\n", gluErrorString(error));
	else
		Console::print("\t\t-texture created ok\n");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLboolean res;
	bool bres = glAreTexturesResident(1, &m_texture, &res) != 0;
	Console::print("\t\t-texture resident: %d\n", bres);

	m_width = xResolution;
	m_height = yResolution;

	return true;
}

bool Texture::storeFloatMat(const std::string &filename)
{
	GLfloat *buf = new GLfloat[m_width*m_height];

	// get the OpenGL texture data
	set();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, buf);

	// create a file
	FILE *fp = fopen(filename.c_str(), "w+");

	for (int i=0; i<m_width; ++i)
	{
		for (int j=0; j<m_height; ++j)
		{
			fprintf(fp, "%f ", buf[i*m_height + j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	delete[] buf;

	Console::print("-stored %dx%d texture\n", m_width, m_height);

	return true;
}

void Texture::toImage(Image *image)
{
	ASSERT(image);
	
	// get the OpenGL texture data
	unsigned char *buf = new unsigned char[m_width*m_height*3];
	set();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);

	// copy it to the image
	image->create(m_width, m_height, 3, Image::I8BITS);
	Console::print("%d x %d image from texture\n", m_width, m_height);
	for (int i=0; i<m_width; ++i)
		for (int j=0; j<m_height; ++j) {
			(*image)(i,j)[0] = buf[3*(j*m_width + i)];
			(*image)(i,j)[1] = buf[3*(j*m_width + i)+1];
			(*image)(i,j)[2] = buf[3*(j*m_width + i)+2];
		}
}