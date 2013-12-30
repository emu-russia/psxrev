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

#include "Component.h"
#include "util.h"

using namespace begui;

Component::Component() : m_left(0), m_right(0), m_top(0), m_bottom(0),
	m_pParent(0),
	m_bAlwaysOnTop(false),
	m_bVisible(true),
	m_bHasMouseFocus(false),
	m_bHasKeybFocus(false),
	m_bActive(false),
	m_bFixedZOrder(false),
	m_bEnabled(true)
{
}

// Coordinate transformation
Vector2i	Component::worldToLocal(const Vector2i& v) const
{
	Component *parent = m_pParent;
	Vector2i pos(v.x-m_left, v.y-m_top);
	while (parent) {
		pos.x -= parent->m_left;
		pos.y -= parent->m_top;
		parent = parent->m_pParent;
	}
	return pos;
}

Vector2i	Component::parentToLocal(const Vector2i& v) const
{
	return Vector2i(v.x-m_left, v.y-m_top);
}

Vector2i Component::localToWorld(const Vector2i& v) const
{
	Component *parent = m_pParent;
	Vector2i pos(v.x+m_left, v.y+m_top);
	while (parent) {
		pos.x += parent->m_left;
		pos.y += parent->m_top;
		parent = parent->m_pParent;
	}
	return pos;
}

Vector2i Component::localToParent(const Vector2i& v) const
{
	return Vector2i(v.x+m_left, v.y+m_top);
}

void Component::drawBorderedQuad(int l, int t, int r, int b,		// position to appear on screen
								 int lB, int tB, int rB, int bB,				// border positions
						float texL, float texLB, float texR, float texRB,	// left to right positions in tex
						float texT, float texTB, float texB, float texBB)	// top to bottom
{	
	glBegin(GL_QUADS);
		// top-left corner
		glTexCoord2f(texL, texT);	glVertex3f((float)l, (float)t, 0);
		glTexCoord2f(texLB, texT);	glVertex3f((float)lB, (float)t, 0);
		glTexCoord2f(texLB, texTB);	glVertex3f((float)lB, (float)tB, 0);
		glTexCoord2f(texL, texTB);	glVertex3f((float)l, (float)tB, 0);
		
		// top border
		glTexCoord2f(texLB, texT);	glVertex3f((float)lB, (float)t, 0);
		glTexCoord2f(texRB, texT);	glVertex3f((float)rB, (float)t, 0);
		glTexCoord2f(texRB, texTB);	glVertex3f((float)rB, (float)tB, 0);
		glTexCoord2f(texLB, texTB);	glVertex3f((float)lB, (float)tB, 0);

		// top-right corner
		glTexCoord2f(texRB, texT);	glVertex3f((float)rB, (float)t, 0);
		glTexCoord2f(texR, texT);	glVertex3f((float)r, (float)t, 0);
		glTexCoord2f(texR, texTB);	glVertex3f((float)r, (float)tB, 0);
		glTexCoord2f(texRB, texTB);	glVertex3f((float)rB, (float)tB, 0);

		// left border
		glTexCoord2f(texL, texTB);	glVertex3f((float)l, (float)tB, 0);
		glTexCoord2f(texLB, texTB);	glVertex3f((float)lB, (float)tB, 0);
		glTexCoord2f(texLB, texBB);	glVertex3f((float)lB, (float)bB, 0);
		glTexCoord2f(texL, texBB);	glVertex3f((float)l, (float)bB, 0);

		// center area
		glTexCoord2f(texLB, texTB);	glVertex3f((float)lB, (float)tB, 0);
		glTexCoord2f(texRB, texTB);	glVertex3f((float)rB, (float)tB, 0);
		glTexCoord2f(texRB, texBB);	glVertex3f((float)rB, (float)bB, 0);
		glTexCoord2f(texLB, texBB);	glVertex3f((float)lB, (float)bB, 0);

		// right border
		glTexCoord2f(texRB, texTB);	glVertex3f((float)rB, (float)tB, 0);
		glTexCoord2f(texR, texTB);	glVertex3f((float)r, (float)tB, 0);
		glTexCoord2f(texR, texBB);	glVertex3f((float)r, (float)bB, 0);
		glTexCoord2f(texRB, texBB);	glVertex3f((float)rB, (float)bB, 0);

		// bottom-left corner
		glTexCoord2f(texL, texBB);	glVertex3f((float)l, (float)bB, 0);
		glTexCoord2f(texLB, texBB);	glVertex3f((float)lB, (float)bB, 0);
		glTexCoord2f(texLB, texB);	glVertex3f((float)lB, (float)b, 0);
		glTexCoord2f(texL, texB);	glVertex3f((float)l, (float)b, 0);

		// bottom border
		glTexCoord2f(texLB, texBB);	glVertex3f((float)lB, (float)bB, 0);
		glTexCoord2f(texRB, texBB);	glVertex3f((float)rB, (float)bB, 0);
		glTexCoord2f(texRB, texB);	glVertex3f((float)rB, (float)b, 0);
		glTexCoord2f(texLB, texB);	glVertex3f((float)lB, (float)b, 0);

		// bottom-right corner
		glTexCoord2f(texRB, texBB);	glVertex3f((float)rB, (float)bB, 0);
		glTexCoord2f(texR, texBB);	glVertex3f((float)r, (float)bB, 0);
		glTexCoord2f(texR, texB);	glVertex3f((float)r, (float)b, 0);
		glTexCoord2f(texRB, texB);	glVertex3f((float)rB, (float)b, 0);
	glEnd();
}

