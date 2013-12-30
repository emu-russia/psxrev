#pragma once

#include "common.h"

class ChildWnd1 : public Window
{
private:
	Button	m_button1, m_button2, m_button3, m_button4, m_button5;
	RadioButton m_radio1;
	RadioButton m_radio2;
	RadioButton m_radio3;
	Group m_group;
	Slider m_slider;
	TextBox m_textbox;
	ListBox m_listbox1, m_listbox2, m_listbox3, m_listbox4;
	ComboBox m_combobox1, m_combobox2, m_combobox3;
	TabContainer m_tabs;
	CheckBox	m_checkbox1, m_checkbox2;
	ViewportComponent m_vp;
	Label m_label1;
	ImageBox m_image[10000];
	ScrollBar m_scroll1;

    float   zoom;
    int     panx, pany;

    bool    m_drag;
    int     drag_x, drag_y;
    int     old_x[10000], old_y[10000];
    int     old_width[10000], old_height[10000];

    int     mouse_x, mouse_y;

    Vias    m_vias[10000];
    int     num_vias;

    bool    wireDraw;
    Wire    m_wire;

    void    updateTitle ();


public:
	ChildWnd1() { }
	void onCreate();
	bool onMouseDown (int x, int y, int button);
	bool onMouseMove(int x, int y, int prevx, int prevy);
	bool onMouseUp(int x, int y, int button);
    void onMouseWheel (int zDelta);

	void onKeyUp(int key);
};

class Application : public BaseApp_Win
{
private:
	ChildWnd1	m_wnd1;
	Dialog		m_dlg1;
	Button		m_dlgBtn1;

	static Application	*m_pInst;

public:
	virtual bool onCreate();
	void onMenu(int id);
	void onButtonDown(int id, const Vector2i& pos);
	void onClick(int id);
	void onRenderVp();

	static Application* inst()	{ if (!m_pInst) m_pInst = new Application(); return m_pInst; }
};