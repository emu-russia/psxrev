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

#ifndef _PROGRESSMANAGER_H42631_INCLUDED_
#define _PROGRESSMANAGER_H42631_INCLUDED_

#pragma once

#include "common.h"
#include <stack>

namespace begui {

class ProgressIndicator
{
public:
	// progress value is in [0,1]
	virtual void setProgress(double progress) = 0;
};

/**
 * ProgressManager: this class handles globally all progress indications.
 *		This makes it much easier for algorithms to refer to this global
 *		entity instead of specific controls in the environment.
 *		The general idea works as follows: we set the progress bar control
 *		that is the target and then we call the algorithm. Internally the
 *		algorithm/process uses only the static method setProgress to set the
 *		current progress. When a time-consuming sub-function has to be called,
 *		we use pushSubRange to create a new subrange (x->y). Now, the nested
 *		function will treat progressmanager as its own work is the only
 *		progress (as it goes from 0 to 100%). The progress manager will map
 *		these values to the current subrange. When the function is done,
 *		the caller calls popSubRange to remove the subrange and continue
 *		with the normal progression.
 *
 *		NOTE: progress values are in [0,1]
 */
class ProgressManager
{
private:
	class SubRange
	{
	public:
		double progress;
		double min, max;
	};
	static ProgressIndicator	*m_pCurProgressInd;
	static std::stack<SubRange>	m_subranges;

public:
	static void setProgressIndicator(ProgressIndicator*);

	static void pushSubRange(double start, double end);
	static void popSubRange();

	// progress value is in [0,1]
	static void setProgress(double progress);
	static double getProgress();
};

};

#endif