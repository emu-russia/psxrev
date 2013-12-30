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

#include "Image.h"
#include "Texture.h"
#include "RNL/rgbe.h"		// HDR file IO
#include "misc.h"
#include "histogram.h"
#include "BaseTextFile.h"
#include "lodepng/lodepng.h"
#include <algorithm>

Image::Image() : m_width(0),
	m_height(0),
	m_bytesPerPixel(0),
	m_bytesPerChannel(0),
	m_exposure(1.0),
	m_gamma(1.0)
{
}

Image::~Image()
{
	clear();
}

void Image::create(size_t width, size_t height, unsigned char num_channels, Format format)
{
	ASSERT(width > 0);
	ASSERT(height > 0);

	m_width = width;
	m_height = height;
	m_format = format;
	m_nChannels = num_channels;
	m_bytesPerChannel = getFormatSize(format);
	m_bytesPerPixel = num_channels * m_bytesPerChannel;
	m_data.resize(m_width*m_height*m_bytesPerPixel);
}

void Image::clear()
{
	m_width = 0;
	m_height = 0;
	m_nChannels = 0;
	m_data.clear();
}

size_t Image::getFormatSize(Image::Format format)
{
	switch (format)
	{
	case Image::I8BITS: return 1;
	case Image::I16BITS: return 2;
	case Image::I32BITS: return 4;
	case Image::F16BITS: return 2;
	case Image::F32BITS: return 4;
	case Image::F64BITS: return 8;
	default:
		ASSERT(0);
		return 0;
	}
}

void Image::copy(const Image &image)
{
	m_width = image.m_width;
	m_height = image.m_height;
	m_format = image.m_format;
	m_nChannels = image.m_nChannels;
	m_bytesPerPixel = image.m_bytesPerPixel;
	m_bytesPerChannel = image.m_bytesPerChannel;
	m_data = image.m_data;
	m_exposure = image.m_exposure;
	m_gamma = image.m_gamma;
}

bool Image::loadPPM(const std::string &fname)
{
	clear();

/*	BaseTextFile file;
	file.addLineCommentDef("#");
	file.loadFile(fname, false);
	std::string type;
	file >> type;
	int nChannels;
	if (type == "P5")
		nChannels = 1;
	else if (type == "P6")
		nChannels = 3;
	else
		return false;
	int w, h;
	file >> w;
	file >> h;
	int dummy;
	file >> dummy;

	Console::print("%d %d %d\n", w, h, dummy);

	// Create the image
	create(w, h, nChannels, Image::I8BITS);

	// read the image data
	fread(&m_data[0], sizeof(unsigned char), m_data.size(), file.getFP());

	return true;
*/
	FILE *fp;
	char imageType[3],str[1024];

	// Read PGM image file with filename "file"

	// The PGM file format for a GREYLEVEL image is:
	// P5 (2 ASCII characters) <CR>
	// two ASCII numbers for nx ny (number of rows and columns <CR>
	// 255 (ASCII number) <CR>
	// binary pixel data with one byte per pixel

	// The PGM file format for a COLOR image is:
	// P6 (2 ASCII characters) <CR>
	// two ASCII numbers for nx ny (number of rows and columns <CR>
	// 255 (ASCII number) <CR>
	// binary pixel data with three bytes per pixel (one byte for eacg RGB)

	fp=fopen(fname.c_str(),"rb");
	if (!fp)
		return false;

	// read the first ASCII line to find out if we read a color image or
	// a greylevel image

	fgets(str,100,fp);
	sscanf(str,"%s",imageType);

	Format format = Image::I8BITS;
	int nChannels = 0;
	if(!strncmp(imageType,"P5",2)) // greylevel image
		nChannels = 1;
	else if(!strncmp(imageType,"P6",2)) // color image 
		nChannels = 3;
	else
		return false;

	// skip comments embedded in header
	fgets(str,1024,fp);  
	while(str[0]=='#')
		fgets(str,1024,fp);

	// read image dimensions 
	int w, h;
	sscanf(str,"%d %d", &w, &h);

	// read the next line into dummy variable
	fgets(str,1024,fp);

	// Create the image
	create(w, h, nChannels, format);
		
	// read the image data
	fread(&m_data[0], sizeof(unsigned char), w*h*m_bytesPerPixel, fp);

	fclose(fp);

	if (m_width==0 || m_height==0)
		return false;

	return true;
}

