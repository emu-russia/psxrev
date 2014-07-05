/* 
// Copyright 2008 Alexandros Panagopoulos
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

#ifndef _LIVEVAR_H42631_INCLUDED_
#define _LIVEVAR_H42631_INCLUDED_

#pragma once

#include <vector>
#include <list>

template <class T>
class LiveVar
{
private:
	T	 m_value;
	bool m_bDirty;

public:
	LiveVar();
	LiveVar(const T& value);
	LiveVar(const LiveVar<T> &v);
	~LiveVar();

	// control of the dirty flag. Make the method names kinda long and different, so that
	// they dont interfere with the contained class T's methods
	bool livevar_is_dirty() const			{ return m_bDirty; }
	void livevar_set_dirty(bool bDirty)	{ m_bDirty = bDirty; }

	// copy operators
	LiveVar<T>& operator = (const LiveVar<T>& var);
	T& operator = (const T& value);

	// cast operators
//	operator T ();
//	operator T () const;
	operator T& ();
	operator const T& () const;
};

/////////////////////////////////////////////////////////////////////

template <class T>
LiveVar<T>::LiveVar() : m_bDirty(false) { }

template <class T>
LiveVar<T>::LiveVar(const T& value) : m_value(value), m_bDirty(true) { }

template <class T>
LiveVar<T>::LiveVar(const LiveVar<T> &v) : m_value(v.m_value), m_bDirty(true) { }

template <class T>
LiveVar<T>::~LiveVar() { }

template <class T>
LiveVar<T>& LiveVar<T>::operator = (const LiveVar<T>& var) {
	m_value = var.m_value;
	m_bDirty = true;
	return (*this);
}

template <class T>
T& LiveVar<T>::operator = (const T& value) {
	m_value = value;
	m_bDirty = true;
	return m_value;
}
/*
template <class T>
LiveVar<T>::operator T ()
{
	// when we access the contained data without const, set the dirty flag
	m_bDirty = true;
	return m_value;
}

template <class T>
LiveVar<T>::operator T () const
{
	return m_value;
}*/

template <class T>
LiveVar<T>::operator T& ()
{
	// when we access the contained data without const, set the dirty flag
	m_bDirty = true;
	return m_value;
}

template <class T>
LiveVar<T>::operator const T& () const
{
	return m_value;
}

#endif