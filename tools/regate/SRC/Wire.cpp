// Implements M1 wire custom widget

#include "common.h"
#include "Wire.h"

using namespace begui;

Wire::Wire()
{
}

Wire::~Wire ()
{
}

void Wire::create (int x, int y, const std::string &style_name)
{
    m_drawMode = true;

    x_start = x_end = x;
    y_start = y_end = y;

	setPos(x, y);
	setSize(8, 8);
}

void Wire::onUpdate()
{
}

void Wire::onRender()
{
    glLineWidth (2.5); 
    glBegin (GL_LINES);
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f (x_start, y_start, 0);
    glVertex3f (x_end, y_end, 0);
    glEnd ();
}

bool Wire::onMouseDown(int x, int y, int button)
{
	return true;
}

bool Wire::onMouseMove(int x, int y, int prevx, int prevy)
{
	return true;
}

bool Wire::onMouseUp(int x, int y, int button)
{
	return true;
}

void Wire::onKeyDown(int key)
{
}

void Wire::onKeyUp(int key)
{
    Console::print ("key=%i\n", key);

    //switch (key) {
    //}
}

void Wire::setEndPoint (int x, int y)
{
    if (m_drawMode) {
        x_end = x;
        y_end = y;
    }
}