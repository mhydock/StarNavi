//==============================================================================
// Date Created:		29 March 2012
// Last Updated:		29 March 2012
//
// File name:			RenderTextureObject.cpp
// Programmer:			Matthew Hydock
//
// File description:	Implementation of the RenderTextureObject, an extension
//						of the TextureObject, to encapsulate the render-to-
//						texture process.
//==============================================================================

#include "RenderTextureObject.h"

//==============================================================================
// Constructors and deconstructors.
//==============================================================================
RenderTextureObject::RenderTextureObject()
// Creates a RenderTextureObject with default width and height.
{
	tex_id = 0;
	
	tex_data		= NULL;
	width 			= DEFAULT_SIZE;
	height 			= DEFAULT_SIZE;
	aspect_ratio	= 1;
	
	fbo_depth = 0;
	fbo = 0;
}

RenderTextureObject::RenderTextureObject(int w, int h)
// Creates a RenderTextureObject with a specified width and height.
{
	tex_id = 0;
	
	tex_data = NULL;
	
	if (w > 0 && h > 0)
	{
		width			= w;
		height			= h;
		aspect_ratio	= (float)width/(float)height;
	}
	else
	{
		width 			= DEFAULT_SIZE;
		height 			= DEFAULT_SIZE;
		aspect_ratio	= 1;
	}
	
	fbo_depth = 0;
	fbo = 0;
}
//==============================================================================


//==============================================================================
// Dimension management. Changes won't take effect until the texture object is
// reinitialized.
//==============================================================================
void RenderTextureObject::setWidth(int w)
// Set the width of the buffers.
{
	width = w;
}

void RenderTextureObject::setHeight(int h)
// Set the height of the buffers.
{
	height = h;
}
//==============================================================================


//==============================================================================
// Overrides of TextureObject methods.
//==============================================================================
void RenderTextureObject::initTexture()
// Initializes the base texture, along with the framebuffer object and the
// render buffer.
{
//	cout << "initializing the texture\n";
	TextureObject::initTexture();
//	cout << "texture initialized\n";
	
//	cout << "initializing the depth buffer\n";
	// Create a depth buffer for the framebuffer object.
	glGenRenderbuffers(1, &fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
//	cout << "depth buffer initialized\n";
	
	printGlError();
	
//	cout << "preparing texture for rendering...\n";
	// Prepare the texture for rendering.
	glBindTexture(GL_TEXTURE_2D, tex_id);
	if (tex_data != NULL) delete(tex_data);
	tex_data = new GLubyte[width*height*4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	printGlError();
	
//	cout << "initializing the frame buffer object\n";
	// Create and bind the frame buffer object.
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
//	cout << "frame buffer object initialized\n";
	
	printFramebufferError();
}
//==============================================================================


//==============================================================================
// Methods to enable/disable rendering to this texture object.
//==============================================================================
void RenderTextureObject::startRendering()
// Turn on the frame buffer and the render buffer, to prepare for rendering.
{
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void RenderTextureObject::stopRendering()
// Rendering is done, turn off the frame buffer and render buffer.
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
//==============================================================================

void RenderTextureObject::printFramebufferError()
// Check to see if there were any framebuffer errors
{
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
	switch(e)
	{
		case GL_FRAMEBUFFER_UNSUPPORTED						: cout << "format not supported\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	: cout << "missing attachment\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			: cout << "incomplete attachment\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE			: cout << "incomplete multisample\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			: cout << "missing draw buffer\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			: cout << "missing read buffer\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS		: cout << "incomplete layer targets\n";
															  break;
		case GL_FRAMEBUFFER_COMPLETE						: cout << "complete\n";
															  break;
		default												: cout << "mystery error: " << e << endl;
															  break;		
	}
}
