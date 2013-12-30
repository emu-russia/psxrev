/* 
// Copyright 2007 Alexandros Panagopoulos
//
// This software is distributed under the terms of the GNU Lesser General Public Licence
//
// This file is part of Be3D library.
//
//    Be3D is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Be3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with Be3D.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _THREAD_H45631_INCLUDED_
#define _THREAD_H45631_INCLUDED_

#pragma once

#include "common.h"

class Thread
{
private:
#ifdef _POSIX_THREADS
	// TODO: posix thread support (for REAL OSes)
#endif
	HANDLE	m_hThread;

public:
	Thread();
	virtual ~Thread();

	void start();
	virtual void run();

private:
	static unsigned long runProc(void* pThis);
};

#endif