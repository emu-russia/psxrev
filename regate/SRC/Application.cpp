#include "Application.h"

Application *Application::m_pInst = NULL;

void ChildWnd1::updateTitle ()
{
    static char title[1024];
    sprintf (title, "pan %i: %i, zoom: %.2f, mouse %i:%i", panx, pany, zoom, mouse_x, mouse_y);
    m_title = title;
}

void ChildWnd1::onCreate()
{

    num_vias = 0;
    panx = pany = 0;
    zoom = 1.0;

    wireDraw = false;

/*
	Image top;
	top.create (1632, 1224, 4 );
	top.loadPNG ( "m2_mit20x/c0004_r0014.png" );
    m_image[0].create ( 0, 0, 1632, 1224, &top );
    addComponent(&m_image[0]);
    old_x[0] = m_image[0].getLeft ();
    old_y[0] = m_image[0].getTop ();
    old_width[0] = m_image[0].getWidth ();
    old_height[0] = m_image[0].getHeight ();

	Image img;
	img.create (280, 348 );
	img.loadPNG ( "cells/DFF.png" );
    for (int i=1; i<2; i++) {
        m_image[i].create ( 25+i, 25+i, 280, 348, &img );
	    addComponent(&m_image[i]);
        old_x[i] = m_image[i].getLeft ();
        old_y[i] = m_image[i].getTop ();
        old_width[i] = m_image[i].getWidth ();
        old_height[i] = m_image[i].getHeight ();
    }
*/

	// change the size of the window, so that the client area is large
	// enough to hold the contents
	setClientAreaSize(800, 600);
}

bool ChildWnd1::onMouseDown (int x, int y, int button)
{
    m_drag = true;
    drag_x = x;
    drag_y = y;
    return Window::onMouseDown (x, y, button);
}

bool ChildWnd1::onMouseMove(int x, int y, int prevx, int prevy)
{
    if (m_drag) {
        panx += x - drag_x;
        pany += y - drag_y;
        drag_x = x;
        drag_y = y;
        
        updateTitle ();

        //for (int i=0; i<2; i++) m_image[i].setPos ( old_x[i] + panx, old_y[i] + pany );
    }
    mouse_x = x;
    mouse_y = y;
    updateTitle ();

    if (wireDraw) m_wire.setEndPoint (mouse_x, mouse_y);

    //Console::print ( "%i, %i\n", mouse_x, mouse_y );
    return Window::onMouseMove (x, y, prevx, prevy);
}

bool ChildWnd1::onMouseUp(int x, int y, int button)
{
    m_drag = false;
    return Window::onMouseUp (x, y, button);
}

void ChildWnd1::onKeyUp(int key)
{
	//Console::print ( "%i\n", key );

	switch ( key )
	{
        case 86:		// V - vias
            m_vias[num_vias].create (mouse_x, mouse_y);
            m_vias[num_vias].setAlwaysOnTop (true);
            addComponent (&m_vias[num_vias]);
            num_vias++;
		    break;

        case 49:        // M1 wire
            if (wireDraw) {
                wireDraw = false;
            }
            else {
                m_wire.create (mouse_x, mouse_y);
                addComponent (&m_wire);
                wireDraw = true;
            }
            break;
        case 50:        // M2 wire
            break;

		case 66:		// B - box
			break;

		case 27:		// Cancel drawing
			break;
	}
	Window::onKeyUp (key);
}

void ChildWnd1::onMouseWheel (int zDelta)
{
    if (zDelta > 0) zoom += 0.1;
    else zoom -= 0.1;
    if (zoom < 0.1) zoom = 0.1;
    if (zoom > 4.0) zoom = 4.0;
    updateTitle ();

    for (int i=0; i<2; i++) m_image[i].setSize ( old_width[i] * zoom, old_height[i] * zoom );
}

bool Application::onCreate()
{
	// Get a pointer to the menu of the main window
	Menu &menu = *FrameWindow::inst()->createMainMenu();

	// File
    {
	    Menu *pFileMenu = menu.addMenuItem("File", 1, makeFunctor(*this, &Application::onMenu));
        pFileMenu->addMenuItem("Load project (Ctrl+L)", 101, makeFunctor(*this, &Application::onMenu));
	    pFileMenu->addMenuItem("Save project (Ctrl+S)", 102, makeFunctor(*this, &Application::onMenu));
	    pFileMenu->addMenuItem("Save project as..", 103, makeFunctor(*this, &Application::onMenu));
	    pFileMenu->addSeparator();
	    pFileMenu->addMenuItem("Quit", 106, makeFunctor(*this, &Application::onMenu));
    }

    // Edit
    {
        Menu *pEditMenu = menu.addMenuItem("Edit", 2, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addMenuItem("Undo (Ctrl+Z)", 201, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addMenuItem("Redo (Ctrl+Y)", 202, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addSeparator();
        pEditMenu->addMenuItem("Cut (Ctrl+X)", 203, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addMenuItem("Copy (Ctrl+C)", 204, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addMenuItem("Paste (Ctrl+V)", 205, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addMenuItem("Delete (Del)", 206, makeFunctor(*this, &Application::onMenu));
        pEditMenu->addSeparator();
        pEditMenu->addMenuItem("Regate Properties...", 207, makeFunctor(*this, &Application::onMenu));
    }

    // Cells
    {
        Menu *pCellsMenu = menu.addMenuItem("CXD Cells", 3, makeFunctor(*this, &Application::onMenu));
        pCellsMenu->addMenuItem("Add cell (Ctrl+A)", 301, makeFunctor(*this, &Application::onMenu));
    }

	// create a child window
	m_wnd1.create(0, 10, 800, 600, "");
	m_wnd1.enableMinimize (true);
	m_wnd1.enableMaximize (true);
	FrameWindow::inst()->addComponent(&m_wnd1);
	FrameWindow::inst()->maximize ();

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, // Instance 
				   HINSTANCE hPrevInstance, // Previous Instance 
				   LPSTR lpCmdLine, // Command Line Parameters 
				   int nCmdShow) // Window Show State 
{
	// Uncomment this line to have a console for text output
	Console::create();

	// Set the path of the resource directory
	ResourceManager::inst()->setResourceDir("resources/");

	FrameWindow::Options opt;
	opt.bOwnDraw = true;
	opt.bFullScreen = false;
	opt.nColorBits = 24;
	opt.nDepthBits = 8;
	opt.nStencilBits = 0;
	//opt.windowStyleName = "std_transparent";
	if (!Application::inst()->initialize("CXD8530CQ", 800, 600, FrameWindow::MULTIPLE, &opt))
		return false;

	Application::inst()->setSyncRendering(true);

	return Application::inst()->run();
}

void Application::onMenu(int id)
{
	switch (id)
	{
	case 101:
		m_dlg1.create(30, 30, 300, 300, "test_dialog");
		m_dlgBtn1.create(30, 30, "Close", 10001, makeFunctor(*this, &Application::onMenu));
		m_dlg1.addComponent(&m_dlgBtn1);
		m_dlg1.showModal();
		break;
	case 10001:
		m_dlg1.close();
		break;
	case 106:
		exit(0);
		break;
	}
}

void Application::onButtonDown(int id, const Vector2i &pos)
{
	switch (id)
	{
	case 202:
		Console::print("click\n");
		break;
	}
}

void Application::onClick(int id)
{
	switch (id)
	{
	case 203:
		Console::print("button 203 : onClick\n");
		break;
	}
}