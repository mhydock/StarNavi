//==============================================================================
// Date Created:		29 March 2012
// Last Updated:		29 March 2012
//
// File name:			RenderTextureObject.h
// Programmer:			Matthew Hydock
//
// File description:	Extension of the TextureObject, to encapsulate the
//						render-to-texture process.
//==============================================================================

#ifndef RENDER_TEXTURE_OBJECT
#define RENDER_TEXTURE_OBJECT

#define GL_GLEXT_PROTOTYPES

#define DEFAULT_SIZE 128

#include "TextureObject.h"

class RenderTextureObject:public TextureObject
{
	private:
		GLuint fbo_depth;
		GLuint fbo;

	public:
		RenderTextureObject();
		RenderTextureObject(int w, int h);

		void initTexture();
		
		void buildRenderTexture();
		
		void setWidth(int w);
		void setHeight(int h);
		
		void startRendering();
		void stopRendering();
		
		static void printFramebufferError();
};
#endif