bool Image::savePPM(const std::string &fname) const
{
	FILE *fp;

	// Write PGM image file with filename "file"

	// The PGM file format for a GREYLEVEL image is:
	// P5 (2 ASCII characters) <CR>
	// two ASCII numbers for nx ny (number of rows and columns <CR>
	// 255 (ASCII number) <CR>
	// binary pixel data with one byte per pixel

	// The PGM file format for a COLOR image is:
	// P6 (2 ASCII characters) <CR>
	// two ASCII numbers for nx ny (number of rows and columns <CR>
	// 255 (ASCII number) <CR>
	// binary pixel data with three bytes per pixel (one byte for each R,G,B)

	fp=fopen(fname.c_str(),"wb");
	if (!fp)
		return false;

	// write the first ASCII line with the file type
	if (m_nChannels == 1)
   		fprintf(fp,"P5\n"); //greylevel image
	else if (m_nChannels == 3)
		fprintf(fp,"P6\n");  // color image
	else
		return false;

	// write image dimensions 
	fprintf(fp,"%d %d\n", m_width, m_height);

	// write '255' to the next line 
	fprintf(fp,"255\n");

	// write the image data
	if (m_width*m_height > 0)
		fwrite(&m_data[0], sizeof(unsigned char), m_width*m_height*m_bytesPerPixel, fp);

	fclose(fp);
	
	return true;
}

void Image::flip()
{
	// create a temp swap space
	size_t bpChannel = getBytesPerChannel();
	unsigned char *buf = new unsigned char[bpChannel];

    for (size_t i=0; i<m_width; ++i)
	{
		for (size_t j=0; j<m_height/2; ++j)
		{
			for (size_t k=0; k<m_nChannels; ++k)
			{
				// swap pixels
				memcpy(buf, &m_data[m_bytesPerPixel*(j*m_height + i) + bpChannel*k], bpChannel);
				memcpy( &m_data[m_bytesPerPixel*(j*m_height + i) + bpChannel*k],
						&m_data[m_bytesPerPixel*((m_height-1-j)*m_height + i) + bpChannel*k],
						bpChannel);
				memcpy(&m_data[m_bytesPerPixel*((m_height-1-j)*m_height + i) + bpChannel*k], buf, bpChannel);
			}
		}
	}
}

void Image::convolution(double *matrix, int size)
{
	ASSERT(matrix);

	int border = int(size/2.0);
	size_t colorPlanes = getChannelsNum();
	std::vector<unsigned char> tmpData = m_data;

	// calc total weight of the kernel (for most kernels
	// it should be 1..)
	double filterW = 0;
	for (int i=0; i<size; ++i)
		for (int j=0; j<size; ++j)
			filterW += matrix[size*j+i];

	for (size_t x=0; x<m_width; ++x)
		for (size_t y=0; y<m_height; ++y)
		{
			for (size_t k=0; k<colorPlanes; ++k)
			{
				double val = 0;
				double f = 0;
				for (int i=0; i<size; ++i)
					for (int j=0; j<size; ++j)
					{
						if (x+i-border < 0 || x+i-border >= m_width ||
							y+j-border < 0 || y+j-border >= m_height)
							f += matrix[size*j + i];
						else
							val += (*this)(x+i-border,y+j-border)[k] * matrix[size*j + i];
					}
				// compensate for borders
				if (filterW != 0)
					val *= 1.0/(1.0 - f/filterW);

				// write the final value
				tmpData[colorPlanes*(y*m_width + x) + k] = (unsigned char)val;
			}
		}

	m_data = tmpData;
}

void Image::crop(size_t minX, size_t minY, size_t maxX, size_t maxY)
{
	ASSERT(minX >= 0 && minY >= 0);
	ASSERT(maxX < m_width && maxY < m_height);

	size_t w = maxX - minX+1;
	size_t h = maxY - minY+1;
	std::vector<unsigned char> data(w*h*m_bytesPerPixel);

	for (size_t i=minX; i<=maxX; ++i)
	{
		for (size_t j=minY; j<=maxY; ++j)
		{
			for (size_t c=0; c<m_bytesPerPixel; ++c)
				data[m_bytesPerPixel*((j-minY)*w + (i-minX))+c] = (*this)(i,j)[c];
		}
	}

	m_data = data;
	m_width = w;
	m_height = h;
}

std::ostream& operator << (std::ostream& stream, const Image& img)
{
	stream.write((char*)&img.m_width, sizeof(img.m_width));
	stream.write((char*)&img.m_height, sizeof(img.m_height));
	stream.write((char*)&img.m_bytesPerPixel, sizeof(img.m_bytesPerPixel));
	stream.write((char*)&img.m_nChannels, sizeof(img.m_nChannels));
	stream.write((char*)&img.m_format, sizeof(img.m_format));
	stream.write((char*)&img.m_exposure, sizeof(img.m_exposure));
	stream.write((char*)&img.m_gamma, sizeof(img.m_gamma));
	int datasize = (int)img.m_data.size();
	stream.write((char*)&datasize, sizeof(datasize));
	if (datasize > 0)
		stream.write((char*)&img.m_data[0], sizeof(unsigned char)*(int)img.m_data.size());
	return stream;
}

