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

#ifndef _IMAGE_H45631_INCLUDED_
#define _IMAGE_H45631_INCLUDED_

#pragma once

#include "common.h"
#include "Matrix.h"
#include <fstream>
#include "histogram.h"
#include <exception>

class Image
{
	friend class EdgeDetector;

public:
	enum Format { // I: integer format, F: floating point format
		I8BITS,
		I16BITS,
		I32BITS,
		F16BITS,
		F32BITS,
		F64BITS
	};

	enum Filter {
		BOX = 0,
		LINEAR,
		CUBIC,
		FILTERS_NUM
	};

#define LUT_SAMPLES 10000
	static double	m_filterLUT[FILTERS_NUM][LUT_SAMPLES];

protected:
	std::vector<unsigned char>	m_data;
	size_t			m_width;
	size_t			m_height;
	size_t			m_bytesPerPixel;
	Format			m_format;
	size_t			m_nChannels;
	size_t			m_bytesPerChannel;
	double			m_exposure;	// for HDR only
	double			m_gamma;	// for HDR only

public:
	Image();
	Image(const Image& img) { copy(img); }
	virtual ~Image();	// gniah.. if not subclassed, spare the vfptr..

	static void precomputeLUTs();

	void create(size_t width, size_t height, unsigned char num_channels=3, Format format = I8BITS);
	void clear();
	void copy(const Image& image);

	void rawCopy(void *data)	{ memcpy(&m_data[0], data, m_data.size()); }

	Format		getFormat() const			{ return m_format; }
	size_t		getChannelsNum() const	{ return m_nChannels; }
	size_t		getWidth() const			{ return m_width; }
	size_t		getHeight() const			{ return m_height; }
	size_t		getBytesPerPixel() const	{ return m_bytesPerPixel; }
	size_t		getBytesPerChannel() const	{ return m_bytesPerChannel; }
	bool		isEmpty() const			{ if (m_width*m_height == 0 || m_data.size()==0) return true; return false; }
	double		getExposure() const			{ return m_exposure; }
	double		getGamma() const			{ return m_gamma; }
	void*		getData()					{ return (void*)&m_data[0]; }

	static size_t	getFormatSize(Format format);

	void changeFormat(Format format);
	void normalize();
	void flip();
	void resize(double scale, Filter filter=CUBIC, double filter_stretch = 1.0);
	void resize(size_t neww, size_t newh, Filter filter=CUBIC, double filter_stretch = 1.0);
	void crop(size_t minX, size_t minY, size_t maxX, size_t maxY);
	bool load(const std::string &fname);
	bool loadWithAlpha(const std::string &img_fname, const std::string &alpha_fname);
	bool loadPPM(const std::string &fname);
	bool loadBMP(const std::string& fname);
	bool loadHDR(const std::string& fname);
	bool loadPNG(const std::string& fname);
	bool savePPM(const std::string &fname) const;
	bool savePNG(const std::string &fname) const;
	void convolution(double *matrix, int size);
	void getHistogram(Histogram<double> &hist, size_t channel, size_t nBins=256) const;
	double calcEntropy(size_t channel, size_t nBins=64) const;

	// convert to and from other objects
	template <class T> void fromMatrix(const Matrix<T> &mat, bool bScaleValues=false);
	template <class T> void toMatrix(Matrix<T> &mat) const;

	Color sample_at(double x, double y) const;	// sample the image using hermite interpolation

	// data access
	__forceinline		unsigned char* operator () (size_t x, size_t y)		{ return &m_data[m_bytesPerPixel*(m_width*y + x)]; }
	__forceinline const unsigned char* operator () (size_t x, size_t y) const	{ return &m_data[m_bytesPerPixel*(m_width*y + x)]; }

	template <class T>
	T& at(size_t x, size_t y, size_t k) { return *(T*)&m_data[m_bytesPerPixel*(m_width*y + x) + k*m_bytesPerChannel]; }
	template <class T>
	const T& at(size_t x, size_t y, size_t k) const { return *(T*)&m_data[m_bytesPerPixel*(m_width*y + x) + k*m_bytesPerChannel]; }

	__forceinline Color	color(size_t x, size_t y) const;

	// copy operator
	Image& operator = (const Image& img) { copy(img); return *this; }
	
	// serialization functions
	friend std::ostream& operator << (std::ostream& stream, const Image& img);
	friend std::istream& operator >> (std::istream& stream, Image& img);

private:
	inline void interpolate(double x, double y, Filter filter, double filter_stretch, double out[3]);
};

/**
 * EdgeDetector:
 *
 */
class EdgeDetector
{
public:
	static void sobel(const Image &image, std::vector< Matrix<double> > &gradient, 
										std::vector< Matrix<double> > &angle);
	static void canny(const Image &image, std::vector< Matrix<double> > &gradient, 
										std::vector< Matrix<double> > &angle);
	static void combineResponses(const std::vector< Matrix<double> > &resp, Matrix<double> &out);
};

//--------------------------------

