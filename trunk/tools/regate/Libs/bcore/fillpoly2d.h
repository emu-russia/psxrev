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

#ifndef _FILLPOLY2D_H45631_INCLUDED_
#define _FILLPOLY2D_H45631_INCLUDED_

#pragma once

#include <algorithm>
#include <vector>
#include "LineLineIntersect.h"

namespace fillpoly
{
	struct edge
	{
		Vector2 v1, v2;	// sorted, v1.y < v2.y
	};

	inline bool edge_less_y(const edge& e1, const edge& e2)
	{
		return (e1.v1.y < e2.v1.y);
	}
}

template <class T>
void fillpoly2d(const std::vector<Vector2> &vertices, int w, int h, T& renderer)
{
	std::vector<fillpoly::edge> edges;
	for (size_t i=0; i<vertices.size(); ++i)
	{
		// create a new edge, with v1.y < v2.y
		fillpoly::edge e;
		if (i==0)
			e.v1 = vertices.back();
		else
			e.v1 = vertices[i-1];
		e.v2 = vertices[i];
		if (e.v1.y > e.v2.y)
		{
			Vector2 v = e.v1;
			e.v1 = e.v2;
			e.v2 = v;
		}

		// add the edge
		edges.push_back(e);
	}

	// sort edges at increasing y-values
//	std::sort(edges.begin(), edges.end(), edge_less_y);

	// for each scanline
	int curEdge = 0;
	for (int y=0; y<h; ++y)
	{
		// find the edges that intersect this scanline
		bool bIntersect = false;
		std::list<int> scanline;	// store the intersection points here
		for (size_t ei=0; ei<edges.size(); ++ei)
		{
			fillpoly::edge &e = edges[ei];
			// if this edge ends before this scanline,
			// and we still havent seen an edge that intersects
			// the scanline, next time start the search after this one
			if (e.v2.y <= y) {
		//		if (!bIntersect)
		//			curEdge = ei+1;
				continue;
			}
			// if this edge starts after our scanline, we shouldnt
			// look at any more edges
			if (e.v1.y > y)
				continue;//break;

			// this edge intersects the scanline
			bIntersect = true;

			// find the intersection point
			Vector2 pt;
			bool bI = line_line_intersect(e.v1, e.v2, Vector2(0,y), Vector2(w-1,y), &pt);
			ASSERT(bI);
			if (pt.x==0)
				continue;

			scanline.push_back((int)pt.x);
		}

		// sort the intersection pts at increasing x
		scanline.sort();

		// now fill the scanline
		std::list<int>::iterator it = scanline.begin();
		while (1)
		{
			if (it == scanline.end())
				break;
			int x1 = *it;
			++it;
			if (it == scanline.end())
				break;
			int x2 = *it;
			++it;

			for (int x=x1; x<x2; ++x)
				renderer.draw(Vector2(x,y));
		}
	}
}

#endif