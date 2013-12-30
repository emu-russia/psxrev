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

#pragma warning (push)
#pragma warning (disable : 4996)	// to avoid this, use namespace stdext:: instead of std...
									// but this breaks compilation with VS 6... thanks MS!

#include "common.h"
#include <hash_map>

namespace begui {

class WindowResourceManager
{
private:
	static WindowResourceManager	*m_instance;
	std::string						m_resourceDir;
	Texture	m_mainWindowBg;
	Texture m_childWindowTemplate;

	class ClassProps {
	public:
		stdext::hash_map<std::string, std::string>	m_strProps;
		stdext::hash_map<std::string, double>		m_numProps;
	};
	stdext::hash_map<std::string, ClassProps> m_classProps;

	
	WindowResourceManager();

public:
	static inline WindowResourceManager* inst() { if (!m_instance) m_instance = new WindowResourceManager(); return m_instance; }
	
	virtual ~WindowResourceManager();

	void loadResources();
	void freeResources();

	Texture*	getMainWindowBg()		{ return &m_mainWindowBg; }
	Texture*	getChildWindowImage()	{ return &m_childWindowTemplate; }

	std::string getResourceDir() const;
	void		setResourceDir(const std::string& resdir);

	// Property handling
	void loadPropertyFile(const std::string& fname);

	int			getPropI(const std::string& className, const std::string& propName);
	double		getPropF(const std::string& className, const std::string& propName);
	std::string getPropS(const std::string& className, const std::string& propName);
};

#pragma warning (pop)

};