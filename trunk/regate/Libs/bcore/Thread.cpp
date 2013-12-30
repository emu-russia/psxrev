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

#include "Thread.h"

Thread::Thread()
{
}

Thread::~Thread()
{
}

void Thread::start()
{
#ifdef _POSIX_THREADS
	// write some posix code (for REAL OSes)
#endif

	// create and start the thread (blasted win32 api).
	m_hThread = ::CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->runProc, 
								(void *)this, 0, NULL);
}

void Thread::run()
{
	// override
}

unsigned long Thread::runProc(void *pThis)
{
	((Thread*)pThis)->run();
	return 0;
}