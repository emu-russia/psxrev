#include "Dockable.h"

using namespace begui;

void Dockable::attachTo(Component *pC, Dockable::DockSide sideOfTarget, Dockable::Align align, bool bAllowResize)
{
	ASSERT(pC);

	DockRelation *rel = 0;

	// check if there is already a relationship with the given target
	for (size_t i=0; i<m_relations.size(); ++i) {
		if (m_relations[i].dockTarget == pC)
			rel = &m_relations[i];
	}

	// if not, create one
	if (!rel) {
		m_relations.push_back(DockRelation());
		rel = &m_relations.back();

		// add this component as a dependent of the target
		try {
			Dockable *pDC = dynamic_cast<Dockable*>(pC);
			pDC->addDependent(this);
		}
		catch (const std::bad_cast&) {
			// do nothing! We allow docking to the sides of
			// components that are not dockable as well
		}
	}

	// store the params
	rel->dockTarget = pC;
	rel->sideOfTarget = sideOfTarget;
	rel->alignment = align;
	rel->bAllowResize = bAllowResize;
}

void Dockable::detachFrom(Component *pC)
{
	ASSERT(pC);

	// remove this component from the list of dependents of the father
	try {
		Dockable *pDC = dynamic_cast<Dockable*>(pC);
		pDC->remDependent(this);
	}
	catch (const std::bad_cast&) {
		// do nothing! We allow docking to the sides of
		// components that are not dockable as well
	}

	// find and erase the corresponding relation
	// ASSUMPTION: only one relation per target
	for (size_t i=0; i<m_relations.size(); ++i) {
		if (m_relations[i].dockTarget == pC) {
			m_relations.erase(m_relations.begin() + i);
			return;
		}
	}
}

void Dockable::addDependent(Component *pC)
{
	ASSERT(pC);

	// add this component to the list of dependents
	m_dependents.push_back(pC);
}

void Dockable::remDependent(Component *pC)
{
	ASSERT(pC);

	// remove this component from the list of dependents
	for (size_t i=0; i<m_dependents.size(); ++i)
		if (m_dependents[i] == pC) {
			m_dependents.erase(m_dependents.begin() + i);
			return;
		}
}

bool Dockable::isDocked(Dockable::DockSide side)
{
	// check if this component has a relationship for that side
	for (size_t i=0; i<m_relations.size(); ++i)
	{
		switch (side) {
			case DOCK_LEFT: if (m_relations[i].sideOfTarget == DOCK_RIGHT) return true; break;
			case DOCK_RIGHT: if (m_relations[i].sideOfTarget == DOCK_LEFT) return true; break;
			case DOCK_TOP: if (m_relations[i].sideOfTarget == DOCK_BOTTOM) return true; break;
			case DOCK_BOTTOM: if (m_relations[i].sideOfTarget == DOCK_TOP) return true; break;
		}
	}

	// check if any of the dependents has a corresponding relationship
	for (size_t i=0; i<m_dependents.size(); ++i) {
		try {
			Dockable *pDC = dynamic_cast<Dockable*>(m_dependents[i]);
			ASSERT(pDC);
			for (size_t j=0; j<pDC->m_relations.size(); ++j) {
				if (pDC->m_relations[j].sideOfTarget == side)
					return true;
			}
		}
		catch (std::bad_cast&) {
		}
	}

	return false;
}

