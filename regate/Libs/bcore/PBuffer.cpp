#include "PBuffer.h"

//TEMP!
extern void wglGetLastError();

PBuffer::PBuffer() : m_hpbuffer(0), m_hdc(0), m_hglrc(0),
	m_width(0), m_height(0),
	m_bShareContext(false),
	m_bShareObjects(false)
{
}

PBuffer::~PBuffer()
{
	free();
}

#define MAX_ATTRIBS     256
#define MAX_PFORMATS    256

bool PBuffer::create(int w, int h, int pixel_format, bool bShareContext, bool bShareObjects)
{
	HDC hdc = wglGetCurrentDC();
	HGLRC hglrc = wglGetCurrentContext();
	wglGetLastError();

	m_bShareContext = bShareContext;
	m_bShareObjects = bShareObjects;

	// Query for a suitable pixel format based on the specified mode.
	int     format;
	int     pformat[MAX_PFORMATS];
	unsigned int nformats;
	int     iattributes[2*MAX_ATTRIBS];
	float   fattributes[2*MAX_ATTRIBS];
	int     nfattribs = 0;
	int     niattribs = 0;

/*	const int iattributes[] = { WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE,
	                           WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	                           WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_FLOAT_ARB,
	                           WGL_RED_BITS_ARB, 16,
	                           WGL_GREEN_BITS_ARB, 16,
	                           WGL_BLUE_BITS_ARB, 16,
	         //                  WGL_ALPHA_BITS_ARB, 16,
	           //                WGL_DEPTH_BITS_ARB, 24,
	                           WGL_BIND_TO_TEXTURE_RGBA_ARB, GL_TRUE,
	                           0 };
	niattribs = 9;*/

	// Attribute arrays must be "0" terminated - for simplicity, first
	// just zero-out the array entire, then fill from left to right.
	memset(iattributes, 0, sizeof(int)*2*MAX_ATTRIBS);
	memset(fattributes, 0, sizeof(float)*2*MAX_ATTRIBS);

	// Since we are trying to create a pbuffer, the pixel format we
	// request (and subsequently use) must be "p-buffer capable".
	iattributes[niattribs  ] = WGL_DRAW_TO_PBUFFER_ARB;
	iattributes[++niattribs] = GL_TRUE;

	// we are asking for a pbuffer that is meant to be bound
	// as an RGBA texture - therefore we need a color plane
	iattributes[++niattribs] = WGL_BIND_TO_TEXTURE_RGBA_ARB;
	iattributes[++niattribs] = GL_TRUE;

	switch (pixel_format)
	{
	case GL_RGBA8:
		iattributes[++niattribs] = WGL_PIXEL_TYPE_ARB;
		iattributes[++niattribs] = WGL_TYPE_RGBA_ARB;
		
		iattributes[++niattribs] = WGL_RED_BITS_ARB;
		iattributes[++niattribs] = 8;
		iattributes[++niattribs] = WGL_GREEN_BITS_ARB;
		iattributes[++niattribs] = 8;
		iattributes[++niattribs] = WGL_BLUE_BITS_ARB;
		iattributes[++niattribs] = 8;
		break;
	case GL_RGBA16F_ARB:
		iattributes[++niattribs] = WGL_PIXEL_TYPE_ARB;
		iattributes[++niattribs] = WGL_TYPE_RGBA_FLOAT_ARB;
		
		iattributes[++niattribs] = WGL_RED_BITS_ARB;
		iattributes[++niattribs] = 16;
		iattributes[++niattribs] = WGL_GREEN_BITS_ARB;
		iattributes[++niattribs] = 16;
		iattributes[++niattribs] = WGL_BLUE_BITS_ARB;
		iattributes[++niattribs] = 16;
		break;
	case GL_RGBA32F_ARB:
		iattributes[++niattribs] = WGL_PIXEL_TYPE_ARB;
		iattributes[++niattribs] = WGL_TYPE_RGBA_FLOAT_ARB;
		
		iattributes[++niattribs] = WGL_RED_BITS_ARB;
		iattributes[++niattribs] = 32;
		iattributes[++niattribs] = WGL_GREEN_BITS_ARB;
		iattributes[++niattribs] = 32;
		iattributes[++niattribs] = WGL_BLUE_BITS_ARB;
		iattributes[++niattribs] = 32;
		break;
	default: Console::print("PBuffer::create(): Unsupported pixel format\n"); ASSERT(0);
	}

	if (m_bShareContext)
    {
		// Get the pixel format for the on-screen window.
		format = GetPixelFormat(hdc);
		if (format == 0)
		{
			Console::print("pbuffer creation error:  GetPixelFormat() failed\n");
			return false;
		}
    }
    else
    {
		if ( !wglChoosePixelFormatARB( hdc, iattributes, fattributes, MAX_PFORMATS, pformat, &nformats ) )
		{
			Console::print("pbuffer creation error:  wglChoosePixelFormatARB() failed.\n");
			return false;
		}
		wglGetLastError();
		if (nformats <= 0)
		{
			Console::print("pbuffer creation error:  Couldn't find a suitable pixel format.\n");
			return false;
		}
		format = pformat[0];
	}

	// Set up the pbuffer attributes
/*	memset(iattributes, 0, sizeof(int)*2*MAX_ATTRIBS);
	niattribs = 0;
	
	// the render texture format is RGBA
	iattributes[niattribs] = WGL_TEXTURE_FORMAT_ARB;
	iattributes[++niattribs] = texFormat;
	
	// the render texture target is GL_TEXTURE_2D
	iattributes[++niattribs] = WGL_TEXTURE_TARGET_ARB;
	iattributes[++niattribs] = WGL_TEXTURE_2D_ARB;
	
	// ask to allocate room for the mipmaps
//	iattributes[++niattribs] = WGL_MIPMAP_TEXTURE_ARB;
//	iattributes[++niattribs] = TRUE;
	
	// ask to allocate the largest pbuffer it can, if it is
	// unable to allocate for the width and height
	iattributes[++niattribs] = WGL_PBUFFER_LARGEST_ARB;
	iattributes[++niattribs] = FALSE;*/
	
	// Choose a pixel format for the floating-point framebuffer
	const int fbuffer_flags[] = { WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,
	                              WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
	                              0 };

	// Create the p-buffer.
	m_hpbuffer = wglCreatePbufferARB( hdc, format, w, h, fbuffer_flags /*iattributes*/ );
	if (m_hpbuffer == 0)
	{
		Console::print("pbuffer creation error:  wglCreatePbufferARB() failed\n");
		wglGetLastError();
		return false;
	}
	wglGetLastError();

	// Get the device context.
	m_hdc = wglGetPbufferDCARB(m_hpbuffer);
	if (m_hdc == 0)
	{
		Console::print("pbuffer creation error:  wglGetPbufferDCARB() failed\n");
		wglGetLastError();
		return false;
	}
	wglGetLastError();

	if (m_bShareContext)
	{
		// share the context
		// Since the device contexts are compatible (i.e. same pixelformat),
        // we should be able to use the same gl rendering context.
		m_hglrc = hglrc;
	}
	else
	{
		// Create a gl context for the p-buffer.
		m_hglrc = wglCreateContext(m_hdc);
		if (m_hglrc == 0)
		{
			Console::print("pbuffer creation error:  wglCreateContext() failed\n");
			wglGetLastError();
			return false;
		}
		wglGetLastError();
	}
	
	if (m_bShareObjects)
	{
		if(!wglShareLists(hglrc, m_hglrc))
		{
			Console::print("pbuffer: wglShareLists() failed\n");
			return false;
		}
	}

	// Determine the actual width and height we were able to create.
	wglQueryPbufferARB( m_hpbuffer, WGL_PBUFFER_WIDTH_ARB, &m_width );
	wglQueryPbufferARB( m_hpbuffer, WGL_PBUFFER_HEIGHT_ARB, &m_height );

	Console::print("Created P-Buffer - width: %d, height: %d\n", m_width, m_height);

	return true;
}

void PBuffer::free()
{
	if ( m_hpbuffer )
    {
        // Check if we are currently rendering in the pbuffer
        if (wglGetCurrentContext() == m_hglrc )
            wglMakeCurrent(0,0);

        // delete the pbuffer context
        wglDeleteContext( m_hglrc );
        wglReleasePbufferDCARB( m_hpbuffer, m_hdc );
        wglDestroyPbufferARB( m_hpbuffer );
        m_hpbuffer = 0;
    }
}

void PBuffer::makeCurrent()
{
	if (wglMakeCurrent( m_hdc, m_hglrc) == FALSE)
		wglGetLastError();
}