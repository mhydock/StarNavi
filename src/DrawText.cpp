//==============================================================================
// Date Created:		3 May 2011
// Last Updated:		29 March 2012
//
// File name:			DrawText.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that renders text to a texture.
//==============================================================================

#include "DrawText.h"

DrawText::DrawText(string t,string f,int s,float x,float y,text_align a)
{
	xPos = x;
	yPos = y;

	font_path	= f;
	font_size	= s;
	
	name		= t;
	text		= t;
	align		= a;
	
	setTextColor(1,1,1,1);
	setBackgroundColor(0,0,0,0);
	
	horz_padding = 0;
	vert_padding = 0;

	texture = NULL;

	refreshTexture();
}
	
DrawText::~DrawText()
{
	delete texture;
}

//==============================================================================
// Basic setters and getters.
//==============================================================================
void DrawText::setTextWidth(float w)
{
	width = w;
	height = w/aspect_ratio;
}

float DrawText::getTextWidth()
{
	return width;
}

void DrawText::setTextHeight(float h)
{
	height = h;
	width = h*aspect_ratio;
}

float DrawText::getTextHeight()
{
	return height;
}

float DrawText::getWidth()
{
	return width+2*horz_padding;
}

float DrawText::getHeight()
{
	return height+2*vert_padding;
}

void DrawText::setHorzPadding(float h)
{
	horz_padding = h;
}

float DrawText::getHorzPadding()
{
	return horz_padding;
}
		
void DrawText::setVertPadding(float v)
{
	vert_padding = v;
}

float DrawText::getVertPadding()
{
	return vert_padding;
}
//==============================================================================


//==============================================================================
// Font/text methods.
//==============================================================================
void DrawText::setFontSize(int s)
{
	font_size = s;
}

int DrawText::getFontSize()
{
	return font_size;
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
	name = t;
	text = t;
}

string DrawText::getText()
{
	return text;
}

void DrawText::setAlignment(text_align a)
{
	align = a;
}

text_align DrawText::getAlignment()
{
	return align;
}
//==============================================================================


//==============================================================================
// Color methods
//==============================================================================
void DrawText::setTextColor(float r, float g, float b, float a)
{
	text_color[0] = r;
	text_color[1] = g;
	text_color[2] = b;
	text_color[3] = a;
}

void DrawText::setTextColor(float c[4])
{
	setTextColor(c[0],c[1],c[2],c[3]);
}

float* DrawText::getTextColor()
{
	return text_color;
}

void DrawText::setBackgroundColor(float r, float g, float b, float a)
{
	bg_color[0] = r;
	bg_color[1] = g;
	bg_color[2] = b;
	bg_color[3] = a;
}

void DrawText::setBackgroundColor(float c[4])
{
	setBackgroundColor(c[0],c[1],c[2],c[3]);
}

float* DrawText::getBackgroundColor()
{
	return bg_color;
}
//==============================================================================


//==============================================================================
// Methods for drawing.
//==============================================================================
void DrawText::refreshTexture()
// Using the stored font and text, build a texture.
{
	// Delete the old texture.
	if (texture != NULL)
		delete texture;
	
	// Initialize the texture object.
	texture = new TextureObject();

	// Make SDL surfaces to render text to.
	SDL_Surface* initial = NULL;
	
	// Open the font.
	TTF_Font* font = TTF_OpenFont(font_path.c_str(),font_size);
	SDL_Color fg_color = {255*text_color[0],255*text_color[1],255*text_color[2],255*text_color[3]};
	
	// Make the initial text buffer.	
	initial = TTF_RenderText_Blended(font, text.c_str(), fg_color);
	TTF_CloseFont(font);

	// Load the new texture into the texture object, provided something was
	// actually rendered.
	if (initial != NULL)
	{
		texture->setTexture(initial);
		width			= texture->getWidth();
		height			= texture->getHeight();
		aspect_ratio	= texture->getAspectRatio();
	
		// Free the surface
		SDL_FreeSurface(initial);
	}
	else
	// If no text was rendered, just set everything to the defaults.
	{
		texture			= NULL;
		width			= 0;
		height			= 0;
		aspect_ratio	= 1;
	}
		
//	printGlError();
}
		
void DrawText::draw()
// Draw the text.
{	
	glPushMatrix();
		glTranslatef(xPos,yPos,0);
		
		// Shift the text alignment.
		switch(align)
		{
			case LEFT			: glTranslatef(0,0,0);
								  break;
			case MIDDLE			: glTranslatef(-width/2,0,0);
								  break;
			case RIGHT			: glTranslatef(-width,0,0);
								  break;
		}
		// Draw the solid background.
		drawQuad(0,0,width+2*horz_padding,height+2*vert_padding,bg_color,NULL);
		
		// Make text visible and draw it.
		glTranslatef(0,0,1);
		drawQuad(horz_padding,vert_padding,width,height,WHITE,texture);
	glPopMatrix();
}
//==============================================================================
