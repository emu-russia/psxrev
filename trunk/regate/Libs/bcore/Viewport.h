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

#ifndef _VIEWPORT_H45631_INCLUDED_
#define _VIEWPORT_H45631_INCLUDED_

#pragma once

#include "common.h"
#include "BBox.h"

class Viewport
{
public:
	enum ProjType {
		ISOMETRIC = 0,
		PERSPECTIVE
	};

private:
	Vector2		m_pan;
	float		m_zoom;

	ProjType	m_projType;
	int			m_x, m_y;
	int			m_width, m_height;
	float		m_ncp, m_fcp, m_fov;
	float		m_aspectRatio;
	float		m_orthoScale;

public:
	Viewport();

	void setDimensions(int x, int y, int w, int h);
	void setOrthoProj(float ncp=0.0f, float fcp=1.0f, float scale = 1.0f, float aspectRatio = 1.0f);
	void setPerspectiveProj(float ncp, float fcp, float fov, float aspectRatio = 1.0f);
	void set();	// to OpenGL

	void setLookAtBox(const BBox& box, const Vector3& up = Vector3(0,1,0));
	void zoom(float f);
	void pan(const Vector2& t);
	void rotate(const Vector2& r);

	void setMatrix();
};

#endif