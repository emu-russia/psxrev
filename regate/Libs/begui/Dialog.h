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

#ifndef _DIALOG_H42631_INCLUDED_
#define _DIALOG_H42631_INCLUDED_

#pragma once

#include "common.h"
#include "Window.h"

namespace begui {
	
class TextBox;
class Slider;
class Button;

/**
 * Dialog: represent a modal or modeless dialog box
 */
class Dialog : public Window
{
	friend class FrameWindow;
public:
	void showModal();
	void close();
};

/**
 * DialogFactory: this class is an easy way to create dialog boxes that correspond to the
 *			simple template of label-control pairs and sets of buttons arranged one under the other (as in most
 *			dialog boxes)
 */
class DialogFactory
{
public:
	/* general approach: call beginDialogAssembly() to start assemblying a new dialog box. Then call the add.. methods
	to add pairs of a label on the left and a control on the right. Each call adds the pair to a new line in the dialog
	box (this means, under the previous pair added). Finally, call endDialogAssembly() to finish and get the pointer
	to the newly created dialog */
	void beginDialogAssembly(const std::string &title);
	void addText(const std::string &text);
	void addTextField(const std::string &label, TextBox **control = 0);	// if <control> is not null, set it to point to the newly created control
	void addSlider(const std::string &label, Slider **control = 0);
	void addButton(const std::string &label, const std::string &btn_text, Button **control = 0);
	//...for all controls
	Dialog* endDialogAssembly();
};

};

#endif