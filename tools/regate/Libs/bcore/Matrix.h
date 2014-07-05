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

#ifndef _MATRIX_H45631_INCLUDED_
#define _MATRIX_H45631_INCLUDED_

#pragma once

#include "common.h"
#include <fstream>

/**
 * Matrix: a class to store general NxM matrices.
 *
 */
template <class T>
class Matrix
{
private:
	T *m_elems;
	size_t m_nRows, m_nCols;

public:
	Matrix() : m_elems(0), m_nRows(0), m_nCols(0) { };
	Matrix(size_t cols, size_t rows) : m_nRows(rows), m_nCols(cols) { m_elems = new T[rows*cols]; }
	Matrix(size_t cols, size_t rows, const T& val) : m_nRows(rows), m_nCols(cols) {
		m_elems = new T[rows*cols]; for (size_t i=0; i<rows*cols; ++i) m_elems[i]=val;
	}
	Matrix(const Matrix<T>& mat) : m_nRows(mat.m_nRows), m_nCols(mat.m_nCols) {
		m_elems=new T[mat.m_nRows*mat.m_nCols];
		for (size_t i=0; i<mat.m_nRows*mat.m_nCols; ++i) m_elems[i] = mat.m_elems[i];
	}
	~Matrix() { SAFE_DELETE_VEC(m_elems); };

	// methods to create a matrix
	inline void create(size_t cols, size_t rows) { ASSERT(rows>0 && cols>0); clear(); m_elems = new T[rows*cols]; m_nRows=rows; m_nCols=cols; }
	inline void clear() { SAFE_DELETE_VEC(m_elems); m_nRows=0; m_nCols=0; }
		   void createGaussian(int size);
	// NOTE: if size is odd, sum of elements will not be 0 in the following (dont use for e.g. derivatives):
		   void createStep(int size, double orientation);	// create a step, with negative at the negative oriented x-axis
		   void createStepBand(int size, int w, double orientation);	// create a step, with negative at the negative oriented x-axis

	inline size_t	numRows() const	{ return m_nRows; }
	inline size_t	numCols() const	{ return m_nCols; }

	// mean and variance for the *whole* matrix
	inline double mean() const;
	inline double variance() const;
	inline double maxval() const;
	inline double minval() const;
	inline T	sum() const;	// sum of all elements
	inline T	abssum() const;	// sum of absolute values of all elements

	// correlation and covariance
	inline Matrix<double> covariance() const;	// each column is an observation, each row a variable
	inline Matrix<double> correlation() const;	// each column is an observation, each row a variable
	inline void calcCorrCovar(Matrix<double> &corr, Matrix<double> &covar) const;	// do both in one pass, 
										// since they are interconnected, to avoid duplicate computations

	// unary operations:
	Matrix<T> transpose() const;

	// convolution
	template <class T>
	friend inline Matrix<T> convolution(const Matrix<T> &m1, const Matrix<T> &m2);
	template <class T>
	friend inline Matrix<Color> convolution(const Matrix<Color> &m1, const Matrix<T> &m2);
	template <class T>
	friend inline double local_convolution(const Matrix<T> &m, const Matrix<T> &filter, size_t x, size_t y);
	template <class T>
	friend inline Color local_convolution(const Matrix<Color> &m, const Matrix<T> &filter, size_t x, size_t y);

	template <class T>
	friend Matrix<T> operator * (const Matrix<T>& m1, const Matrix<T> &m2);

	// multiplication and division with scalars
	template <class T, class M>
	friend Matrix<T> operator * (const Matrix<T>& m, const M f);
	template <class T, class M>
	friend Matrix<T> operator * (const M f, const Matrix<T>& m);
	template <class T, class M>
	friend Matrix<T> operator / (const Matrix<T>& m, const M f);

	// access elements
	__forceinline		T& operator () (size_t i, size_t j)			{ ASSERT(i>=0&&i<numCols()&&j>=0&&j<numRows()); return m_elems[j*m_nCols + i]; }
	__forceinline const T& operator () (size_t i, size_t j) const	{ ASSERT(i>=0&&i<numCols()&&j>=0&&j<numRows()); return m_elems[j*m_nCols + i]; }

	inline Matrix<T>& operator = (const Matrix<T>& mat);
	
	// serialization operators
	template <class S>
	friend std::ostream& operator << (std::ostream& stream, const Matrix<S>& mat);
	template <class S>
	friend std::istream& operator >> (std::istream& stream, Matrix<S>& mat);
};

//-----------------------------

