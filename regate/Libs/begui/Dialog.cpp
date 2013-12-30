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

#include "Dialog.h"
#include "FrameWindow.h"

using namespace begui;

void Dialog::showModal()
{
	if (FrameWindow::inst())
		FrameWindow::inst()->showModal(this);
	else {
		ASSERT(0);
		// Use Container::showModal for your top-level container, instead of this method
	}
}

void Dialog::close()
{
	if (FrameWindow::inst())
		FrameWindow::inst()->hideModal();
	else {
		ASSERT(0);
		// Use Container::showModal for your top-level container, instead of this method
	}
}