std::istream& operator >> (std::istream& stream, Image& img)
{
	img.clear();
	int datasize;
	stream.read((char*)&img.m_width, sizeof(img.m_width));
	stream.read((char*)&img.m_height, sizeof(img.m_height));
	stream.read((char*)&img.m_bytesPerPixel, sizeof(img.m_bytesPerPixel));
	stream.read((char*)&img.m_nChannels, sizeof(img.m_nChannels));
	stream.read((char*)&img.m_format, sizeof(img.m_format));
	stream.read((char*)&img.m_exposure, sizeof(img.m_exposure));
	stream.read((char*)&img.m_gamma, sizeof(img.m_gamma));
	img.m_bytesPerChannel = Image::getFormatSize(img.m_format);
	ASSERT(img.m_bytesPerChannel == img.m_bytesPerPixel/img.m_nChannels);
	stream.read((char*)&datasize, sizeof(datasize));
	img.m_data.resize(datasize);
	if (datasize > 0)
		stream.read((char*)&img.m_data[0], sizeof(unsigned char)*datasize);
	Console::print("image %d x %d, %d bytespp, format: %d (%d bytes)\n", img.m_width, img.m_height, img.m_bytesPerPixel,
		(int)img.m_format, (int)datasize);
	return stream;
}

Color Image::sample_at(double fx, double fy) const
{
	int x = (int)fx;
	int y = (int)fy;

	ASSERT(fx>=0 && fx<getWidth() && fy>=0 && fy<getHeight());

	// sample the (x,y)
	Color cl00((*this)(x,y));
	Color cl01, cl10, cl11;

	if (x<(int)getWidth()-1)
		cl01 = Color((*this)(x+1,y));
	else
		cl01 = cl00;
	if (y<(int)getHeight()-1)
		cl10 = Color((*this)(x,y+1));
	else
		cl10 = cl00;
	if (x<(int)getWidth()-1 && y<(int)getHeight()-1)
		cl11 = Color((*this)(x+1,y+1));
	else
		cl11 = cl00;

/*	// find the tangent at pixel (x,y)
	Color tangentX(0,0,0), tangentY(0,0,0);
	if (x > 0)
		tangentX += Color((*this)(x-1,y));
	else
		tangentX += cl00;
	if (x < getWidth()-1)
		tangentX -= Color((*this)(x+1,y));
	else
		tangentX -= cl00;
	if (y > 0)
		tangentY += Color((*this)(x,y-1));
	else
		tangentY += cl00;
	if (y < getHeight()-1)
		tangentY -= Color((*this)(x,y+1));
	else
		tangentY -= cl00;*/

	// interpolate across x and y
	double tmp_i;
	float dfx = (float)modf(fx, &tmp_i);
	float dfy = (float)modf(fy, &tmp_i);
	
	Color cl1 = (1-dfx)*cl00 + dfx*cl01;
	Color cl2 = (1-dfx)*cl10 + dfx*cl11;
	return (1-dfy)*cl1 + dfy*cl2;
}

bool Image::load(const std::string &fname)
{
	Console::print("loading %s\n", fname.c_str());

	clear();
	size_t pos = fname.find_last_of(".");
	std::string ext = fname.substr(pos+1);
	std::transform(ext.begin(), ext.end(), ext.begin(), toupper);
	if (ext == "BMP")
		return loadBMP(fname);
	else if (ext == "PPM")
		return loadPPM(fname);
	else if (ext == "HDR")
		return loadHDR(fname);
	else if (ext == "PNG")
		return loadPNG(fname);
	else {
		Console::error("unsupported image file extension %s\n", ext.c_str());
		return false;
	}
}

