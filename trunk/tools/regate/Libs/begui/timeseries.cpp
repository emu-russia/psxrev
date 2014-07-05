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

#include "timeseries.h"
#include "common.h"
#include "util.h"

Updater* Updater::m_inst = 0;


void Updater::update_all(double time)
{
	std::list< Updateable* >::iterator it;
	for (it=m_vars.begin(); it!=m_vars.end(); ++it) {
		(*it)->update(time);
	}
}

void Updater::update_all_current_time()
{
	update_all(begui::system::current_time() / 1000.0);
}

void Updater::register_var(Updateable *variable)
{
	ASSERT(variable);
	m_vars.push_back(variable);
}

void Updater::unregister_var(Updateable *variable)
{
	ASSERT(variable);
	std::list< Updateable* >::iterator it;
	for (it=m_vars.begin(); it!=m_vars.end(); ++it) {
		if ((*it) == variable) {
			m_vars.erase(it);
			return;
		}
	}
}