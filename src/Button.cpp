//==============================================================================
// Date Created:		23 April 2011
// Last Updated:		14 March 2012
//
// File name:			Button.cpp
// Programmer:			Matthew Hydock
//
// File description:	GUI component to draw a button to screen. Can be static
//						or interactive, and calls a functor when clicked.
//==============================================================================

#include "Button.h"

Button::Button(string l, AbstractFunctor* f, float x, float y, float w, float h, bool active)
// Make a button with a label l, that performs action f when clicked.
{
	name = l;
	act = f;
	
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	interactive = active;
	text = new DrawText(name);
	text->setPosition(xPos,yPos);
	
	outline_color = new float[4];
	grad1_color = new float[4];
	grad2_color = new float[4];
	hover_color = new float[4];
	
	setColorArray(outline_color,.1,.1,.1,1);
	setColorArray(grad1_color,.15,.15,.15,0);
	setColorArray(grad2_color,.25,.25,.25,1);
	setColorArray(hover_color,0,0,0,.4);
	
	horz_padding = 2;
	vert_padding = 2;
	
	collide_flag = false;
}

Button::~Button()
{
	delete text;
	delete outline_color;
	delete grad1_color;
	delete grad2_color;
	delete hover_color;
}


bool Button::isInteractive()
// Is the button clickable?
{
	return interactive;
}

void Button::setInteract(bool c)
// Set whether the button is clickable or not.
{
	interactive = c;
}

		
void Button::setAction(AbstractFunctor* f)
// Set the button's action.
{
	act = f;
}

void Button::activate()
// Perform the button's assigned action.
{
//	cout << "button activated\n";
	act->Call();
}

void Button::draw()
// Draw a rectangle with a gradient, dark on the far left and right, white in
// the middle. Try to center the text.
{
	float** grad = new float*[4];
	grad[0] = grad1_color;
	grad[1] = grad1_color;
	grad[2] = grad2_color;
	grad[3] = grad2_color;

	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glPushMatrix();
		glTranslatef(xPos,yPos,0);
		// Draw the slightly larger rectangle that will become the outline.
		drawQuad(0,0,width,height,outline_color,NULL);
	
		// Draw the gradient that will be the button.
		glPushMatrix();
			glTranslatef(0,0,1);
			drawQuad(horz_padding,vert_padding,width/2-horz_padding,height-2*vert_padding,grad,NULL);
			
			glTranslatef(width,0,0);
			glScalef(-1,1,1);
			drawQuad(horz_padding,vert_padding,width/2-horz_padding,height-2*vert_padding,grad,NULL);
		glPopMatrix();
		// Done drawing the button.

		// Draw the text. This takes up two layers.
		glPushMatrix();
			glTranslatef(width/2,-(height-text->getHeight())/2,2);
			text->draw();
		glPopMatrix();
		// Done drawing text.
	
		// If mousing over a button, draw a shadow.
		if (collide_flag)
		{
			glPushMatrix();
				glTranslatef(0,0,4);
				drawQuad(0,0,width,height,hover_color,NULL);
			glPopMatrix();
		}
	glPopMatrix();
	
	// Turn off blending.
	glDisable(GL_BLEND);
	
	delete[] grad;
}