template <class T>
Matrix<T>& Matrix<T>::operator = (const Matrix<T>& mat) {
	clear();
	m_nRows = mat.m_nRows; m_nCols = mat.m_nCols;
	m_elems=new T[m_nRows*m_nCols];
//	memcpy(m_elems, mat.m_elems, sizeof(T)*m_nRows*m_nCols); // Dont use memcpy in case we have complex classes that need deep copy as T
	for (size_t i=0; i<m_nRows*m_nCols; ++i) m_elems[i] = mat.m_elems[i];
	return *this;
}

// serialization functions
template <class T>
std::ostream& operator << (std::ostream& stream, const Matrix<T>& mat)
{
	stream.write((char*)&mat.m_nRows, sizeof(mat.m_nRows));
	stream.write((char*)&mat.m_nCols, sizeof(mat.m_nCols));
	stream.write((char*)mat.m_elems, sizeof(T)*mat.m_nRows*mat.m_nCols);
	return stream;
}

template <class T>
std::istream& operator >> (std::istream& stream, Matrix<T>& mat)
{
	mat.clear();
	stream.read((char*)&mat.m_nRows, sizeof(mat.m_nRows));
	stream.read((char*)&mat.m_nCols, sizeof(mat.m_nCols));
	mat.m_elems = new T[mat.m_nRows*mat.m_nCols];
	stream.read((char*)mat.m_elems, sizeof(T)*mat.m_nRows*mat.m_nCols);
	return stream;
}

// THIS WORKS ONLY for simple numeric types
template <class T>
double Matrix<T>::mean() const
{
	double avg = 0;
	for (int i=0; i<m_nRows*m_nCols; ++i)
		avg += m_elems[i];
	return avg/(m_nRows * m_nCols);
}

template <class T>
double Matrix<T>::variance() const
{
	double avg = mean();
	double var = 0;
	for (size_t i=0; i<m_nRows*m_nCols; ++i)
		var += m_elems[i]*m_elems[i];
	return var/(m_nRows * m_nCols) - avg*avg;
}

template <class T>
double Matrix<T>::maxval() const
{
	ASSERT(m_nRows*m_nCols > 0);
	T maxv = m_elems[0];
	for (int i=1; i<m_nRows*m_nCols; ++i)
		if (m_elems[i] > maxv)
			maxv = m_elems[i];
	return maxv;
}

template <class T>
double Matrix<T>::minval() const
{
	ASSERT(m_nRows*m_nCols > 0);
	T minv = m_elems[0];
	for (int i=1; i<m_nRows*m_nCols; ++i)
		if (m_elems[i] < minv)
			minv = m_elems[i];
	return minv;
}

template <class T>
void Matrix<T>::createGaussian(int size)
{
	// create the matrix
	create(size, size);

	// calculate sigma for given kernel size
	double sigma = size / sqrt(8*log(2.0));

	// generate the gaussian
	for (int i=0; i<size; ++i)
		for (int j=0; j<size; ++j)
		{
			double x = i-(double)size/2;
			double y = j-(double)size/2;
			double val = 1/sqrt(sigma*sigma * 2*PI) * exp( -(x*x + y*y)/(2*sigma*sigma) );
			(*this)(i,j) = (T)val;
		}
}

template <class T>
void Matrix<T>::createStep(int size, double orientation)
{
	create(size, size);

	Vector2 dir(cos(-orientation), sin(-orientation));
	for (int x=0; x<size; ++x)
	for (int y=0; y<size; ++y)
	{
		Vector2 v(x-size/2+0.5, y-size/2+0.5);

		if (v.dot(dir) > 0)
			(*this)(x,y) = 1;
		else
			(*this)(x,y) = -1;
	}
}

template <class T>
void Matrix<T>::createStepBand(int size, int w, double orientation)
{
	create(size, size);

	Vector2 dir(cos(-orientation), sin(-orientation));
	Vector2 vdir(-dir.y, dir.x);
	for (int x=0; x<size; ++x)
	for (int y=0; y<size; ++y)
	{
		Vector2 v(x-size/2+0.5, y-size/2+0.5);

		if (fabs(v.dot(vdir)) > w) {
			(*this)(x,y) = 0;
			continue;
		}
		if (v.dot(dir) > 0)
			(*this)(x,y) = 1;
		else
			(*this)(x,y) = -1;
	}
}

