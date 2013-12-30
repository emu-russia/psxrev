#pragma once

namespace begui {

class Vias : public Component
{

public:
    Vias ();
    virtual ~Vias();

	virtual void create(int x, int y, const std::string &style_name="std");

	// overrides
	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

private:
	int     m_radius;

};

};  // begui