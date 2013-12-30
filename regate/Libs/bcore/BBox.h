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

#ifndef _BBOX_H47945_INCLUDED_
#define _BBOX_H47945_INCLUDED_

#pragma once

#include "common.h"

class BBox {
public:
	Vector3	m_min, m_max;

public:
	BBox() : m_min(0,0,0), m_max(0,0,0) {};
	BBox(const Vector3& min, const Vector3& max) : m_min(min), m_max(max) {};
	BBox(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z) : m_min(min_x, min_y, min_z), m_max(max_x, max_y, max_z) {};

	inline Vector3 getCenter() const	{ return (m_min+m_max)/2.0; };
	inline Vector3 getSize() const		{ return m_max-m_min; };

	inline void add(const Vector3& v)	{
		if (v.x < m_min.x) m_min.x = v.x;
		if (v.y < m_min.y) m_min.y = v.y;
		if (v.z < m_min.z) m_min.z = v.z;
		if (v.x > m_max.x) m_max.x = v.x;
		if (v.y > m_max.y) m_max.y = v.y;
		if (v.z > m_max.z) m_max.z = v.z;
	};

	inline float getMaxDim() const {
		static Vector3 v;
		v = m_max-m_min;
		float f = (v.x>v.y)?v.x:v.y;
		f = (f>v.z)?f:v.z;
		return f;
	};

	inline bool contains(const BBox& bbox) const {
		if (m_min.x > bbox.m_min.x ||
			m_min.y > bbox.m_min.y ||
			m_min.z > bbox.m_min.z ||
			m_max.x < bbox.m_max.x ||
			m_max.y < bbox.m_max.y ||
			m_max.z < bbox.m_max.z)
			return false;
		return true;
	}

	void render();
};

#endif