/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of BeGUI library.
//
//    BeGUI is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    BeGUI is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with BeGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ViewportComponent.h"
#include "util.h"
#include "FrameWindow.h"

using namespace begui;

ViewportComponent::ViewportComponent() : m_bNavigationEnabled(false)
{
}

void ViewportComponent::create(int x, int y, int w, int h, const Viewport& vp, Functor0 &render_callback)
{
	setPos(x,y);
	setSize(w, h);
	m_viewport = vp;
	m_renderCallback = render_callback;

	// init trackball
	m_trackball.init();
	m_trackball.reshape(w, h);
}

void ViewportComponent::onUpdate()
{
}

void ViewportComponent::onRender()
{
	int w = getWidth();
	int h = getHeight();

	// correct the dimensions of the viewport
	Matrix4 mat;
	mat.loadGLMatrix(GL_MODELVIEW_MATRIX);
	Vector3 t = mat.getTranslation();
	int frameH = display::getHeight();
	m_viewport.setDimensions((int)t.x, frameH-int(t.y+h), w, h);

	// push matrices
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// push all attributes
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// setup the viewport
	m_viewport.set();

	// setup the local transformation
	if (m_bNavigationEnabled)
		m_trackball.setMatrix();

	// render the contents by calling the functor
	m_renderCallback();

	// pop matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// pop attrib bits
	glPopAttrib();

	// reset the original viewport
	FrameWindow::inst()->resetViewport();
}

bool ViewportComponent::onMouseDown(int x, int y, int button)
{
	if (m_bNavigationEnabled)
		m_trackball.mouseDown(x-Component::getLeft(), y-Component::getTop());

	return Component::onMouseDown(x,y,button);
}

bool ViewportComponent::onMouseMove(int x, int y, int prevx, int prevy)
{
	if (m_bNavigationEnabled)
		m_trackball.motion(x-Component::getLeft(), y-Component::getTop());

	return Component::onMouseMove(x,y,prevx, prevy);
}

bool ViewportComponent::onMouseUp(int x, int y, int button)
{
	if (m_bNavigationEnabled)
		m_trackball.mouseUp(x-Component::getLeft(), y-Component::getTop());

	m_lastClickPos = Vector2((float)(x-Component::getLeft()), (float)(y-Component::getTop()));

	return Component::onMouseUp(x,y,button);
}

void ViewportComponent::onKeyDown(int key)
{
	Component::onKeyDown(key);
}

void ViewportComponent::onKeyUp(int key)
{
	Component::onKeyUp(key);
}

bool ViewportComponent::isPtInside(int x, int y)
{
	if (x<m_left || x>m_right)
		return false;
	if (y<m_top || y>m_bottom)
		return false;
	return true;
}