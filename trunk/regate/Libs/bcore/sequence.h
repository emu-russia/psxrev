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

#ifndef _SEQUENCE_H45631_INCLUDED_
#define _SEQUENCE_H45631_INCLUDED_

#pragma once

#include <list>
#include <vector>

/**
 * Sequence: a sequence of values of type T, with a timestamp attached on each one
 *		The current value is computed when update() is called. Its value
 *		comes from the linear interpolation between the two samples closest in
 *		time. This value is cached in m_curValue.
 */
template <typename T>
class AnimSequence
{
private:
	template <class S>
	class Item
	{
	public:
		S		value;
		float	time;

		Item(S& s, float t) : value(s), time(t) { }
	};
	typedef Item<T> tItem;

	std::vector<tItem>	m_values;
	T					m_curValue;

public:
	AnimSequence() { m_values.push_back(tItem(T(), 0)); };

	// push a new value
	void push_back(const T& val, float time) {
		ASSERT(time >= m_values.back().time);
		m_values.push_back(tItem(val, time));
	}

	// update to move to the current time.
	void update(float time)
	{
		m_currentValue = valueAt(time);
	}

	// get the current value
	const T& value() const	{ return m_curValue; }

	// get the value for an arbitrary time
	// calculates a value using linear interpolation
	const T& valueAt(float time) const
	{
		if (time < m_values.front().time)
			return m_values.front().value;
		
		T prevValue = m_values.front().value;
		float prevTime = m_values.front().time;
		for (int i=0; i<m_values.size(); ++i)
		{
			if (time < m_values[i].time)
			{
				// CHANGEME: interpolate using an arbitrary interpolator as a template arg!!
				float f = (time - m_values[i].time)/(prevTime - m_values[i].time);
				return (1-f)*prevValue + f*m_values[i].value;
			}
			prevValue = m_values[i].value;
			prevTime = m_values[i].time;
		}
		return m_values.back().value;
	}

	// define the equality operator to be able to use this as a single variable
	AnimSequence<T>& operator = (const T& val) { m_values.back().value = val; return *this; }

	// dereference operator to get the current value
	const T& operator *() const	{ return m_curValue; }
};

/**
 * OblivSequence: an oblivious sequence of values. It is the same as a normal
 *			sequence, with the difference that we assume that time progresses
 *			monotonically. So, after update is called for time t, it will
 *			never be called for time values < t.
 *			An oblivious sequence DISCARDS all values that are no longer
 *			of interest cause they refer to the past.
 */
template <typename T>
class OblivSequence : public AnimSequence<T>
{	
private:
	template <class S>
	class Item
	{
	public:
		S		value;
		float	time;

		Item(S& s, float t) : value(s), time(t) { }
	};
	typedef Item<T> tItem;

	std::list<tItem>	m_values;
	T					m_curValue;
#ifdef _DEBUG
	float	m_lastTime;
#endif

public:
	OblivSequence() :
#ifdef _DEBUG
	  m_lastTime(0)
#endif
	{
		m_values.push_back(tItem(T(), 0));
	}

	// push a new value
	void push_back(const T& val, float time) {
		ASSERT(time >= m_values.back().time);
		m_values.push_back(tItem(val, time));
	}

	// update to move to the current time.
	void update(float time)
	{
		ASSERT(time >= m_lastTime);
		
		if (time < m_values.front().time)
			return m_values.front().value;
		
		T prevValue = m_values.front().value;
		float prevTime = m_values.front().time;
		for (std::list<T>::iterator it=m_values.begin(); it!=m_values.end(); )
		{
			tItem &t = (*it);
			if (time < t.time)
			{
				// CHANGEME: interpolate using an arbitrary interpolator as a template arg!!
				float f = (time - t.time)/(prevTime - t.time);
				return (1-f)*prevValue + f*t.value;
			}
			prevValue = t.value;
			prevTime = t.time;

			// check if this item should be removed.
			++it;
			if (it == m_values.end())
			{
				// if this (it-1) is the last element, do not remove
				// it. Just return the last value.
				return m_values.back().value;
			}
			if ((*it).time < time)
			{
				// remove this element
				--it;
				it = m_values.erase(it);
			}
		}
		return m_values.back().value;
	}

	// get the current value
	const T& value() const	{ return m_curValue; }

	// define the equality operator to be able to use this as a single variable
	OblivSequence<T>& operator = (const T& val) { m_values.back().value = val; return *this; }

	// dereference operator to get the current value
	const T& operator *() const	{ return m_curValue; }
};

#endif