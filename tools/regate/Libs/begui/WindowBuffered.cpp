#include "WindowBuffered.h"
#include "util.h"
#include <limits.h>

using namespace begui;

Texture *WindowBuffered::m_pSharedRenderTarget = 0;
int WindowBuffered::m_SRTRefCount = 0;

WindowBuffered::WindowBuffered() : 
	m_prevMoveStepTime(0),
	m_moveSpeed(0,0)
{
}

WindowBuffered::~WindowBuffered()
{
	m_renderPass.free();
	if (m_bSharedRenderBuffer) {
		m_SRTRefCount--;
		if (m_SRTRefCount == 0) {
			SAFE_DELETE(m_pSharedRenderTarget);
		}
	}
}

void WindowBuffered::frameUpdate()
{
	Window::frameUpdate();

	// update the render buffer
	int rw = display::getWidth();
	int rh = display::getHeight();
	if (rw != m_renderPass.getWidth() || rh != m_renderPass.getHeight()) {
		// setup the render-to-texture pass again
		if (m_bSharedRenderBuffer)
		{
			// if the shared render target texture does not exist, create it
			if (!m_pSharedRenderTarget) {
				ASSERT(m_SRTRefCount == 0);
				m_pSharedRenderTarget = new Texture;
				m_SRTRefCount = 1;
			}
			m_pSharedRenderTarget->create(rw, rh, GL_RGBA8, 0);

			// setup the render pass
			if (!m_renderPass.setup(RenderPass::PIXEL_RGBA8, rw, rh, m_pSharedRenderTarget)) {
				Console::error("WindowBuffered: Could not create render pass!\n");
				return;
			}
		}
		else {
			if (!m_renderPass.setup(RenderPass::PIXEL_RGBA8, rw, rh)) {
				Console::error("WindowBuffered: Could not create render pass!\n");
				return;
			}
		}
	}

	// do the rendering HERE (not inside the main rendering loop, because we cannot have nested render passes)!
	Rect<int> border = getInactiveBorders();
	m_renderPass.beginPass();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, rw, rh, 0, 0.0, 1.0);
	glViewport(0,0,rw,rh);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-(float)getLeft(), -(float)getTop(), 0);

	display::pushRefFrame(getLeft()+border.left, getTop()+border.top, rw, rh);
	
	Window::frameRender();

	display::popRefFrame();
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	m_renderPass.endPass();
}

void WindowBuffered::frameRender()
{
	Rect<int> border = getInactiveBorders();

	// display the render target
	glColor4f(1,1,1,1);
	m_renderPass.getFrameData()->set();
	int ww = getWidth() + border.left + border.right;
	int hh = getHeight() + border.top + border.bottom;
	float rh = (float)m_renderPass.getFrameData()->getHeight();
	float rw = (float)m_renderPass.getFrameData()->getWidth();
	float tx = 0;
	float ty = 0;
	float tw = (float)ww/rw;
	float th = (float)hh/rh;
	float dx = -m_moveSpeed.x*50;
	glBegin(GL_QUADS);
		glTexCoord2f(tx,rh-ty);	glVertex2f((float)getLeft(), (float)getTop());
		glTexCoord2f(tx+tw,rh-ty);	glVertex2f((float)getLeft()+ww, (float)getTop());
		glTexCoord2f(tx+tw,rh-th-ty);	glVertex2f((float)getLeft()+ww+dx, (float)getTop()+hh);
		glTexCoord2f(tx,rh-th-ty);	glVertex2f((float)getLeft()+dx, (float)getTop()+hh);
	glEnd();
}

void WindowBuffered::onUserMove(int dx, int dy)
{
	unsigned long time = system::current_time();
	long dt = (time - m_prevMoveStepTime);
	if (dt < 0) return;

	m_moveSpeed = Vector2((float)dx/dt, (float)dy/dt);
	m_prevMoveStepTime = time;
}