void Component::getFocus()
{
	getMouseFocus();
	getKeybFocus();
}

void Component::releaseFocus()
{
	releaseMouseFocus();
	releaseKeybFocus();
}

void Component::getMouseFocus()
{
	m_bHasMouseFocus = true;
}

void Component::getKeybFocus()
{
	m_bHasKeybFocus = true;
}

void Component::releaseMouseFocus()
{
	m_bHasMouseFocus = false;
}

void Component::releaseKeybFocus()
{
	m_bHasKeybFocus = false;
}

bool Component::onMouseDown(int x, int y, int button)
{
	return true;
}

bool Component::onMouseMove(int x, int y, int prevx, int prevy)
{
	return true;
}

bool Component::onMouseUp(int x, int y, int button)
{
	return true;
}

void Component::onKeyDown(int key)
{
}

void Component::onKeyUp(int key)
{
}

void Component::onMouseWheel(int zDelta)
{
}

bool Component::isPtInside(int x, int y)
{
	if (x<m_left || x>m_right || y<m_top || y>m_bottom)
		return false;
	return true;
}

void Component::frameRender()
{
	// transform the parent coordinate system to the local one
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((float)m_left, (float)m_top, 0);
	
	// Render the component
	onRender();

	// Reset the coordinate system
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Component::frameUpdate()
{
	onUpdate();
}

void Component::drawImage(ResourceManager::ImageRef &image, int x, int y, int w, int h)
{
	ASSERT(image.m_texture);
	image.m_texture->set();

	if (w <= 0)
		w = image.m_width;
	if (h <= 0)
		h = image.m_height;
	glBegin(GL_QUADS);
		glTexCoord2f(image.m_topLeft.x, image.m_topLeft.y);
		glVertex3f((float)x, (float)y, 0);
		glTexCoord2f(image.m_bottomRight.x, image.m_topLeft.y);
		glVertex3f((float)(x+w), (float)y, 0);
		glTexCoord2f(image.m_bottomRight.x, image.m_bottomRight.y);
		glVertex3f((float)(x+w), (float)(y+h), 0);
		glTexCoord2f(image.m_topLeft.x, image.m_bottomRight.y);
		glVertex3f((float)x, (float)(y+h), 0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void Component::drawImageWtBorders(begui::ResourceManager::ImageRef &image, int x, int y, 
								   int w, int h, const Rect<int> &resizable_area)
{
	ASSERT(image.m_texture);
	image.m_texture->set();

	if (w <= 0)
		w = image.m_width;
	if (h <= 0)
		h = image.m_height;

	int intern_left = x+resizable_area.left;
	int intern_right = x+w - (image.m_width - resizable_area.right);
	int intern_top = y+resizable_area.top;
	int intern_bottom = y+h - (image.m_height - resizable_area.bottom);

	drawBorderedQuad(x, y, x+w, y+h, 
		intern_left, intern_top, intern_right, intern_bottom,
		image.m_topLeft.x, image.m_topLeft.x + (float)resizable_area.left/image.m_texture->getWidth(),
		image.m_bottomRight.x, image.m_topLeft.x + (float)resizable_area.right/image.m_texture->getWidth(),
		image.m_topLeft.y, image.m_topLeft.y + (float)resizable_area.top/image.m_texture->getHeight(),
		image.m_bottomRight.y, image.m_topLeft.y + (float)resizable_area.bottom/image.m_texture->getHeight());
	
	glDisable(GL_TEXTURE_2D);
}

float Component::getHierarchyAlpha() const
{
	float alpha = m_alpha;
	Component *pParent = m_pParent;
	while (pParent) {
		alpha = alpha*pParent->m_alpha;
		pParent = pParent->getParent();
	}
	return alpha;
}

void Component::setAlpha(float alpha)
{
	m_alpha = alpha;
}