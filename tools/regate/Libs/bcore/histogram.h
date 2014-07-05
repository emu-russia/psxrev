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

#ifndef _HISTOGRAM_H45631_INCLUDED_
#define _HISTOGRAM_H45631_INCLUDED_

#pragma once

#include "common.h"
#include <fstream>

template <class T>
class Histogram
{
protected:
	std::vector<int>	m_bins;
	T					m_min, m_max;

public:
	Histogram() { };
	Histogram(int nBins, const T& min, const T& max) { m_bins = std::vector<int>(nBins, 0); m_min=min; m_max=max; }
	Histogram(const Histogram& h) { m_bins = h.m_bins; m_min=h.m_min; m_max=h.m_max; }
	virtual ~Histogram() { }

	virtual void	create(int nBins, const T& min, const T& max)		{ m_bins = std::vector<int>(nBins, 0); m_min=min; m_max=max; }
	size_t	getBinsNum() const		{ return m_bins.size(); }
	T		getRangeMin() const		{ return m_min; }
	T		getRangeMax() const		{ return m_max; }
	virtual void	add(const T& val)		{ ASSERT(m_min!=m_max); int bin = (int)((m_bins.size()-1) * (val - m_min)/(m_max - m_min)); m_bins[bin]++; }
	int		getBin(size_t bin) const			{ ASSERT(bin<m_bins.size()); return m_bins[bin]; }
	int&	getBin(size_t bin)					{ ASSERT(bin<m_bins.size()); return m_bins[bin]; }

	int  operator [] (const T& val) const	{ ASSERT(m_min!=m_max); int bin = (int)((m_bins.size()-1) * (val - m_min)/(m_max - m_min)); ASSERT(bin>=0 && bin<m_bins.size()); return m_bins[bin]; }
	int& operator [] (const T& val)			{ ASSERT(m_min!=m_max); int bin = (int)((m_bins.size()-1) * (val - m_min)/(m_max - m_min)); ASSERT(bin>=0 && bin<m_bins.size()); return m_bins[bin]; }

	Histogram<T> operator - (const Histogram<T>& hist) const	{
		ASSERT(m_bins.size() == hist.m_bins.size());
		Histogram<T> h(*this);
		for (size_t i=0; i<m_bins.size(); ++i) h.m_bins[i] = abs(m_bins[i] - hist.m_bins[i]);
		return h;
	}

	friend double dif_norm1(const Histogram<T>& h1, const Histogram<T>& h2) {
		ASSERT(h1.m_bins.size() == h2.m_bins.size());
		double dif = 0;
		int sum1 = 0, sum2 = 0;
		for (size_t i=0; i<h1.m_bins.size(); ++i) {
			sum1 += h1.m_bins[i];
			sum2 += h2.m_bins[i];
		}
		if (sum1 == 0 || sum2 == 0)
			return 1;
		for (size_t i=0; i<h1.m_bins.size(); ++i)
			dif += fabs((double)h1.m_bins[i]/sum1 - (double)h2.m_bins[i]/sum2);
		return dif/2;
	}

	// file I/O
	friend std::istream& operator >> (std::istream& stream, Histogram<T> &hist) {
		int nBins = 0;
		stream.read((char*)&nBins, sizeof(nBins));
		hist.m_bins.resize(nBins);
		stream.read((char*)&hist.m_bins[0], sizeof(int)*nBins);
		stream.read((char*)&hist.m_min, sizeof(T));
		stream.read((char*)&hist.m_max, sizeof(T));
		return stream;
	}
	friend std::ostream& operator << (std::ostream& stream, Histogram<T> &hist) {
		int nBins = (int)hist.m_bins.size();
		stream.write((char*)&nBins, sizeof(nBins));
		stream.write((char*)&hist.m_bins[0], sizeof(int)*nBins);
		stream.write((char*)&hist.m_min, sizeof(T));
		stream.write((char*)&hist.m_max, sizeof(T));
		return stream;
	}
};

template <class T>
class ProbHistogram : public Histogram<T>
{
protected:
	int	m_nTotalValues;

public:
	ProbHistogram() : m_nTotalValues(0) { }
	ProbHistogram(const Histogram<T>& h) : m_nTotalValues(0) {
		m_min=h.getRangeMin(); m_max=h.getRangeMax();
		m_bins.resize(h.getBinsNum());
		for (size_t i=0; i<m_bins.size(); ++i) {
			m_bins[i] = h.getBin(i);
			m_nTotalValues += m_bins[i];
		}
	}

	void create(int nBins, const T& min, const T& max)		{ Histogram::create(nBins, min, max); m_nTotalValues = 0; }
	void add(const T& val)		{ Histogram::add(val); m_nTotalValues++; }

