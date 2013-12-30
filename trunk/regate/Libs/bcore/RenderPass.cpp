#include "RenderPass.h"
#include <stdlib.h>

//TEMP!
void wglGetLastError() { };

RenderPass::RenderPass() : m_pFrameTexture(0), m_bOwnsTexture(false),
	m_width(-1), m_height(-1),
	m_format(PIXEL_RGBA8),
	m_last_hdc(0),
	m_last_hglrc(0),
	m_pDepthTex(0),
	m_bUsePBuffer(false),
	m_FBO(0),
	m_FBODepthBuffer(0),
	m_bGenMipmaps(false)
{
}

RenderPass::~RenderPass()
{
	free();
}

bool RenderPass::setup(PixelFormat pixelFormat, int frameW, int frameH, Texture *pTarget, bool bDepthBuffer)
{
	ASSERT(!m_bGenMipmaps);	// Not implemented completely. Need to check if the mipmap filters have to
	// be set before checking for framebuffer completeness or rendering, and if so, to add interface
	// functionality to do so

	free();

	ASSERT(frameW > 0);
	ASSERT(frameH > 0);
	
	// setup a texture format
	int texFormat = GL_RGBA8;
	switch (pixelFormat)
	{
	case PIXEL_RGBA8: texFormat = GL_RGBA8; break;
	case PIXEL_RGBA16F: texFormat = GL_RGBA16F_ARB; break;
	case PIXEL_RGBA32F: texFormat = GL_RGBA32F_ARB; break;
	default: ASSERT(0);
	}

	if (pTarget)
	{
		// set an external texture as a render target
		ASSERT(frameW == pTarget->getWidth());
		ASSERT(frameH == pTarget->getHeight());

		m_pFrameTexture = pTarget;
		m_bOwnsTexture = false;
	}
	else
	{
		// Create a texture for the given pixel format
		m_pFrameTexture = new Texture();
		m_pFrameTexture->create(frameW, frameH, texFormat);
		m_bOwnsTexture = true;
	}

	m_format = pixelFormat;
	m_width = frameW;
	m_height = frameH;

	m_bUsePBuffer = false;
	if (m_bUsePBuffer)
	{
		// Create the pbuffer
		if (!m_pbuffer.create(frameW, frameH, texFormat))
		{
			MessageBox(0, TEXT("Failed to create pbuffer"), TEXT("Error"), MB_OK);
			exit(-1);
		}

		// create a texture for the depth buffer
		if (bDepthBuffer)
		{
			m_pDepthTex = new Texture();
			m_pDepthTex->create(frameW, frameH, texFormat);
		}
	}
	else
	{
		// Use Frame Buffer Objects

//		if (m_bGenMipmaps) {
//			m_pFrameTexture->set();
//			glGenerateMipmapEXT(GL_TEXTURE_2D);
//		}

		// create an FBO for rendering
		//glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glGenFramebuffersEXT(1, &m_FBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FBO);

		if (bDepthBuffer) {
			glGenRenderbuffersEXT(1, &m_FBODepthBuffer);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_FBODepthBuffer);

			// assign some storage for the depth buffer
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, frameW, frameH);

			// attach the depth buffer to the FBO
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_FBODepthBuffer);
		}

		// attach a texture to the FBO for rendering
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_pFrameTexture->getGLTex(), 0);

		// check if the frame buffer is complete
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
			// some error occured
			// TODO: report the error and try to recover

			// TODO: fallback to a p-buffer

			// TEMP: report error
			return false;
		}
	}

	return true;
}

void RenderPass::free()
{
	if (m_bUsePBuffer)
		m_pbuffer.free();
	else
	{
		if (m_FBO)
			glDeleteFramebuffersEXT(1, &m_FBO);
		if (m_FBODepthBuffer)
			glDeleteRenderbuffersEXT(1, &m_FBODepthBuffer);
	}
	if (m_bOwnsTexture && m_pFrameTexture)
	{
		SAFE_DELETE(m_pFrameTexture);
		SAFE_DELETE(m_pDepthTex);
	}
}

void RenderPass::beginPass()
{
	if (m_bUsePBuffer)
	{
		m_last_hdc = wglGetCurrentDC();
		m_last_hglrc = wglGetCurrentContext();
		wglGetLastError();
		
		// bind the render target texture
		glBindTexture(GL_TEXTURE_2D, m_pFrameTexture->getGLTex());

		// release the pbuffer from the render texture object
		if (wglReleaseTexImageARB(m_pbuffer.getHPBuffer(), WGL_FRONT_LEFT_ARB) == FALSE)
			wglGetLastError();

		if (m_pDepthTex)
		{
			ASSERT(0);
			//wglDrawBuffers(WGL_FRONT_ARB);
			glBindTexture(GL_TEXTURE_2D, m_pDepthTex->getGLTex());
			if (wglReleaseTexImageARB(m_pbuffer.getHPBuffer(), WGL_FRONT_RIGHT_ARB) == FALSE)
				wglGetLastError();
		}

		// make the pbuffer's rendering context current.
		if (wglMakeCurrent( m_pbuffer.getDC(), m_pbuffer.getRenderingContext()) == FALSE)
			wglGetLastError();
	}
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FBO);
	}

	// setup the viewport
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_width, m_height);
}

void RenderPass::endPass()
{
	// pop the viewport bit from beginPass()
	glPopAttrib();

	if (m_bUsePBuffer)
	{
		// make the glut window's rendering context current and draw to the glut window.
		if (wglMakeCurrent(m_last_hdc, m_last_hglrc) == FALSE)
			wglGetLastError();

		// bind the render target texture
		glBindTexture(GL_TEXTURE_2D, m_pFrameTexture->getGLTex());

		// bind the pbuffer to the render texture object
		if (wglBindTexImageARB(m_pbuffer.getHPBuffer(), WGL_FRONT_LEFT_ARB) == FALSE)
			wglGetLastError();
	}
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

void RenderPass::makePBufferCurrent()
{
	ASSERT(m_bUsePBuffer);
	m_pbuffer.makeCurrent();
}