template <class T>
Matrix<T> convolution(const Matrix<T> &m1, const Matrix<T> &m2)
{
	Matrix<T> result(m1.numCols(), m1.numRows());

	size_t m2_center_x = m2.numCols()/2;
	size_t m2_center_y = m2.numRows()/2;

	for (size_t i=0; i<result.numCols(); ++i)
		for (size_t j=0; j<result.numRows(); ++j)
		{
			T val = 0;
			for (int m=0; m<m2.numCols(); ++m) {
				// flipped column (x) index
				int mm = m2.numCols() - 1 - m;

				for (int n=0; n<m2.numRows(); ++n) {
					// flipped row (y) index
					int nn = m2.numRows() - 1 - n;

					int ii = i + (m - m2_center_x);
					int jj = j + (n - m2_center_y);

					// check bounds
					if (ii < 0 || ii >= m1.numCols() || jj<0 || jj>=m1.numRows())
						continue;

					val += m1(ii,jj)*m2(mm, nn);
				}
			}
			result(i,j) = val;
		}

	return result;
}

template <class T>
Matrix<Color> convolution(const Matrix<Color> &m1, const Matrix<T> &m2)
{
	Matrix<Color> result(m1.numCols(), m1.numRows());

	size_t m2_center_x = m2.numCols()/2;
	size_t m2_center_y = m2.numRows()/2;

	for (size_t i=0; i<result.numCols(); ++i)
		for (size_t j=0; j<result.numRows(); ++j)
		{
			Color val(0,0,0);
			for (int m=0; m<m2.numCols(); ++m) {
				// flipped row index
				int mm = m2.numCols() - 1 - m;

				for (int n=0; n<m2.numRows(); ++n) {
					// flipped column index
					int nn = m2.numRows() - 1 - n;

					int ii = i + (m - m2_center_x);
					int jj = j + (n - m2_center_y);

					// check bounds
					if (ii < 0 || ii >= m1.numCols() || jj<0 || jj>=m1.numRows())
						continue;

					val += m1(ii,jj)*m2(mm, nn);
				}
			}
			result(i,j) = val;
		}

	return result;
}

template <class T>
double local_convolution(const Matrix<T> &mat, const Matrix<T> &filter, size_t x, size_t y)
{
	double resp = 0.0;
	int fw = filter.numCols();
	int fh = filter.numRows();
	for (int fx=0; fx<fw; ++fx)
		for (int fy=0; fy<fh; ++fy) {
			int x2 = (int)x + fx - fw/2;
			int y2 = (int)y + fy - fh/2;
			if (x2<0 || x2>=mat.numCols() || y2<0 || y2>=mat.numRows())
				continue;
			resp += mat(x2,y2)*filter(fx, fy);
		}
	return resp;
}

template <class T>
Color local_convolution(const Matrix<Color> &mat, const Matrix<T> &filter, size_t x, size_t y)
{
	Color resp(0.0f, 0.0f, 0.0f);
	int fw = filter.numCols();
	int fh = filter.numRows();
	for (int fx=0; fx<fw; ++fx)
		for (int fy=0; fy<fh; ++fy) {
			int x2 = (int)x + fx - fw/2;
			int y2 = (int)y + fy - fh/2;
			if (x2<0 || x2>=mat.numCols() || y2<0 || y2>=mat.numRows())
				continue;
			resp += mat(x2,y2)*filter(fx, fy);
		}
	return resp;
}

template <class T>
T Matrix<T>::sum() const {
	T val = 0;
	for (size_t i=0; i<m_nRows*m_nCols; ++i)
		val += m_elems[i];
	return val;
}

template <class T>
T Matrix<T>::abssum() const {
	T val = 0;
	for (size_t i=0; i<m_nRows*m_nCols; ++i)
		val += abs(m_elems[i]);
	return val;
}

template <class T>
Matrix<double> Matrix<T>::covariance() const
{
	size_t nVars = numRows();
	size_t nSamples = numCols();

	// calculate mean and std for each variable
	std::vector<double> mean(nVars, 0);
	for (size_t i=0; i<nVars; ++i)
	{
		for (size_t s=0; s<nSamples; ++s)
			mean[i] += (*this)(s, i);
		mean[i] /= nSamples;
	}

	// calculate the covariance
	Matrix<double> cov(nVars, nVars, 0);
	for (size_t i=0; i<nVars; ++i)
		for (size_t j=i; j<nVars; ++j)
		{
			cov(i,j) = 0;
			for (size_t s=0; s<nSamples; ++s)
				cov(i,j) += ((*this)(s,i) - mean[i]) * ((*this)(s,j) - mean[j]);
			cov(i,j) /= nSamples;
			cov(j,i) = cov(i,j);
		}

	return cov;
}