void Dockable::frameUpdate()
{
	// go through all relations, and enforce them in order.
	// if relations are contradictory, the latest of each set of
	// contradicting ones will be enforced
	int new_left = getLeft(), new_top = getTop();
	int new_right = getRight(), new_bottom = getBottom();
	bool bFixedLeft = false, bFixedTop = false;
	bool bFixedRight = false, bFixedBottom = false;
	bool bUnsolvable = false;

	for (size_t i=0; i<m_relations.size(); ++i)
	{
		Component *pTarget = m_relations[i].dockTarget;
		ASSERT(pTarget);

		// enforce docking
		switch (m_relations[i].sideOfTarget) {
			case DOCK_RIGHT:
				if (!bFixedLeft) {
					new_left = pTarget->getRight()+1;
					bFixedLeft = true;
				}
				else
					bUnsolvable = true;
				break;
			case DOCK_LEFT:
				if (!bFixedLeft)	// maintain size only if possible
					new_left = pTarget->getLeft()-1-getWidth();
				if (!bFixedRight) {
					new_right = pTarget->getLeft()-1;
					bFixedRight = true;
				}
				else
					bUnsolvable = true;
				break;
			case DOCK_BOTTOM:
				if (!bFixedTop) {
					new_top = pTarget->getBottom()+1;
					bFixedTop = true;
				}
				else
					bUnsolvable = true;
				break;
			case DOCK_TOP:
				if (!bFixedTop)	// maintain height if possible
					new_top = pTarget->getTop()-1-getHeight();
				if (!bFixedBottom) {
					new_bottom = pTarget->getTop()-1;
					bFixedBottom = true;
				}
				else
					bUnsolvable = true;
				break;
		}

		// enforce alignment
		switch (m_relations[i].alignment) {
			case ALIGN_LEFT:
				if (!bFixedLeft)
					new_left = pTarget->getLeft();
				if (!bFixedRight)
					new_right = pTarget->getLeft() + getWidth();
				bFixedLeft = true;
				break;
			case ALIGN_RIGHT:
				if (!bFixedLeft)
					new_left = pTarget->getRight() - getWidth();
				if (!bFixedRight)
					new_right = pTarget->getRight();
				bFixedRight = true;
				break;
			case ALIGN_LEFT_RIGHT:
				if (!bFixedLeft)
					new_left = pTarget->getLeft();
				if (!bFixedRight)
					new_right = pTarget->getRight();
				bFixedLeft = bFixedRight = true;
				break;
			case ALIGN_TOP:
				if (!bFixedTop)
					new_top = pTarget->getTop();
				if (!bFixedBottom)
					new_bottom = pTarget->getTop() + getHeight();
				bFixedTop = true;
				break;
			case ALIGN_BOTTOM:
				if (!bFixedTop)
					new_top = pTarget->getBottom() - getHeight();
				if (!bFixedBottom)
					new_bottom = pTarget->getBottom();
				bFixedBottom = true;
				break;
			case ALIGN_TOP_BOTTOM:
				if (!bFixedTop)
					new_top = pTarget->getTop();
				if (!bFixedBottom)
					new_bottom = pTarget->getBottom();
				bFixedTop = bFixedBottom = true;
				break;
			case ALIGN_MIDDLE_HORZ:
				if (!bFixedLeft && !bFixedRight) {
					new_left = pTarget->getLeft() + pTarget->getWidth()/2 - getWidth()/2;
					new_right = new_left + getWidth();
				}
				bFixedLeft = bFixedRight = true;
				break;
			case ALIGN_MIDDLE_VERT:
				if (!bFixedTop && !bFixedBottom) {
					new_top = pTarget->getTop() + pTarget->getHeight()/2 - getHeight()/2;
					new_bottom = new_top + getHeight();
				}
				bFixedTop = bFixedBottom = true;
				break;
		}
	}

	ASSERT(!bUnsolvable); // oh snap! The user messed up. Break if we are debugging

	// now set the new position and dimensions of the component
	setPos(new_left, new_top);
	setSize(new_right-new_left, new_bottom-new_top);

	// update all dependent components
	// NOTE: this loop causes frameUpdate to be called twice per update
	// for the docked components. This, however, is the only way to enforce
	// that frameUpdate is going to be called in the right order for each
	// component in the tree of dependencies
	// TODO: do the update only if the position/size of the component
	// changed while applying the relations
	// TODO: make sure that dependencies form a tree!!!!! Otherwise,
	// we will fall in an infinite loop!!!
	for (size_t i=0; i<m_dependents.size(); ++i) {
		ASSERT(m_dependents[i]);
		m_dependents[i]->frameUpdate();
	}

	// optional route: do not use frameUpdate but a new method, like
	// enforceRelations() or something

	Component::frameUpdate();
}