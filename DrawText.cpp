//==============================================================================
// Date Created:		3 May 2011
// Last Updated:		6 May 2011
//
// File name:			DrawText.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that renders text to a texture.
//==============================================================================

#include "DrawText.h"

DrawText::DrawText(string t,string f,int s,float x,float y,anchor_type a)
{
	xPos = x;
	yPos = y;

	font_path	= f;
	text		= t;
	size		= s;

	setColor(1,1,1,1);

	anchor = a;

	rendered_text = 0;

	initTexture();
	refreshTexture();
}
	
DrawText::~DrawText()
{
}

//==============================================================================
// Setters and getters.
//==============================================================================
void DrawText::setWidth(float w)
{
	width = w;
	height = w/aspect_ratio;
}

void DrawText::setHeight(float h)
{
	height = h;
	width = h*aspect_ratio;
}


void DrawText::setFontSize(int s)
{
	size = s;
}

int DrawText::getFontSize()
{
	return size;
}


void DrawText::setFont(string f)
{
	font_path = f;
}

string DrawText::getFont()
{
	return font_path;
}


void DrawText::setText(string t)
{
	text = t;
}

string DrawText::getText()
{
	return text;
}

void DrawText::setColor(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

float* DrawText::getColor()
{
	return color;
}
//==============================================================================


//==============================================================================
// Methods for drawing.
//==============================================================================
void DrawText::initTexture()
// Initialize the object's texture.
{
	//Generate an OpenGL 2D texture from the SDL_Surface*
	glGenTextures(1, &rendered_text);	
	glBindTexture(GL_TEXTURE_2D, rendered_text);
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

void DrawText::refreshTexture()
// Using the stored font and text, build a texture.
{
	// Make SDL surfaces to render text to.
	SDL_Surface* initial = NULL;
	
	// Open the font.
	TTF_Font* font = TTF_OpenFont(font_path.c_str(),size);
	SDL_Color fg_color = {255*color[0],255*color[1],255*color[2],255*color[3]};
	
	// Make the initial text buffer.	
	initial = TTF_RenderText_Blended(font, text.c_str(), fg_color);
	TTF_CloseFont(font);

	// Store the buffer's width and height.
	tex_data = (GLubyte*)(initial->pixels);
	width	= initial->w;
	height	= initial->h;

	// To maintain the text's shape while drawing.
	aspect_ratio = (float)width/(float)height;

	// Bind the texture, set the its pixel map, then release.
	glBindTexture(GL_TEXTURE_2D, rendered_text);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Free the surfaces
	SDL_FreeSurface(initial);
		
//	printGlError();
}
		
void DrawText::draw()
{
	// Turn texture mode.
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glPushMatrix();
		switch(anchor)
		{
			case CENTER			: break;
			case LEFT_UPPER		: glTranslatef(width/2,-height/2,0);
								  break;
			case RIGHT_UPPER	: glTranslatef(-width/2,-height/2,0);
								  break;
			case RIGHT_LOWER	: glTranslatef(-width/2,height/2,0);
								  break;
			case LEFT_LOWER		: glTranslatef(width/2,height/2,0);
								  break;
		}
		
		glTranslatef(xPos,yPos,0);
		glScalef(width/2, height/2,1);
		
		glBindTexture(GL_TEXTURE_2D, rendered_text);
		glBegin(GL_QUADS);
			glColor4f(1,1,1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,1);
			glTexCoord2f(0,1);	glVertex2d(-1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,1);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}
//==============================================================================
