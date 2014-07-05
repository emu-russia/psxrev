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


Texture3D::Texture3D()
{
}

Texture3D::~Texture3D()
{
	free();
}

bool Texture3D::load(const std::string &fname)
{
	ASSERT(glTexImage3D);

	// free any already allocated texture
	free();

	FILE *fp=fopen(fname.c_str(),"rb");
	if (!fp)
		return false;

	char str[100], imageType[100];
	fgets(str,100,fp);
	sscanf(str,"%s",imageType);

	// Make sure this is a volume file
	if(strncmp(imageType,"P7",2))
		return false;

	// skip comments embedded in header
	fgets(str,100,fp);
	while(str[0]=='#')
		fgets(str,100,fp);

	// read image dimensions 
	int w, h, d;
	sscanf(str,"%d %d %d", &w, &h, &d);

	// read the next line into dummy variable
	fgets(str,100,fp);
		
	// read the image data 
	std::vector<unsigned char> data(w*h*d);
	fread(&data[0], sizeof(unsigned char), w*h*d, fp);

	fclose(fp);

	// create the texture
	create(GL_LUMINANCE, w, h, d, &data[0]);
	Console::print("\tLoading 3D texture : %s\n", fname.c_str());

	return true;
}

void Texture3D::create(int format, int width, int height, int depth, unsigned char* voxels)
{
	ASSERT(voxels);

	if (m_texture)
		glDeleteTextures(1, &m_texture);
	
	// Create the texture
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_3D, m_texture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0, format, 
				GL_UNSIGNED_BYTE, voxels);
	m_width = width;
	m_height = height;
	m_depth = depth;
	
	Console::print("\tLoaded 3D texture (%d x %d x %d)\n", m_width, m_height, m_depth);
}

void Texture3D::set()
{
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, m_texture);
}