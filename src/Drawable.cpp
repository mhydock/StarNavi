//==============================================================================
// Date Created:		17 April 2011
// Last Updated:		29 March 2012
//
// File name:			Drawable.cpp
// Programmer:			Matthew Hydock
//
// File description:	Methods for an abstract class. Removes a lot of
//						duplicate code, especially from container-type objects.
//==============================================================================

#include "Drawable.h"

float Drawable::WHITE[4] = {1,1,1,1};

string Drawable::getName()
{
	return name;
}

void Drawable::setName(string n)
{
	name = n;
}

//==============================================================================
// Methods to set and retrieve the object's position and dimensions.
//==============================================================================
void Drawable::setPosition(float x, float y)
// Set position. Two coordinates.
{
	setPosX(x);
	setPosY(y);
}

void Drawable::setPosX(float x)
// Set only x coord.
{
	xPos = x;
}

void Drawable::setPosY(float y)
// Set only y coord.
{
	yPos = y;
}

float Drawable::getPosX()
// Return the object's x coord.
{
	return xPos;
}

float Drawable::getPosY()
// Return the object's y coord.
{
	return yPos;
}
		
//------------------------------------------------------------------------------

void Drawable::setWidth(float w)
// Set the object's width.
{
	width = w;
}

void Drawable::setHeight(float h)
// Set the object's height.
{
	height = h;
}

float Drawable::getWidth()
// Return the object's width.
{
	return width;
}

float Drawable::getHeight()
// Return the object's height.
{
	return height;
}
//==============================================================================


//==============================================================================
// Methods for rudimentary collision detection.
//==============================================================================
bool Drawable::getCollideFlag()
// Return the flag that knows if the object is colliding or not.
{
	return collide_flag;
}

bool Drawable::isColliding(float x, float y)
// Check to see if the given coordinate is inside the drawable. Remember, the
// anchor is in the upper left, and we're using OpenGL, so values are
// increasingly negative as we go down.
{
//	cout << name << endl;
//	cout << xPos << " " << yPos << " " << width << " " << height << endl;
//	cout << x << " " << y << endl;
	
	collide_flag = (x >= xPos) && (x <= xPos+width) && (y >= yPos-height) && (y <= yPos);
		
	return collide_flag;
}
//==============================================================================


//==============================================================================
// Methods to draw primitives
//==============================================================================
void Drawable::setColorArray(float* c, float r, float g, float b, float a)
// Takes an array representing a color, and sets its values.
{
	c[0] = r;
	c[1] = g;
	c[2] = b;
	c[3] = a;
}

void Drawable::drawQuad(float x, float y, float w, float h, float* color, TextureObject* tex)
// Draws a quad of size (w,h) at (x,y), with (x,y) in the upper-left corner.
// Color and texture mapped if they apply. Convenience method, if the quad is
// only one color.
{
	float** c = NULL;
	
	if (color != NULL)
	{
		c = new float*[4];
		c[0] = color;
		c[1] = color;
		c[2] = color;
		c[3] = color;
	}
	
	drawQuad(x,y,w,h,c,tex);
	
	if (c != NULL)
		delete[] c;
}

void Drawable::drawQuad(float x, float y, float w, float h, float** color, TextureObject* tex)
// Draws a quad of size (w,h) at (x,y), with (x,y) in the upper-left corner.
// Color and texture mapped if they apply.
{
	// If texture object given, activate it.
	if (tex != NULL)
		tex->loadTexture();
	else
		glBindTexture(GL_TEXTURE_2D,0);
	
	// If color is given (for each corner), then store it.
	float** c = NULL;
	float* cc = NULL;
	if (color != NULL)
		c = color;
	// Else, generate null color (black, zero alpha).
	else
	{
		cc = new float[4];
		c = new float*[4];
		
		cc[0] = 0;
		cc[1] = 0;
		cc[2] = 0;
		cc[3] = 0;
		
		c[0] = cc;
		c[1] = cc;
		c[2] = cc;
		c[3] = cc;
	}
	
	// Push the current state, scale and shift as appropriate, then draw quad.
	glPushMatrix();
		glTranslatef(x,-y,0);
		glScalef(w,h,1);
		
		glBegin(GL_QUADS);
			glColor4fv(c[0]);	glTexCoord2f(0,0);	glVertex2d(0,0);
			glColor4fv(c[1]);	glTexCoord2f(0,1);	glVertex2d(0,-1);
			glColor4fv(c[2]);	glTexCoord2f(1,1);	glVertex2d(1,-1);
			glColor4fv(c[3]);	glTexCoord2f(1,0);	glVertex2d(1,0);
		glEnd();
	glPopMatrix();
	
	// If the texture object was given, deactivate it.
	if (tex != NULL)
		tex->unloadTexture();
		
	// Cleanup arrays allocated for colors.
	if (c != NULL && c != color)
	{
		delete[] c;
		delete cc;
	}
}

void Drawable::drawSlice(float x, float y, float arc_begin, float arc_end, float r, int steps, float* color)
{
	if (arc_begin == arc_end)
		return;

	if (color != NULL)
		glColor4fv(color);
	else
		glColor4d(1,1,1,1);
		
	glPushMatrix();
		glTranslatef(x,-y,0);
		glScalef(r,r,1);
	
		glBegin(GL_TRIANGLES);
			float arc_length = arc_end-arc_begin;
		
			float i = 0;
			float a, b;
			for (float j = i+1; j < steps; j++)
			{
				a = arc_begin+((i/steps)*arc_length);
				b = arc_begin+((j/steps)*arc_length);
				
				glVertex2d(0,0);
				glVertex2d(cos(a*M_PI/180.0),sin(a*M_PI/180.0));
				glVertex2d(cos(b*M_PI/180.0),sin(b*M_PI/180.0));
			
				i = j;
				
				//cout << a << endl;
			}
		
			a = arc_begin+((i/steps)*arc_length);
			//cout << a << endl;
			
			glVertex2d(0,0);
			glVertex2d(cos(a*M_PI/180.0),sin(a*M_PI/180.0));
			glVertex2d(cos(arc_end*M_PI/180.0),sin(arc_end*M_PI/180.0));
		glEnd();
	glPopMatrix();
}
//==============================================================================


//==============================================================================
// Debug methods
//==============================================================================
void Drawable::printGlError()
// Check to see if there were any gl errors
{
	TextureObject::printGlError();
}
//==============================================================================
