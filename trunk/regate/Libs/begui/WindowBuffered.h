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

#ifndef _WINDOW_BUFFERED_H42631_INCLUDED_
#define _WINDOW_BUFFERED_H42631_INCLUDED_

#pragma once

#include "Window.h"
#include "../bcore/RenderPass.h"

namespace begui {

/**
 * WindowBuffered: A window that can render its contents to a texture instead of
 *		directly on the screen. This allows 2d and 3d deformations of the rendered
 *		window and contents, as well as the ability not to re-render the contents of
 *		a window unless they have changed.
 *
 */
class WindowBuffered : public Window
{
public:
	WindowBuffered();
	virtual ~WindowBuffered();

	void	enableTransitionEffects(bool bOnMove, bool bOnStateChange, bool bOnResize);
	void	enableClothSimulation(bool bEnable);
	void	setContentUpdateOnTransition(bool bEnable);
	void	setSelectiveUpdate(bool bEnable);
	void	usePrivateRenderBuffer(bool bEnable);

	virtual void frameUpdate();
	virtual void frameRender();

protected:
	virtual void onUserMove(int dx, int dy);

private:
	RenderPass m_renderPass;
	static Texture *m_pSharedRenderTarget;
	static int		m_SRTRefCount;	// ref counter for the shared render target pointer
	bool	m_bSharedRenderBuffer;	// the render buffer where the contents of this window are rendered to is shared
									// between multiple buffered window instances.

	bool	m_bFixedContentWhileFX;	// do not update the contents of this window while displaying a transition effect
	bool	m_bSelectiveUpdate;		// update only dirty areas
	bool	m_fxOnMove;
	bool	m_fxOnResize;
	bool	m_fxOnStateChange;		// when the window is minimized/maximized/restored

	bool	m_bEnableClothSimulation;	// if enabled, window deformations can be done by a cloth simulator. This
									// way, when the window is minimized, for example, the cloth simulator is enabled,
									// the window "hinges" are detached, and wind is enabled, to blow the window away
									// as it collides with 3d volumes defined for the rest of the windows on screen :)
									// If disabled, a predefined animation is performed.

	unsigned long	m_prevMoveStepTime;
	Vector2			m_moveSpeed;
};

};

#endif