Color Image::color(size_t x, size_t y) const
{
	Color cl;
	switch (m_format) {
		case Image::I8BITS:
			cl.r = (float)(at<uint8_t>(x,y,0) / 255.0);
			if (m_nChannels > 1)
				cl.g = (float)(at<uint8_t>(x,y,1) / 255.0);
			if (m_nChannels > 2)
				cl.b = (float)(at<uint8_t>(x,y,2) / 255.0);
			break;
		case Image::I16BITS:
			cl.r = (float)(at<uint16_t>(x,y,0) / 255.0);
			if (m_nChannels > 1)
				cl.g = (float)(at<uint16_t>(x,y,1) / 255.0);
			if (m_nChannels > 2)
				cl.b = (float)(at<uint16_t>(x,y,2) / 255.0);
			break;
		case Image::I32BITS:
			cl.r = (float)(at<uint32_t>(x,y,0) / 255.0);
			if (m_nChannels > 1)
				cl.g = (float)(at<uint32_t>(x,y,1) / 255.0);
			if (m_nChannels > 2)
				cl.b = (float)(at<uint32_t>(x,y,2) / 255.0);
			break;
		case Image::F16BITS:
			Console::error("Image::color() : F16BITS not supported!!\n");
			break;
		case Image::F32BITS:
			cl.r = (float)(at<float32_t>(x,y,0));
			if (m_nChannels > 1)
				cl.g = (float)(at<float32_t>(x,y,1));
			if (m_nChannels > 2)
				cl.b = (float)(at<float32_t>(x,y,2));
			break;
		case Image::F64BITS:
			cl.r = (float)(at<float64_t>(x,y,0));
			if (m_nChannels > 1)
				cl.g = (float)(at<float64_t>(x,y,1));
			if (m_nChannels > 2)
				cl.b = (float)(at<float64_t>(x,y,2));
			break;
	}
	return cl;
}

template <class T>
void Image::fromMatrix(const Matrix<T> &mat, bool bScaleValues)
{
	clear();

	int w = mat.numCols();
	int h = mat.numRows();

	m_width = w;
	m_height = h;
	m_bytesPerPixel = 1;
	m_bytesPerChannel = 1;
	m_format = Image::I8BITS;
	m_nChannels = 1;
	m_data.resize(w*h);
	if (!bScaleValues)
		for (int x=0; x<w; ++x)
			for (int y=0; y<h; ++y)
				(*this)(x,y)[0] = (unsigned char)mat(x,y);
	else {
		// find the min/max values
		double maxV = mat(0,0), minV = mat(0,0);
		for (int x=0; x<w; ++x)
			for (int y=0; y<h; ++y)
			{
				if (mat(x,y) > maxV) maxV = mat(x,y);
				if (mat(x,y) < minV) minV = mat(x,y);
			}

		// if matrix is uniform, just zero it out
		if (minV == maxV) maxV = minV + 1.0;

		// copy normalized
		for (int x=0; x<w; ++x)
			for (int y=0; y<h; ++y) {
				(*this)(x,y)[0] = (unsigned char)(255*(mat(x,y) - minV)/(maxV - minV));
			}
	}
}

template <class T>
void Image::toMatrix(Matrix<T> &mat) const
{
	mat.create(m_width, m_height);

	switch (m_format) {
	case Image::I8BITS:
		for (size_t x=0; x<m_width; ++x)
			for (size_t y=0; y<m_height; ++y) {
				mat(x,y) = 0;
				for (size_t c=0; c<m_nChannels; ++c)
					mat(x,y) += at<uint8_t>(x,y,c) / 255.0 / m_nChannels;
			}
		break;
	case Image::I16BITS:
		for (size_t x=0; x<m_width; ++x)
			for (size_t y=0; y<m_height; ++y) {
				mat(x,y) = 0;
				for (size_t c=0; c<m_nChannels; ++c)
					mat(x,y) += at<uint16_t>(x,y,c) / 255.0 / m_nChannels;
			}
		break;
	case Image::I32BITS:
		for (size_t x=0; x<m_width; ++x)
			for (size_t y=0; y<m_height; ++y) {
				mat(x,y) = 0;
				for (size_t c=0; c<m_nChannels; ++c)
					mat(x,y) += at<uint32_t>(x,y,c) / 255.0 / m_nChannels;
			}
		break;
	case Image::F32BITS:
		for (size_t x=0; x<m_width; ++x)
			for (size_t y=0; y<m_height; ++y) {
				mat(x,y) = 0;
				for (size_t c=0; c<m_nChannels; ++c)
					mat(x,y) += at<float32_t>(x,y,c) / m_nChannels;
			}
		break;
	case Image::F64BITS:
		for (size_t x=0; x<m_width; ++x)
			for (size_t y=0; y<m_height; ++y) {
				mat(x,y) = 0;
				for (size_t c=0; c<m_nChannels; ++c)
					mat(x,y) += at<float64_t>(x,y,c) / m_nChannels;
			}
		break;
	default:
		throw std::exception("image format not supported");
	}
}

#endif