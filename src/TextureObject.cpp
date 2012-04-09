//==============================================================================
// Date Created:		29 June 2011
// Last Updated:		25 March 2012
//
// File name:			TextureObject.h
// Programmer:			Matthew Hydock
//
// File description:	Class made to manage and contain a single texture.
//==============================================================================

#include "TextureObject.h"

//==============================================================================
// Constructors and Deconstructors
//==============================================================================
TextureObject::TextureObject()
{
	tex_id = 0;
	
	tex_data		= NULL;
	width			= 0;
	height			= 0;
	aspect_ratio	= 1;
}
	
TextureObject::TextureObject(string path)
{
	tex_id = 0;
	tex_data = NULL;
	setTexture(path);
}

TextureObject::TextureObject(SDL_Surface* surface)
{
	tex_id = 0;
	tex_data = NULL;
	setTexture(surface);
}

TextureObject::~TextureObject()
{
	clearTexture();
}
//==============================================================================


//==============================================================================
// Texture management.
//==============================================================================
void TextureObject::initTexture()
// Initialize the texture object.
{
	glGenTextures(1, &tex_id);	
	glBindTexture(GL_TEXTURE_2D, tex_id);
//------------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::clearTexture()
// Delete the OpenGL texture object, and reset the texture's internal variables.
// WARNING: THIS REMOVES THE ASSOCIATED IMAGE DATA FROM MEMORY.
{
	if (tex_id == 0)
		return;
		
	glDeleteTextures(1,&tex_id);
	tex_id = 0;
	
	if (tex_data != NULL)
		delete tex_data;
		
	tex_data		= NULL;
	width			= 0;
	height			= 0;
	aspect_ratio	= 1;
	
}

void TextureObject::setTexture(string path)
// Load the desired image using SDL's Image library, then map to OpenGL texture.
{
	// Reset the texture object.
	clearTexture();
	initTexture();
	
	// Attempt to load the image file.
	SDL_Surface* temp = IMG_Load(path.c_str());
	
	// If the file couldn't be loaded, create an empty texture object.
	if (!temp)
	{
		fprintf(stderr, "Error: '%s' could not be opened: %s\n", path.c_str(), IMG_GetError());
		
		tex_data		= NULL;
		width			= 0;
		height			= 0;
		aspect_ratio	= 1;
	}
	
	// If the file was loaded, preserve its dimensions and save its data.
	else
	{
//		if(SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(temp->format, COLORKEY)) == -1)
//			fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());

		width			= temp->w;
		height			= temp->h;
		aspect_ratio	= (float)width/(float)height;
		
		// Properly store the texture data by copying it.
		tex_data		= new GLubyte[width*height*4];
		memcpy(tex_data,temp->pixels,sizeof(GLubyte)*width*height*4);
	}
	
	// Bind the texture, set the its pixel map, then release.
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(temp);
}	

void TextureObject::setTexture(SDL_Surface* surface)
// Apply a new texture using an SDL Surface.
{
	// Reset the texture object.
	clearTexture();
	initTexture();
	
	// Store the buffer's width, height and aspect ratio.
	width			= surface->w;
	height			= surface->h;
	aspect_ratio	= (float)width/(float)height;
	
	// Properly store the texture data by copying it.
	tex_data		= new GLubyte[width*height*4];
	memcpy(tex_data,surface->pixels,sizeof(GLubyte)*width*height*4);

	// Bind the texture, set the its pixel map, then release.
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
}
//==============================================================================


//==============================================================================
// Getters (values are set through texture loading).
//==============================================================================	
int TextureObject::getWidth()
{
	return width;
}

int TextureObject::getHeight()
{
	return height;
}

float TextureObject::getAspectRatio()
{
	return aspect_ratio;
}
//==============================================================================


//==============================================================================
// Loading and unloading methods
//==============================================================================
void TextureObject::loadTexture()
// Bind the texture, and turn on texture mode.
{
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void TextureObject::unloadTexture()
// Unbind the texture, and turn off texture mode.
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
//==============================================================================

void TextureObject::printGlError()
// Check to see if there were any gl errors
{
	switch(glGetError())
	{
		case GL_INVALID_ENUM		: cout << "invalid enum\n";
									  break;
		case GL_INVALID_VALUE		: cout << "invalid value\n";
									  break;
		case GL_INVALID_OPERATION	: cout << "invalid operation\n";
									  break;
		case GL_STACK_OVERFLOW		: cout << "stack overflow\n";
									  break;
		case GL_STACK_UNDERFLOW		: cout << "stack underflow\n";
									  break;
		case GL_OUT_OF_MEMORY		: cout << "out of memory\n";
									  break;
		case GL_TABLE_TOO_LARGE		: cout << "table too large\n";
									  break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: cout << "invalid framebuffer operation\n";
									  break;
		default						: cout << "no error\n";
	}
}
