//==============================================================================
// Date Created:		16 May 2011
// Last Updated:		14 March 2012
//
// File name:			ListItem.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class that acts as a selectable text item in a
//						graphical list.
//==============================================================================

#include "ListItem.h"

//==============================================================================
// Constructor and Deconstructor.
//==============================================================================
ListItem::ListItem(string t, string f, int size, float x, float y, text_align a)
{
	name = t;
	text = new DrawText(t,f,size,x,y,a);
	
	xPos = x;
	yPos = y;
	width = text->getTextWidth();
	height = text->getTextHeight();
	
	selected = false;
	
	setHoverColor(1,1,1,.25);
	setSelectionColor(1,1,1,.5);
	
	setDefaultTextColor(text->getTextColor());
	setSelectedTextColor(0,0,0,1);
}

ListItem::~ListItem()
{}
//==============================================================================


//==============================================================================
// Setters and getters.
//==============================================================================
void ListItem::setPosition(float x, float y)
{
	Drawable::setPosition(x,y);
	text->setPosition(x,y);
}

void ListItem::setPosX(float x)
{
	Drawable::setPosX(x);
	text->setPosX(x);
}

void ListItem::setPosY(float y)
{
	Drawable::setPosY(y);
	text->setPosY(y);
}
		
void ListItem::setText(string t)
{
	name = t;
	text->setText(t);
}

string ListItem::getText()
{
	return text->getText();
}

DrawText* ListItem::getTextObject()
{
	return text;
}

void ListItem::toggleSelected()
{
	selected = !selected;
	
	if (selected)
		text->setTextColor(getSelectedTextColor());
	else if (!selected)
		text->setTextColor(getDefaultTextColor());
	
	text->refreshTexture();
}

bool ListItem::isSelected()
{
	return selected;
}
//==============================================================================


//==============================================================================
// User interaction methods
//==============================================================================
void ListItem::activate()
{
	toggleSelected();
}
//==============================================================================


//==============================================================================
// Color manipulation
//==============================================================================
void ListItem::setHoverColor(float r, float g, float b, float a)
{
//	cout << r << " " << g << " " << b << " " << a << endl;
	
	hoverColor[0] = r;
	hoverColor[1] = g;
	hoverColor[2] = b;
	hoverColor[3] = a;
}

void ListItem::setHoverColor(float c[4])
{
	setHoverColor(c[0],c[1],c[2],c[3]);
}

float* ListItem::getHoverColor()
{
	return hoverColor;
}

void ListItem::setSelectionColor(float r, float g, float b, float a)
{
//	cout << r << " " << g << " " << b << " " << a << endl;
	
	selectionColor[0] = r;
	selectionColor[1] = g;
	selectionColor[2] = b;
	selectionColor[3] = a;
}

void ListItem::setSelectionColor(float c[4])
{
	setSelectionColor(c[0],c[1],c[2],c[3]);
}

float* ListItem::getSelectionColor()
{
	return selectionColor;
}

void ListItem::setSelectedTextColor(float r, float g, float b, float a)
{
//	cout << r << " " << g << " " << b << " " << a << endl;

	selectedTextColor[0] = r;
	selectedTextColor[1] = g;
	selectedTextColor[2] = b;
	selectedTextColor[3] = a;
}

void ListItem::setSelectedTextColor(float c[4])
{
	setSelectedTextColor(c[0],c[1],c[2],c[3]);
}

float* ListItem::getSelectedTextColor()
{
	return selectedTextColor;
}

void ListItem::setDefaultTextColor(float r, float g, float b, float a)
{
//	cout << r << " " << g << " " << b << " " << a << endl;

	defaultTextColor[0] = r;
	defaultTextColor[1] = g;
	defaultTextColor[2] = b;
	defaultTextColor[3] = a;
}

void ListItem::setDefaultTextColor(float c[4])
{
	setDefaultTextColor(c[0],c[1],c[2],c[3]);
}

float* ListItem::getDefaultTextColor()
{
	return defaultTextColor;
}
//==============================================================================


//==============================================================================
// Draw method
//==============================================================================	
void ListItem::draw()
{
//	cout << "drawing a list item: " << name << endl;
	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		if (collide_flag || selected)
		{
			if (selected) drawQuad(xPos,yPos,width,height,selectionColor,NULL);
			if (collide_flag) drawQuad(xPos,yPos,width,height,hoverColor,NULL);
		}
	
		glPushMatrix();
			glTranslatef(5,0,2);
			text->draw();
		glPopMatrix();
	glPopMatrix();
	
	// Turn off blending.
	glDisable(GL_BLEND);
}
//==============================================================================
