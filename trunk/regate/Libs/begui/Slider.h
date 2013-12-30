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

#ifndef _SLIDER_H42631_INCLUDED_
#define _SLIDER_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Component.h"
#include "ResourceManager.h"

namespace begui {

class Slider : public Component
{
private:
	int			m_id;
	void		(*m_pCallback)(int);
	double		m_min, m_max;
	double		m_curValue;
	int			m_nSteps;
	bool		m_bShowSteps;
	bool		m_bShowValue;
	bool		m_bDispPercentage;
	std::string	m_valuePrintFormat;	// the format used by sprintf to display the values
	bool		m_bIsEnabled;

	bool		m_bDragging;

	double		*m_pBoundValue;

	ResourceManager::ImageRef	m_sliderBg, m_labelBg, m_marker;
	Rect<int>	m_sliderActiveArea, m_sliderResizableArea;
	Rect<int>	m_markerActiveArea;
	Rect<int>	m_labelActiveArea, m_labelResizableArea;
	Color		m_sliderTextColor, m_labelTextColor;

public:
	Slider();

	void create(int x, int y, int width, int id=0, void (*callback)(int id)=0, const std::string &style_name = "std");
	void setBounds(double min, double max, double initVal, int nSteps=0);

	void bindValue(double *val);	// create a 'live' variable for the slider's current value
									// 0 accepted, to remove current bound variable.

	void showSteps(bool bShow)		{ m_bShowSteps = bShow; }
	void setCurValue(double val)	{ m_curValue = val; if (m_pBoundValue) *m_pBoundValue = val; }
	void showValue(bool bShow)		{ m_bShowValue = bShow; }
	void displayPercentage(bool bPerc)	{ m_bDispPercentage = bPerc; }
	void setValuePrintFormat(const std::string& format)	{ m_valuePrintFormat = format; }
	void setEnabled(bool bEnabled)	{ m_bIsEnabled = bEnabled; }

	virtual void onUpdate();
	virtual void onRender();
	virtual bool onMouseDown(int x, int y, int button);
	virtual bool onMouseMove(int x, int y, int prevx, int prevy);
	virtual bool onMouseUp(int x, int y, int button);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual bool isPtInside(int x, int y);
};

};

#endif