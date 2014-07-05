// Implements vias custom widget

#include "common.h"
#include "Vias.h"

using namespace begui;

Vias::Vias()
{
}

Vias::~Vias ()
{
}

void Vias::create (int x, int y, const std::string &style_name)
{
    m_radius = 6;

	setPos(x, y);
	setSize(8, 8);
}

void Vias::onUpdate()
{
}

void Vias::onRender()
{
    glEnable (GL_POINT_SMOOTH);
    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize (m_radius);
    glBegin (GL_POINTS);
    glColor3b (0, 0, 0);
    glVertex3i (getLeft(), getTop(), 0);
    glEnd ();
}

bool Vias::onMouseDown(int x, int y, int button)
{
	return true;
}

bool Vias::onMouseMove(int x, int y, int prevx, int prevy)
{
	return true;
}

bool Vias::onMouseUp(int x, int y, int button)
{
	return true;
}

void Vias::onKeyDown(int key)
{
}

void Vias::onKeyUp(int key)
{
}