bool Image::loadWithAlpha(const std::string &img_fname, const std::string &alpha_fname)
{
	// load the two separate images
	Image img, alpha;
	if (!img.load(img_fname))
		return false;
	if (!alpha.load(alpha_fname))
		return false;

	// check they match
	if (img.getWidth() != alpha.getWidth() || img.getHeight() != alpha.getHeight() ||
		img.getBytesPerChannel() != alpha.getBytesPerChannel()) {
		Console::error("Image::loadWithAlpha : image properties dont match\n");
		return false;
	}
	if (img.getChannelsNum() < 3) {
		Console::error("Image::loadWithAlpha : function doesnt support images with less than 3 channels\n");
		return false;
	}

	// combine them into this one
	clear();
	m_width = img.getWidth();
	m_height = img.getHeight();
	m_nChannels = 4;
	m_bytesPerChannel = img.getBytesPerChannel();
	m_bytesPerPixel = m_nChannels * m_bytesPerChannel;
	m_data.resize(m_width*m_height*m_bytesPerPixel);
	for (size_t i=0; i<m_width; ++i)
		for (size_t j=0; j<m_height; ++j)
		{
			memcpy(&m_data[m_bytesPerPixel*(j*m_width + i)  ],
				&img.m_data[img.m_bytesPerPixel*(j*m_width + i)  ],
				m_bytesPerChannel);
			memcpy(&m_data[m_bytesPerPixel*(j*m_width + i) + 1*m_bytesPerChannel],
				&img.m_data[img.m_bytesPerPixel*(j*m_width + i) + 1*m_bytesPerChannel],
				m_bytesPerChannel);
			memcpy(&m_data[m_bytesPerPixel*(j*m_width + i) + 2*m_bytesPerChannel],
				&img.m_data[img.m_bytesPerPixel*(j*m_width + i) + 2*m_bytesPerChannel],
				m_bytesPerChannel);
			memcpy(&m_data[m_bytesPerPixel*(j*m_width + i) + 3*m_bytesPerChannel],
				&alpha.m_data[alpha.m_bytesPerPixel*(j*m_width + i)],
				m_bytesPerChannel);
		}

	return true;
}

bool Image::loadHDR(const std::string &filename)
{
	// Read in the HDR light probe.
	FILE* fp = fopen(filename.c_str(), "rb" );
	if (!fp)
		return false;

	rgbe_header_info info;
	int nWidth, nHeight;
	RGBE_ReadHeader( fp, &nWidth, &nHeight, &info );

	// Create a float array and read in the RGB components
	float* fHDRPixels = new float[3 * nWidth * nHeight];
	memset(fHDRPixels, 0, 3 * nWidth * nHeight * sizeof(float));
	RGBE_ReadPixels_RLE( fp, fHDRPixels, nWidth, nHeight );

	// copy the pixel data
	m_width = nWidth;
	m_height = nHeight;
	m_bytesPerChannel = 4;
	m_format = Image::F32BITS;
	m_nChannels = 3;
	m_bytesPerPixel = m_nChannels * m_bytesPerChannel;
	m_data.resize(m_width*m_height*m_bytesPerPixel);
	memcpy(&m_data[0], fHDRPixels, m_data.size());
	
	m_exposure = info.exposure;
	m_gamma  = info.gamma;

	// clean up
	delete[] fHDRPixels;
	
	Console::print("\t-loaded hdr image %s (w = %d, h = %d)\n", filename.c_str(), m_width, m_height);

	return true;
}

void Image::normalize()
{
	if (isEmpty())
		return;

	// normalize each channel separately
	switch (m_format) {
	case Image::I8BITS:
		for (size_t c=0; c<m_nChannels; ++c) {
			uint8_t minv = at<uint8_t>(0,0,c), maxv = at<uint8_t>(0,0,c);
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
			{
				uint8_t val = at<uint8_t>(i,j,c);
				if (val < minv) minv = val;
				if (val > maxv) maxv = val;
			}
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
				at<uint8_t>(i,j,c) = 255*(at<uint8_t>(i,j,c) - minv)/(maxv-minv);
		}
		break;
	case Image::I16BITS:
		for (size_t c=0; c<m_nChannels; ++c) {
			uint16_t minv = at<uint16_t>(0,0,c), maxv = at<uint16_t>(0,0,c);
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
			{
				uint16_t val = at<uint16_t>(i,j,c);
				if (val < minv) minv = val;
				if (val > maxv) maxv = val;
			}
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
				at<uint16_t>(i,j,c) = 65535*(at<uint16_t>(i,j,c) - minv)/(maxv-minv);
		}
		break;
	case Image::I32BITS:
		for (size_t c=0; c<m_nChannels; ++c) {
			uint32_t minv = at<uint32_t>(0,0,c), maxv = at<uint32_t>(0,0,c);
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
			{
				uint32_t val = at<uint32_t>(i,j,c);
				if (val < minv) minv = val;
				if (val > maxv) maxv = val;
			}
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
				at<uint32_t>(i,j,c) = 4294967295*(at<uint32_t>(i,j,c) - minv)/(maxv-minv);
		}
		break;
	case Image::F32BITS:
		for (size_t c=0; c<m_nChannels; ++c) {
			float32_t minv = at<float32_t>(0,0,c), maxv = at<float32_t>(0,0,c);
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
			{
				float32_t val = at<float32_t>(i,j,c);
				if (val < minv) minv = val;
				if (val > maxv) maxv = val;
			}
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
				at<float32_t>(i,j,c) = (at<float32_t>(i,j,c) - minv)/(maxv-minv);
		}
		break;
	case Image::F64BITS:
		for (size_t c=0; c<m_nChannels; ++c) {
			float64_t minv = at<float64_t>(0,0,c), maxv = at<float64_t>(0,0,c);
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
			{
				float64_t val = at<float64_t>(i,j,c);
				if (val < minv) minv = val;
				if (val > maxv) maxv = val;
			}
			for (size_t i=0; i<m_width; ++i)
			for (size_t j=0; j<m_height; ++j)
				at<float64_t>(i,j,c) = (at<float64_t>(i,j,c) - minv)/(maxv-minv);
		}
		break;
	default:
		throw std::exception("format not supported");
	}
}

