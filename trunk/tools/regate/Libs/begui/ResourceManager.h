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

#ifndef _RESOURCEMANAGER_H42631_INCLUDED_
#define _RESOURCEMANAGER_H42631_INCLUDED_

#pragma once

#pragma warning (push)
#pragma warning (disable : 4996)	// to avoid this, use namespace stdext:: instead of std...
									// but this breaks compilation with VS 6... thanks MS!

#include "common.h"
#include <hash_map>
#include "../bcore/Rect.h"

namespace begui {

/**
 *===================================================================================
 * ResourceManager: This class manages resources used by the GUI. This includes
 *			textures, strings, and scalar/vector values that define the GUI objects.
 *			It provides for organized loading/unloading of texture resources, and
 *			allows the use of parametrized skins.
 *===================================================================================
 */
class ResourceManager
{
public:
	/**
	* ImageRef: defines a loaded image, by the texture on which it has
	*		been loaded, and the coordinates of the rectangle of that
	*		image on the texture. This allows for many small images to be
	*		packed in few large textures.
	*/
	class ImageRef {
		long	m_id;
	public:
		Texture *m_texture;
		Vector2 m_topLeft;
		Vector2 m_bottomRight;
		int		m_width, m_height;	// width and height of the original image file from which this image was loaded
									// (can be used to convert from normalized to pixel coordinates and vice versa)

	public:
		ImageRef() : m_texture(0), m_width(0), m_height(0) { }
	};

	/**
	 * Stock images available
	 */
	enum StockMap {
		FRAME_BACKGROUND,
		WINDOW_RES,
		STD_CONTROLS
	};
	
	struct ImageDesc {
		std::string filename;
		int left, top, right, bottom;
	};

	/**
	 * Style: collection of properties that define a visual style for a component class
	 */
	class Style {
		friend class ResourceManager;
	protected:
		std::string m_name;
		stdext::hash_map<std::string, std::string>	m_sVals;
		stdext::hash_map<std::string, double>		m_fVals;
		stdext::hash_map<std::string, int>			m_iVals;
		stdext::hash_map<std::string, Color>		m_cVals;
		stdext::hash_map<std::string, ImageDesc>	m_imgVals;
		stdext::hash_map<std::string, Rect<int> >	m_riVals;
	public:
		const std::string& get_name() const					{ return m_name; }
		int			get_i(const std::string &name) const	{ return m_iVals.find(name)->second; }
		double		get_s(const std::string &name) const	{ return m_fVals.find(name)->second; }
		std::string	get_f(const std::string &name) const	{ return m_sVals.find(name)->second; }
		Color		get_c(const std::string &name) const	{ return m_cVals.find(name)->second; }
		ImageDesc	get_img(const std::string &name) const	{ return m_imgVals.find(name)->second; }
		Rect<int>	get_rect(const std::string &name) const	{ return m_riVals.find(name)->second; }

		bool hasProp(const std::string &name) const {
			if (m_iVals.find(name) != m_iVals.end()) return true;
			if (m_fVals.find(name) != m_fVals.end()) return true;
			if (m_sVals.find(name) != m_sVals.end()) return true;
			if (m_cVals.find(name) != m_cVals.end()) return true;
			if (m_imgVals.find(name) != m_imgVals.end()) return true;
			if (m_riVals.find(name) != m_riVals.end()) return true;
			return false;
		}
	};

	/**
	 * ClassDef: All styles and properties that define a component class
	 */
	class ClassDef {
		friend class ResourceManager;
	protected:
		std::string m_name;
		stdext::hash_map<std::string, Style>	m_styles;
	public:
		const std::string& get_name() const						{ return m_name; }
		const Style&	style(const std::string &name) const	{ return m_styles.find(name)->second; }
	};

private:
	static ResourceManager	*m_instance;
	std::string								m_resourceDir;
	std::vector<Texture*>					m_loadedTextures;
	stdext::hash_map<std::string, ImageRef>	m_images;

	stdext::hash_map<std::string, ClassDef> m_classes;

	
	ResourceManager();

public:
	static inline ResourceManager* inst() { if (!m_instance) m_instance = new ResourceManager(); return m_instance; }
	
	virtual ~ResourceManager();

	void loadResources();
	void freeResources();

	Texture*	getStockMap(StockMap i);

	// loadImage is the method that should be used to request images. If the image has already
	// been loaded, then loadImage will not reload it, unless the bForceDuplicate flag is set.
	ImageRef	loadImage(const std::string &filename, bool bPack = true, bool bForceDuplicate = false);
	ImageRef	loadImage(const ImageDesc &desc);

	std::string getResourceDir() const;
	void		setResourceDir(const std::string& resdir);

	// Property handling
	bool loadPropertyFile(const std::string& fname);
	const ClassDef& getClassDef(const std::string &class_name) const	{ return m_classes.find(class_name)->second; }
};

#pragma warning (pop)

};

#endif