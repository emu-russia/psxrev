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

#include "Container.h"

using namespace begui;

Container::Container() : m_pActiveComponent(0), m_pModalComponent(0)
{
}

Container::~Container()
{
}

void Container::frameUpdate()
{
	onUpdate();

	// update children too
	for (size_t i=0; i<m_children.size(); ++i)
		m_children[i]->frameUpdate();

	if (m_pModalComponent && m_pModalComponent->isVisible())
		m_pModalComponent->frameUpdate();
}

void Container::frameRender()
{
	// Change the coordinate system to the local one
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((float)m_left, (float)m_top, 0);
	
	// Render the container itself
	onRender();
	
	// render children
	std::vector<Component*> ontop;
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->isVisible())
		{
			m_children[i]->frameRender();
		}
	}

	// show the modal component, if any
	if (m_pModalComponent && m_pModalComponent->isVisible())
	{
		glEnable(GL_BLEND);
		glColor4f(0,0,0, 0.5f);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f((float)getWidth(), 0);
			glVertex2f((float)getWidth(), (float)getHeight());
			glVertex2f(0, (float)getHeight());
		glEnd();

		m_pModalComponent->frameRender();
	}

	// Reset the coordinate system
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

bool Container::onMouseDown(int x, int y, int button)
{
	// Transform coordinates.
	Vector2i lP = parentToLocal(Vector2i(x,y));

	// handle modal components
	if (m_pModalComponent && m_pModalComponent->isVisible()) {
		return m_pModalComponent->onMouseDown(lP.x, lP.y, button);
	}

	// select active component
	// traversal is done in z-order (starting from the end of the
	// list where the top child is) in order to eliminate problems
	// due to overlapping
	int activeComponent = -1;
	for (int i=(int)m_children.size()-1; i>=0; --i)
	{
		if (m_children[i]->isVisible())
		{
			if (m_children[i]->isPtInside(lP.x, lP.y))
			{
				activeComponent = i;
				break;
			}
		}
	}

	// Change the current active component
	Component *pOldActive = m_pActiveComponent;
	m_pActiveComponent = (activeComponent>=0)?m_children[activeComponent]:0;

	// Deactive the previous active component
	if (pOldActive && pOldActive != m_pActiveComponent)
	{
		pOldActive->releaseFocus();
		pOldActive->onDeactivate();
	}

	if (m_pActiveComponent)
	{
		// Bring the new active component on top (method respects
		// fixedZOrder flag)
		bringChildToFront(activeComponent);

		// Activate the new component
		m_pActiveComponent->onActivate();

		// switch focus from this component to the child
		releaseFocus();
		m_pActiveComponent->getFocus();

		// get the local coordinates inside the child component:
		m_pActiveComponent->onMouseDown(lP.x, lP.y, button);

		return true;
	}
	else
	{
		// Custom mouse handling
		onMouseDownEx((int)lP.x, (int)lP.y);
	}

	return false;
}

bool Container::onMouseMove(int x, int y, int prevx, int prevy)
{
	Vector2i lP = parentToLocal(Vector2i(x,y));
	Vector2i lPrevP = parentToLocal(Vector2i(prevx,prevy));

	bool bHandled = false;

	// handle modal components
	if (m_pModalComponent && m_pModalComponent->isVisible()) {
		return m_pModalComponent->onMouseMove(lP.x, lP.y, lPrevP.x, lPrevP.y);
	}

	// call mouseMove of active component
	// (without updating the active component)
	if (m_pActiveComponent)
	{
		// Transform coordinates.
		if (m_pActiveComponent->hasMouseFocus())
		{
			m_pActiveComponent->onMouseMove(lP.x, lP.y, lPrevP.x, lPrevP.y);
			bHandled = true;
			return true;
		}
	}

	// call mousemove for all children (other than the active component)
	for (int i=(int)m_children.size()-1; i>=0; --i)
	{
		if (m_children[i] == m_pActiveComponent && m_pActiveComponent->hasMouseFocus())
			continue;
		if (m_children[i]->isPtInside(lP.x, lP.y) || m_children[i]->isPtInside(lPrevP.x, lPrevP.y)) {
			m_children[i]->onMouseMove(lP.x, lP.y, lPrevP.x, lPrevP.y);
			bHandled = true;
		}
	}

	// custom handling
	if (!bHandled)
		onMouseMoveEx(lP.x, lP.y, lPrevP.x, lPrevP.y);

	return false;
}

