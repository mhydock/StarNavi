//==============================================================================
// Date Created:		29 June 2011
// Last Updated:		2 July 2011
//
// File name:			TextureObject.h
// Programmer:			Matthew Hydock
//
// File description:	Class made to manage and contain a single texture.
//==============================================================================

#include "global_header.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>

#ifndef TEXOBJ
#define TEXOBJ

class TextureObject
{
	protected:
		int width;
		int height;
		float aspect_ratio;
		
		GLuint format;
		GLuint tex_id;
		GLubyte* tex_data;

	public:
		TextureObject();
		TextureObject(string path);
		TextureObject(SDL_Surface* surface);
		~TextureObject();
		
		void initTexture();
		void clearTexture();
		void setTexture(string path);
		void setTexture(SDL_Surface* surface);
		
		int getWidth();
		int getHeight();
		float getAspectRatio();
		
		void loadTexture();
		void unloadTexture();
		
		static void printGlError();
};

#endif
