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

#pragma once

#include "common.h"

namespace begui {

class PropertySheet
{
public:
	static bool			loadFromFile(const std::string& filename);
	static int			getProperty_i(const std::string& entity_name, const std::string& property_name);
	static double		getProperty_d(const std::string& entity_name, const std::string& property_name);
	static Color		getProperty_cl(const std::string& entity_name, const std::string& property_name);
	static Vector2		getProperty_v2(const std::string& entity_name, const std::string& property_name);
	static std::string	getProperty_s(const std::string& entity_name, const std::string& property_name);
};

};