	double probOfValue(const T& val) const	{
		ASSERT(m_min!=m_max);
		int bin = (int)((m_bins.size()-1) * (val - m_min)/(m_max - m_min));
		return (double)m_bins[bin]/m_nTotalValues;
	}
	double probOfBin(size_t bin) const {
		return (double)m_bins[bin]/m_nTotalValues;
	}
};

template <class T>
class MulticlassPHistogram
{
protected:
	typedef Histogram<T> tHistogram;
	std::vector<tHistogram> m_histograms;	// an extra histogram for total count of each bin
	std::vector<int> m_numInstPerClass;
	int m_nTotalInst;

public:
	MulticlassPHistogram() : m_nTotalInst(0) { };
	MulticlassPHistogram(int nBins, int nClasses, const T& min, const T& max) : m_nTotalInst(0) {
		m_histograms.resize(nClasses+1); for (int i=0; i<nClasses+1; ++i) m_histograms[i].create(nBins, min, max);
		m_numInstPerClass = std::vector<int>(nClasses, 0);
	}

	void create(int nBins, int nClasses, const T& min, const T& max) {
		m_histograms.resize(nClasses+1);
		for (int i=0; i<nClasses+1; ++i) m_histograms[i].create(nBins, min, max);
		m_numInstPerClass = std::vector<int>(nClasses, 0);
		m_nTotalInst = 0;
	}

	void add(const T& val, int class_id)	{
		m_histograms[class_id].add(val);
		m_histograms.back().add(val);	// last histogram holds the total count of values
		m_numInstPerClass[class_id]++;
		m_nTotalInst++;
	}

	double probOfValue(const T& val, int class_id) const	{
		//divide the num of values for this class by the total num of values
	//	return (double)(m_histograms[class_id][val])/m_numInstPerClass[class_id];// / (m_histograms.back()[val] / m_numInstPerClass;

/*		double pclass_val = Pclass_val(val, class_id);
		if (pclass_val != 0)
			return Pclass(class_id) * Pval_class(val, class_id) / pclass_val;
		return 0;*/
		return Pval_class(val, class_id)/ Pval(val);
//		double pval = Pval(val);
//		if (pval != 0)
//			return Pclass(class_id) * Pval_class(val, class_id) / pval;
		return 0;
	}
	double Pval_class(const T& val, int class_id) const {
		if (m_numInstPerClass[class_id] > 0)
			return (double)(m_histograms[class_id][val])/m_numInstPerClass[class_id];	// P(val|S)
		return 0;
	}
	
	double Pclass(int class_id) const {
		return (double)m_numInstPerClass[class_id]/m_nTotalInst;	// P(S)
	}

	double Pclass_val(const T& val, int class_id) const {
		if (m_histograms.back()[val] > 0)
			return (double)m_histograms[class_id][val] / m_histograms.back()[val];	// P(S|val)
		return 0;
	}

	double Pval(const T& val) const {
		double num_val = 0;
		for (int i=0; i<m_histograms.size()-1; ++i)
			num_val += m_histograms[i][val];
		return num_val / m_nTotalInst;
	}

	// access per-class histograms
	int		getNumClasses() const			{ return (int)m_histograms.size(); }
		  Histogram<T>& operator [] (int classid)		{ return m_histograms[classid]; }
	const Histogram<T>& operator [] (int classid) const	{ return m_histograms[classid]; }

	// file I/O
	friend std::istream& operator >> (std::istream& stream, MulticlassPHistogram<T> &hist) {
		int nHists = 0;
		int nClasses = 0;
		stream.read((char*)&nHists, sizeof(nHists));
		stream.read((char*)&nClasses, sizeof(nClasses));
		hist.m_histograms.resize(nHists);
		for (int i=0; i<nHists; ++i)
			stream >> hist.m_histograms[i];
		hist.m_numInstPerClass.resize(nClasses);
		stream.read((char*)&hist.m_numInstPerClass[0], sizeof(int)*nClasses);
		hist.m_nTotalInst = 0;
		for (int i=0; i<nClasses; ++i) hist.m_nTotalInst += hist.m_numInstPerClass[i];
		return stream;
	}
	friend std::ostream& operator << (std::ostream& stream, MulticlassPHistogram<T> &hist) {
		int nHists = (int)hist.m_histograms.size();
		int nClasses = (int)hist.m_numInstPerClass.size();
		stream.write((char*)&nHists, sizeof(nHists));
		stream.write((char*)&nClasses, sizeof(nClasses));
		for (int i=0; i<nHists; ++i)
			stream << hist.m_histograms[i];
		stream.write((char*)&hist.m_numInstPerClass[0], sizeof(int)*nClasses);
		return stream;
	}
};

#endif