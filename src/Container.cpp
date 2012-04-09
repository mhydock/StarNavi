//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		14 March 2012
//
// File name:			Container.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that can contain objects that extend
//						the Container class. It draws objects by building a
//						viewport of a requested size in a specified location,
//						and attaches an ortho space to it to draw in.
//==============================================================================

#include "Container.h"

Container::Container(Drawable* d, AbstractFunctor* fn, float x, float y, float w, float h)
{
	act = NULL;
	
	content = d;
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	act = fn;
	
	originalX = x;
	originalY = y;
	originalW = w;
	originalH = h;
	
	rounded = false;
	filled = false;
	lined = true;
	
	setLineColor(1,1,1,1);
	setFillColor(0,0,0,0);
}

Container::~Container()
{
	delete content;
	delete act;
}

	
Drawable* Container::getContent()
{
	return content;
}


float Container::getOriginalX()
{
	return originalX;
}

float Container::getOriginalY()
{
	return originalY;
}

//==============================================================================
// Drawing style getters and setters.
//==============================================================================
void Container::setRounded(bool r)
// Turns rounded corners mode on or off, and makes all corners rounded or square.
{
	if (r)
		setCorners(true,true,true,true);
	else
		setCorners(false,false,false,false);
}

bool Container::isRounded()
// Returns whether rounded mode is on or not.
{
	return rounded;
}

void Container::setCorners(bool lu, bool ru, bool rd, bool ld)
// Selectively set corner rounding.
{
	corners[0] = lu;
	corners[1] = ru;
	corners[2] = rd;
	corners[3] = ld;
	
	rounded = lu || ru || rd || ld;
}

void Container::setCornerRadius(int r)
// Set the size of the corner radius.
{
	cornerRadius = r;
}

int Container::getCornerRadius()
// Return the size of the corner radius.
{
	return cornerRadius;
}

void Container::setFilled(bool f)
{
	filled = f;
}

bool Container::isFilled()
{
	return filled;
}
		
void Container::setLined(bool l)
{
	lined = l;
}

bool Container::isLined()
{
	return lined;
}
		
void Container::setFillColor(float r, float g, float b, float a)
{
	setColorArray(fill_color,r,g,b,a);
}

void Container::setLineColor(float r, float g, float b, float a)
{
	setColorArray(line_color,r,g,b,a);
}
//==============================================================================


//==============================================================================
// Manage the shape and size.
//==============================================================================
void Container::scale(float x, float y)
// Scale the container's width from the original width, based on some ratios for
// x and y scaling.
{
	width = originalW * x;
	height = originalH * y;
}

void Container::translate(float x, float y)
// Shift the container's position by x and y, based on the original position.
{
	xPos = originalX + x;
	yPos = originalY + y;
}
//==============================================================================

void Container::activate()
// Call the container's functor.
{
	if (act != NULL)
		act->Call();
}
			
bool Container::isColliding(float x, float y)
// Check if mouse is colliding with container. If so, set flag, and see if the
// mouse is also colliding with the content.
{
	collide_flag = (x >= xPos) && (x <= xPos+width) && (y >= yPos) && (y <= yPos+height);
	
	float localX = x-xPos;
	float localY = y-yPos;
	
	localY -= height;

	if (content != NULL)
		content->isColliding(localX,localY);
		
	return collide_flag;
}

void Container::drawFilled()
{
	if (!rounded)
	{
		drawQuad(0,0,width,height,fill_color,NULL);
		return;
	}
	else
	{
		// Draw the cross shape of the box without corners.
		drawQuad(cornerRadius,0,width-cornerRadius*2,height,fill_color,NULL);
		drawQuad(0,cornerRadius,cornerRadius,height-cornerRadius*2,fill_color,NULL);
		drawQuad(width-cornerRadius,cornerRadius,cornerRadius,height-cornerRadius*2,fill_color,NULL);
		
		// Draw each corner individually.
		// Upper Left corner.
		if (corners[0])
			drawSlice(cornerRadius,cornerRadius,90.0,180.0,cornerRadius,10,fill_color);
		else
			drawQuad(0,0,cornerRadius,cornerRadius,fill_color,NULL);
		
		// Upper Right corner.
		if (corners[1])
			drawSlice(width-cornerRadius,cornerRadius,0.0,90.0,cornerRadius,10,fill_color);
		else
			drawQuad(width-cornerRadius,0,cornerRadius,cornerRadius,fill_color,NULL);
			
		// Lower Right corner.
		if (corners[2])
			drawSlice(width-cornerRadius,height-cornerRadius,270.0,360.0,cornerRadius,10,fill_color);
		else
			drawQuad(width-cornerRadius,height-cornerRadius,cornerRadius,cornerRadius,fill_color,NULL);
		
		// Lower Left corner.
		if (corners[3])
			drawSlice(cornerRadius,height-cornerRadius,180.0,270.0,cornerRadius,10,fill_color);
		else
			drawQuad(0,height-cornerRadius,cornerRadius,cornerRadius,fill_color,NULL);
	}
}

void Container::draw()
// Create the new viewport, set the world mode, and draw the contained object.
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0,width,-height,0,-100,100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glViewport(xPos,yPos,width,height);
	
	if (filled)
	{
		glPushMatrix();
			glTranslatef(0,0,-50);	
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			drawFilled();
		glPopMatrix();
	}
	
	if (lined)
	{
		glPushMatrix();
			glTranslatef(0,0,100);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				drawQuad(0,0,width,height,line_color,NULL);
		glPopMatrix();
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (content != NULL)
		content->draw();
	
	collide_flag = false;
}
