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
#include "RNL/rgbe.h"		// HDR file IO


CubeTexture::CubeTexture()
{
}

CubeTexture::~CubeTexture()
{
	free();
}

void CubeTexture::set()
{
	if (m_texture) {
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_texture);
	}
}

bool CubeTexture::loadCubePPM(const std::string (&fnames)[6])
{
	if (!m_texture) {
		glGenTextures(1, &m_texture);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_texture);

	// Load all faces of the cube
	for (int i=0; i<6; ++i)
	{
		std::vector<unsigned char> data;

		// load data.
		BaseTextFile file;
		file.setReadWholeLines(false);
		file.skipComments(true);
		file.addLineCommentDef("#");

		if (!file.loadFile(fnames[i], true))
			return false;

		// read magic number
		std::string word;
		int width, height;
		int maxColComp;	// maximum color component
		std::vector<Color> pixels;
		file >> word;
		if (word == "P6")
		{
			Console::print("\tloading binary ppm file...\n");

			// reading binary file
			file >> width;
			file >> height;
			file >> maxColComp;

			// we take it from here.
			FILE *fp = file.getFP();
			pixels.resize(width*height);
			int nlast = width*height-1;
			for (int j=0; j<height; ++j)
				for (int i=0; i<width; ++i)
				{
					unsigned char val = fgetc(fp);
					pixels[nlast - (j*width+i)].r = (float)val/maxColComp;
					val = fgetc(fp);
					pixels[nlast - (j*width+i)].g = (float)val/maxColComp;
					val = fgetc(fp);
					pixels[nlast - (j*width+i)].b = (float)val/maxColComp;
				}
		}
		else if (word == "P3")
		{
			Console::print("\tloading ascii ppm file...\n");

			// reading ascii file
			file >> width;
			file >> height;
			file >> maxColComp;
			
			pixels.resize(width*height);
			int nlast = width*height-1;
			for (int j=0; j<height; ++j)
				for (int i=0; i<width; ++i)
				{
					file >> pixels[nlast - (j*width+i)].r;
					file >> pixels[nlast - (j*width+i)].g;
					file >> pixels[nlast - (j*width+i)].b;
					pixels[j*width+i] /= (float)maxColComp;
				}
		}
		else
			return false;

		Console::print("\t-cubemap face loaded: width = %d, height = %d (%s)\n", width, height, fnames[i].c_str());

		// translate to bytes
		data.resize(width*height*3);
		for (int k=0; k<width; ++k)
			for (int j=0; j<height; ++j)
			{
				data[j*width*3 + 3*k  ] = (unsigned char)(pixels[j*width + k].r*255);
				data[j*width*3 + 3*k+1] = (unsigned char)(pixels[j*width + k].g*255);
				data[j*width*3 + 3*k+2] = (unsigned char)(pixels[j*width + k].b*255);
			}
		
		// create the opengl texture object
		int faceId = 0;
		switch (i) {
		case 0: faceId = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB; break;
		case 1: faceId = GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB; break;
		case 2: faceId = GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB; break;
		case 3: faceId = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB; break;
		case 4: faceId = GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB; break;
		case 5: faceId = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB; break;
		}
		glTexImage2D(faceId, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	Console::print("\tcubemap texture loaded ok.\n\n");

	return true;
}

bool CubeTexture::loadCubeHDR(const std::string (&fnames)[6])
{
	if (!m_texture) {
		glGenTextures(1, &m_texture);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_texture);
	
	// Load all faces of the cube
	for (int i=0; i<6; ++i)
	{
		// Read in the HDR light probe.
		FILE* fp = fopen(fnames[i].c_str(), "rb" );
		if (!fp)
			return false;

		rgbe_header_info info;
		int nWidth, nHeight;
		RGBE_ReadHeader( fp, &nWidth, &nHeight, &info );

		// We really don't need this
		float fExposure = info.exposure;
		float fGamma  = info.gamma;

		// Create a float array to read in the RGB components
		float* fHDRPixels = new float[3 * nWidth * nHeight];
		memset(fHDRPixels, 0, 3 * nWidth * nHeight * sizeof(float));

		RGBE_ReadPixels_RLE( fp, fHDRPixels, nWidth, nHeight );

		// copy pixel data
		std::vector<float> data(4*nWidth*nHeight);
		for (int k=0; k<nWidth*nHeight; ++k)
		{
			data[k*4] = fHDRPixels[k*3];
			data[k*4+1] = fHDRPixels[k*3+1];
			data[k*4+2] = fHDRPixels[k*3+2];
			data[k*4+3] = 0;
		}

		// copy the pixel data
		if (nWidth != nHeight || (m_width>0 && nWidth != m_width))
		{
			Console::print("ERROR: texture faces for cube texture are not square and of equal size!\n");
			glDeleteTextures(1, &m_texture);
			return false;
		}
		m_width = nWidth;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i, 0, GL_RGBA16F_ARB, nWidth, nHeight, 0, GL_RGBA, GL_FLOAT, &data[0]);
		//gluBuild2DMipmaps(faceId, GL_RGBA16F_ARB, nWidth, nHeight, GL_RGBA, GL_FLOAT, &data[0]);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		delete[] fHDRPixels;
	}
	
	Console::print("\t-loaded HDR cubemap texture (size = %d)\n", m_width);

	return true;
}