bool Container::onMouseUp(int x, int y, int button)
{
	Vector2i lP = parentToLocal(Vector2i(x,y));

	// handle modal components
	if (m_pModalComponent && m_pModalComponent->isVisible()) {
		return m_pModalComponent->onMouseUp(lP.x, lP.y, button);
	}

	// call mouseUp of active component
	// (without updating the active component)
	if (m_pActiveComponent)
	{
		m_pActiveComponent->onMouseUp(lP.x, lP.y, button);
		if (m_pActiveComponent)	// check again, mouse up could have changed that
			m_pActiveComponent->releaseMouseFocus();
		return true;
	}
	else
	{
		// custom handling
		onMouseUpEx(lP.x, lP.y);
	}

	return false;
}

void Container::onKeyDown(int key)
{
	// handle modal components
	if (m_pModalComponent && m_pModalComponent->isVisible()) {
		m_pModalComponent->onKeyDown(key);
		return;
	}

	// if key is tab, change the active component
	// to the next in the tab order
	//TODO

	if (m_pActiveComponent)
	{
		m_pActiveComponent->onKeyDown(key);
	}
}

void Container::onKeyUp(int key)
{
	// handle modal components
	if (m_pModalComponent && m_pModalComponent->isVisible()) {
		m_pModalComponent->onKeyUp(key);
		return;
	}

	if (m_pActiveComponent)
	{
		m_pActiveComponent->onKeyUp(key);
	}
}

void Container::onMouseWheel(int zDelta)
{
	// handle modal components
	if (m_pModalComponent && m_pModalComponent->isVisible()) {
		m_pModalComponent->onMouseWheel(zDelta);
		return;
	}

	if (m_pActiveComponent)
	{
		m_pActiveComponent->onMouseWheel(zDelta);
	}
}

void Container::bringToFront(Component *pC)
{
	ASSERT(pC);
	int id = findChildId(pC);
	bringChildToFront(id);
}

void Container::bringChildToFront(int id)
{
	ASSERT(id >= 0 && id < (int)m_children.size());

	Component *comp = m_children[id];
	if (comp->hasFixedZOrder())
		return;

	// Change the order in the children array to bring the child to the top
	// (top child is the last in the array)
	// respect alwaysOnTop components.
	int sz = (int)m_children.size();
	int i=-1;
	for (i=id; i<sz-1; ++i)
	{
		if (m_children[i+1]->isAlwaysOnTop())
			break;
		m_children[i] = m_children[i+1];
	}
	m_children[i] = comp;
}

void Container::addComponent(Component *pC)
{
	ASSERT(pC);

	// check if the component is already added to this container
	if (contains(pC))
		return;

	// top child is the last in the array. Respect alwaysOnTop
	// flags when adding the child
	if (m_children.size() == 0)
		m_children.push_back(pC);
	else
	{
		int i = (int)m_children.size()-1;
		if (!pC->isAlwaysOnTop())
		{
			for (; i>=0; --i)
			{
				if (!m_children[i]->isAlwaysOnTop())
					break;
			}
		}
		m_children.insert(m_children.begin()+(i+1), pC);
	}
	pC->setParent(this);
}

void Container::remComponent(Component *pC)
{
	ASSERT(pC);

	for (size_t i=0; i<m_children.size(); ++i) {
		if (m_children[i] == pC) {
			m_children.erase(m_children.begin() + i);
			return;
		}
	}
}

bool Container::contains(Component *pC)
{
	if (!pC) return false;
	for (size_t i=0; i<m_children.size(); ++i) {
		if (m_children[i] == pC) {
			return true;
		}
	}
	return false;
}

int Container::findChildId(begui::Component *pC)
{
	ASSERT(pC);
	for (size_t i=0; i<m_children.size(); ++i) {
		if (m_children[i] == pC) {
			return (int)i;
		}
	}
	return -1;
}

void Container::showModal(begui::Component *pC)
{
	ASSERT(pC);
	m_pModalComponent = pC;
	pC->setVisible(true);
}

void Container::hideModal()
{
	if (m_pModalComponent) {
		m_pModalComponent = 0;
	}
}