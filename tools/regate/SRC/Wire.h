#pragma once

namespace begui {

class Wire : public Component
{

public:
    Wire ();
    virtual ~Wire();

	virtual void create(int x, int y, const std::string &style_name="std");

	// overrides
	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

    void    setEndPoint (int x, int y);

private:
    int     x_start, y_start, x_end, y_end;
	int     m_drawMode;

};

};  // begui