void Image::changeFormat(Image::Format format)
{
	if (m_format == format)
		return;

	Image newimg;
	newimg.create(m_width, m_height, m_nChannels, format);

	for (size_t x=0; x<m_width; ++x)
		for (size_t y=0; y<m_height; ++y)
		{
			for (size_t c=0; c<m_nChannels; ++c)
			{
				// read the original format
				double val;
				switch (m_format) {
					case Image::I8BITS: val = at<uint8_t>(x,y,c)/255.0; break;
					case Image::I16BITS: val = at<uint16_t>(x,y,c)/255.0; break;
					case Image::I32BITS: val = at<uint32_t>(x,y,c)/255.0; break;
					case Image::F16BITS: throw std::exception("format not supported"); break;
					case Image::F32BITS: val = at<float32_t>(x,y,c); break;
					case Image::F64BITS: val = at<float64_t>(x,y,c); break;
					default: throw std::exception("unknown destination image format");
				}

				// write it in the new format
				switch (format) {
					case Image::I8BITS: newimg.at<uint8_t>(x,y,c) = (uint8_t)(val*255); break;
					case Image::I16BITS: newimg.at<uint16_t>(x,y,c) = (uint16_t)(val*255); break;
					case Image::I32BITS: newimg.at<uint32_t>(x,y,c) = (uint32_t)(val*255); break;
					case Image::F16BITS: throw std::exception("format not supported"); break;
					case Image::F32BITS: newimg.at<float32_t>(x,y,c) = (float32_t)val; break;
					case Image::F64BITS: newimg.at<float64_t>(x,y,c) = (float64_t)val; break;
					default: throw std::exception("unknown target image format");
				}
			}
		}

	copy(newimg);
}

void Image::getHistogram(Histogram<double> &hist, size_t channel, size_t nBins) const
{
	ASSERT(nBins > 1);
	ASSERT(channel >= 0 && channel < m_nChannels);
	
	// form a histogram of this channel of the image
	hist.create(nBins, 0, 1);
	for (size_t x=0; x<m_width; ++x)
		for (size_t y=0; y<m_height; ++y)
		{
			double val;
			switch (m_format) {
				case Image::I8BITS: val = at<uint8_t>(x,y,channel)/255.0; break;
				case Image::I16BITS: val = at<uint16_t>(x,y,channel)/255.0; break;
				case Image::I32BITS: val = at<uint32_t>(x,y,channel)/255.0; break;
				case Image::F16BITS: throw std::exception("F16 format not supported"); break;
				case Image::F32BITS: val = at<float32_t>(x,y,channel); break;
				case Image::F64BITS: val = at<float64_t>(x,y,channel); break;
				default: throw std::exception("unknown destination image format");
			}

			val = clamp(val, 0.0, 1.0);
			hist.add(val);
		}
}

double Image::calcEntropy(size_t channel, size_t nBins) const
{
	// create a histogram of the image
	Histogram<double> hist;
	getHistogram(hist, channel, nBins);
	ProbHistogram<double> phist(hist);

	// for each bin, compute probability and entropy
	double entropy = 0;
	for (size_t i=0; i<phist.getBinsNum(); ++i)
	{
		double p = phist.probOfBin(i);
		if (p > 0)
			entropy += -p * log(p);
	}

	return entropy/log(2.0);
}

bool Image::loadPNG(const std::string &fname)
{
	std::vector<unsigned char> data;
	unsigned int w=0, h=0;
	if (LodePNG::decode(data, w, h, fname) != 0)
		return false;
	if (w==0||h==0)
		return false;

	create(w, h, 4, Image::I8BITS);
	m_data = data;

	Console::print("loaded PNG file (%d x %d)\n", w, h);

	return true;
}