template <class T>
Matrix<double> Matrix<T>::correlation() const
{
	size_t nVars = numRows();
	size_t nSamples = numCols();

	// calculate mean and std for each variable
	std::vector<double> mean(nVars, 0), var(nVars, 0);
	for (size_t i=0; i<nVars; ++i)
	{
		for (size_t s=0; s<nSamples; ++s) {
			T elem = (*this)(s, i);
			mean[i] += elem;
			var[i] += elem*elem;
		}
		mean[i] /= nSamples;
		var[i] = var[i]/nSamples - mean[i]*mean[i];

		Console::print("%f - %f\n", mean[i], var[i]);
	}

	// calculate the covariance and correlation
	Matrix<double> corr(nVars, nVars, 0);
	for (size_t i=0; i<nVars; ++i)
		for (size_t j=i; j<nVars; ++j)
		{
			corr(i,j) = 0;
			for (size_t s=0; s<nSamples; ++s)
				corr(i,j) += ((*this)(s,i) - mean[i]) * ((*this)(s,j) - mean[j]);
			corr(i,j) /= nSamples;

			// now, corr(i,j) holds the value of the covariance of i and j
			corr(i,j) /= sqrt(var[i]) * sqrt(var[j]);

			corr(j,i) = corr(i,j);
		}

	return corr;
}

template <class T>
void Matrix<T>::calcCorrCovar(Matrix<double> &corr, Matrix<double> &covar) const
{
	size_t nVars = numRows();
	size_t nSamples = numCols();

	// calculate mean and std for each variable
	std::vector<double> mean(nVars, 0), var(nVars, 0);
	for (size_t i=0; i<nVars; ++i)
	{
		for (size_t s=0; s<nSamples; ++s) {
			T elem = (*this)(s, i);
			mean[i] += elem;
			var[i] += elem*elem;
		}
		mean[i] /= nSamples;
		var[i] = var[i]/nSamples - mean[i]*mean[i];

		Console::print("%f - %f\n", mean[i], var[i]);
	}

	// calculate the covariance and correlation
	corr = Matrix<double>(nVars, nVars, 0);
	covar = Matrix<double>(nVars, nVars, 0);
	for (size_t i=0; i<nVars; ++i)
		for (size_t j=i; j<nVars; ++j)
		{
			// compute covariance
			covar(i,j) = 0;
			for (size_t s=0; s<nSamples; ++s)
				covar(i,j) += ((*this)(s,i) - mean[i]) * ((*this)(s,j) - mean[j]);
			covar(i,j) /= nSamples;

			// compute correlation
			corr(i,j) = covar(i,j) / (sqrt(var[i]) * sqrt(var[j]));

			corr(j,i) = corr(i,j);
			covar(j,i) = covar(i,j);
		}

	return corr;
}

template <class T>
Matrix<T> operator * (const Matrix<T> &m1, const Matrix<T> &m2)
{
	ASSERT(m1.numCols() == m2.numRows());
	Matrix<T> result(m2.numCols(), m1.numRows(), 0);

	for (size_t i=0; i<m2.numCols(); ++i)
		for (size_t j=0; j<m1.numRows(); ++j)
		{
			for (size_t k=0; k<m1.numCols(); ++k)
				result(i,j) += m1(k,j) * m2(i,k);
		}
	return result;
}

template <class T>
Matrix<T> Matrix<T>::transpose() const
{
	Matrix<T> result(numRows(), numCols());
	for (size_t i=0; i<numCols(); ++i)
		for (size_t j=0; j<numRows(); ++j)
			result(j,i) = (*this)(i,j);
	return result;
}

// multiplication and division with scalars
template <class T, class M>
Matrix<T> operator * (const Matrix<T>& m, const M f)
{
	Matrix<T> result(m.numCols(), m.numRows());
	for (size_t i=0; i<m.numCols(); ++i)
		for (size_t j=0; j<m.numRows(); ++j)
			result(i,j) = m(i,j) * f;
	return result;
}

template <class T, class M>
Matrix<T> operator * (const M f, const Matrix<T>& m)
{
	Matrix<T> result(m.numCols(), m.numRows());
	for (size_t i=0; i<m.numCols(); ++i)
		for (size_t j=0; j<m.numRows(); ++j)
			result(i,j) = m(i,j) * f;
	return result;
}

template <class T, class M>
Matrix<T> operator / (const Matrix<T>& m, const M f)
{
	Matrix<T> result(m.numCols(), m.numRows());
	for (size_t i=0; i<m.numCols(); ++i)
		for (size_t j=0; j<m.numRows(); ++j)
			result(i,j) = m(i,j) / f;
	return result;
}

#endif