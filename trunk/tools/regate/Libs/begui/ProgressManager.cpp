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

#include "ProgressManager.h"

using namespace begui;

ProgressIndicator	*ProgressManager::m_pCurProgressInd;
std::stack<ProgressManager::SubRange> ProgressManager::m_subranges;

void ProgressManager::setProgressIndicator(ProgressIndicator *pInd)
{
	m_pCurProgressInd = pInd;
}

void ProgressManager::pushSubRange(double start, double end)
{
	SubRange subrange;
	subrange.min = start;
	subrange.max = end;
	subrange.progress = 0;
	m_subranges.push(subrange);
}

void ProgressManager::popSubRange()
{
	// pop the subrange
	SubRange sr = m_subranges.top();
	m_subranges.pop();

	// update the previous-level subrange
	if (m_subranges.size() > 0)
		m_subranges.top().progress = sr.max;
}

void ProgressManager::setProgress(double progress)
{
	if (m_subranges.size() == 0)
		return;

	// update the progress value stored in the active subrange
	SubRange sr = m_subranges.top();
	sr.progress = progress;
	
	// update the progress indicator, if any
	if (m_pCurProgressInd)
		m_pCurProgressInd->setProgress(sr.min + (sr.max - sr.min)*sr.progress);
}

double ProgressManager::getProgress()
{
	// progress value is in [0,1]
	if (m_subranges.size() == 0)
		return -1;
	SubRange sr = m_subranges.top();
	return sr.min + (sr.max - sr.min)*sr.progress;
}