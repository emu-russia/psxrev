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

double Image::m_filterLUT[FILTERS_NUM][LUT_SAMPLES];
bool g_bImageLUTInited = false;

void Image::precomputeLUTs()
{
	for (int i=0; i<LUT_SAMPLES; ++i)
	{
		// box filter lookup table
		m_filterLUT[0][i] = 1;

		// linear filter
		m_filterLUT[1][i] = 1.0 - (double)i/(LUT_SAMPLES-1);

		// cubic filter
		double x = 2.0*i/LUT_SAMPLES;
		double a = -0.5;
		if (x < 1.0)
			m_filterLUT[2][i] = (a+2)*x*x*x - (a+3)*x*x + 1;
		else
			m_filterLUT[2][i] = a*x*x*x - 5*a*x*x + 8*a*x - 4*a;
	}
}

void Image::interpolate(double x, double y, Filter filter, double filter_stretch, double out[3])
{
	if (!g_bImageLUTInited) {
		Image::precomputeLUTs();
		g_bImageLUTInited = true;
	}

	double flt_half_w_native;
	switch (filter)
	{
	case BOX: flt_half_w_native = 0.5; break;
	case LINEAR: flt_half_w_native = 1; break;
	case CUBIC: flt_half_w_native = 2; break;
	default: ASSERT(0);	// unknown filter type
	}

	double filter_half_w = flt_half_w_native*filter_stretch;
	double LUT_scale = (LUT_SAMPLES-1)/filter_half_w;
	double cutoff_scale = (1.0/filter_stretch)*(1.0/filter_stretch);

	int colorplanes = getChannelsNum();

	for (int c=0; c<colorplanes; ++c)
	{
		double y_temp=0;
		for (int j=int(y - filter_half_w)+1; j<=int(y+filter_half_w); ++j)
		{
			double x_temp = 0;
			for (int i=int(x - filter_half_w)+1; i<=int(x+filter_half_w); ++i)
			{
				if (i<0 || i>=(int)m_width || j<0 || j>=(int)m_height)
					x_temp += (*this)((size_t)x,(size_t)y)[c] * m_filterLUT[filter][(size_t)((int)fabs((x - (double)i)*LUT_scale))];
				else
					x_temp += (*this)(i,j)[c] * m_filterLUT[filter][(size_t)((int)fabs((x - (double)i)*LUT_scale))];
			}
			y_temp += x_temp*m_filterLUT[filter][(int)fabs((y - j)*LUT_scale)];
		}

		out[c] = cutoff_scale*y_temp;// * 1.0/(1.0 - excluded);
		if (out[c] > 255)
			out[c] = 255;
		if (out[c] < 0)
			out[c] = 0;
	}
}

void Image::resize(double scale, Filter filter, double filter_stretch)
{
	ASSERT(scale > 0.0);
	ASSERT(filter>=0 && filter<FILTERS_NUM);
	ASSERT(filter_stretch > 0.0);

	if (m_width==0 || m_height==0)
		return;

	// allocate memory for the new data
	int w = (int)(scale*m_width);
	int h = (int)(scale*m_height);
	if (w < 1) w=1;
	if (h < 1) h=1;
	int colorplanes = getChannelsNum();
	std::vector<unsigned char> data(w*h*colorplanes);

	// interpolate to create the new image
	int lastUpd = 0;
	for (int i=0; i<w; ++i)
	{
		for (int j=0; j<h; ++j)
		{
			double out[3];
			double x = (double)i*m_width/w;
			double y = (double)j*m_height/h;
			if (scale < 1.0) {
				x = (int)x;
				y = (int)y;
			}
			interpolate(x, y, filter, filter_stretch, out);

			for (int c=0; c<colorplanes; ++c)
				data[colorplanes*(j*w + i) + c] = (unsigned char)out[c];
		}
	}

	// replace the original data
	m_data = data;
	m_width = w;
	m_height = h;
}

void Image::resize(size_t w, size_t h, Filter filter, double filter_stretch)
{
	ASSERT(w>0 && h>0);
	ASSERT(filter>=0 && filter<FILTERS_NUM);
	ASSERT(filter_stretch > 0.0);

	if (m_width==0 || m_height==0)
		return;

	// allocate memory for the new data
	size_t colorplanes = getChannelsNum();
	std::vector<unsigned char> data(w*h*colorplanes);

	// interpolate to create the new image
	int lastUpd = 0;
	for (size_t i=0; i<w; ++i)
	{
		for (size_t j=0; j<h; ++j)
		{
			double out[3];
			double x = (double)i*m_width/w;
			double y = (double)j*m_height/h;
			if (w < m_width)
				x = (int)x;
			if (h < m_height)
				y = (int)y;
			interpolate(x, y, filter, filter_stretch, out);

			for (size_t c=0; c<colorplanes; ++c)
				data[colorplanes*(j*w + i) + c] = (unsigned char)out[c];
		}
	}

	// replace the original data
	m_data = data;
	m_width = w;
	m_height = h;
}