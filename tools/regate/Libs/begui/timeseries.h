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

#ifndef _TIMESERIES_H42631_INCLUDED_
#define _TIMESERIES_H42631_INCLUDED_

#pragma once

#include <vector>
#include <list>
#include <ctime>

class Updateable {
public:
	virtual void update(double time) = 0;
};

class Updater {
	std::list< Updateable* > m_vars;
	static Updater*	m_inst;

public:
	inline static Updater* inst()	{ if (!m_inst) m_inst = new Updater(); return m_inst; }

	void update_all(double time);	// updates all timeseries variables in the application
	void update_all_current_time();		// updates all timeseries using the current time in seconds
	void register_var(Updateable *variable);
	void unregister_var(Updateable *variable);
};

template <class T>
class TimeSeries : public Updateable
{
public:
	enum Interpolation {
		CLOSEST,	// (for hard transitions or types that cannot be interpolated, like strings)
		LINEAR,
		HERMITE
	};

private:
	std::vector<T>		m_values;
	std::vector<double> m_timestamps;
	Interpolation		m_interpolation;
	bool				m_bLoop;
	T					m_curValue;
	double				m_timeOffset;

	static std::list< TimeSeries<T>* > m_vars;

public:
	TimeSeries();
	TimeSeries(const T& value);
	TimeSeries(const TimeSeries<T> &ts);
	virtual ~TimeSeries();

	void push_back(const T& value, double time);
	void set_interpolation(Interpolation it)	{ m_interpolation = it; }
	void set_loop(bool bLoop)					{ m_bLoop = bLoop; }
	void transition(const T& start_value, double start_time,
					const T& end_value, double end_time,
					double delay = 0,			// delay before starting the animation, measured in seconds from the time of the call
					Interpolation it = LINEAR);
	void start(double delay = 0);	// run the animation from the beginning, starting from frame 0 at current time + delay (secs)
	void clear();

	TimeSeries<T>& operator = (const TimeSeries<T>& ts);
	TimeSeries<T>& operator = (const T& value);

	operator T ();	// cast operator
	operator T () const;

private:
	virtual void update(double time);
};

/////////////////////////////////////////////////////////////////////

template <class T>
TimeSeries<T>::TimeSeries() : m_interpolation(CLOSEST), m_bLoop(false), m_timeOffset(0)
{
	Updater::inst()->register_var(this);
}

template <class T>
TimeSeries<T>::TimeSeries(const T &value) : m_interpolation(CLOSEST), m_bLoop(false), m_timeOffset(0) {
	m_values.push_back(value);
	m_timestamps.push_back(0);
	m_curValue = value;
	Updater::inst()->register_var(this);
}

template <class T>
TimeSeries<T>::TimeSeries(const TimeSeries<T> &ts) : m_interpolation(ts.m_interpolation),
	m_values(ts.m_values),
	m_timestamps(ts.m_timestamps),
	m_bLoop(ts.m_bLoop),
	m_curValue(ts.m_curValue),
	m_timeOffset(ts.m_timeOffset)
{
	Updater::inst()->register_var(this);
}

template <class T>
TimeSeries<T>::~TimeSeries() {
	Updater::inst()->unregister_var(this);
}

template <class T>
void TimeSeries<T>::clear() {
	m_values.clear();
	m_timestamps.clear();
	m_timeOffset = 0;
}

template <class T>
TimeSeries<T>& TimeSeries<T>::operator = (const TimeSeries<T> &ts) {
	m_values = ts.m_values;
	m_timestamps = ts.m_timestamps;
	m_interpolation = ts.m_interpolation;
	m_bLoop = ts.m_bLoop;
	m_curValue = ts.m_curValue;
	m_timeOffset = ts.m_timeOffset;
	return *this;
}

template <class T>
TimeSeries<T>& TimeSeries<T>::operator = (const T &value) {
	clear();
	m_values.push_back(value);
	m_timestamps.push_back(0);
	m_curValue = value;
	return *this;
}

template <class T>
TimeSeries<T>::operator T () const
{
	return m_curValue;
}

template <class T>
TimeSeries<T>::operator T ()
{
	return m_curValue;
}

template <class T>
void TimeSeries<T>::push_back(const T &value, double time)
{
	m_values.push_back(value);
	m_timestamps.push_back(time);
}

template <class T>
void TimeSeries<T>::update(double time)
{
	if (m_timestamps.empty())
		return;

	// use m_timeOffset (in secs) as the starting time of the animation
	time -= m_timeOffset;

	// take care of looping animation
	if (m_bLoop) {
		double dt = m_timestamps.back() - m_timestamps.front();
		if (dt > 0)
			time = fmod(time, dt);
	}

	// check some common cases first!
	if (time >= m_timestamps.back()) {
		m_curValue = m_values.back();
		return;
	}
	if (time <= m_timestamps.front()) {
		m_curValue = m_values.front();
		return;
	}

	// update the value of m_curValue
	size_t i=0;
	for (; i<m_timestamps.size(); ++i)
	{
		if (m_timestamps[i] > time)
			break;
	}

	switch (m_interpolation)
	{
	case CLOSEST:
		m_curValue = m_values[i-1];
		return;
	case LINEAR: {
		double t1 = m_timestamps[i-1];
		double t2 = m_timestamps[i];
		m_curValue = (T)((t2-time)/(t2-t1)*m_values[i-1] + (time-t1)/(t2-t1)*m_values[i]);
		return;
		}
	}
}

template <class T>
void TimeSeries<T>::start(double delay)
{
	m_timeOffset = (double)(1000*clock()/CLOCKS_PER_SEC / 1000.0);
}

template <class T>
void TimeSeries<T>::transition(const T& start_value, double start_time,
							const T& end_value, double end_time,
							double delay,
							Interpolation it)
{
	clear();
	m_values.push_back(start_value);
	m_values.push_back(end_value);
	m_timestamps.push_back(start_time);
	m_timestamps.push_back(end_time);
	m_curValue = start_value;
	m_interpolation = it;

	start(delay);
}

#endif