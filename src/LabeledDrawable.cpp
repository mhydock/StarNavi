//==============================================================================
// Date Created:		13 May 2011
// Last Updated:		29 March 2012
//
// File name:			LabeledDrawable.h
// Programmer:			Matthew Hydock
//
// File description:	A special class of drawable items that have	built-in
//						name labels.
//==============================================================================

#include "LabeledDrawable.h"

DrawText* LabeledDrawable::getLabel()
// Get the text object of the star.
{
	return label;
}

void LabeledDrawable::initLabel()
{
	//cout << "initializing label\n";
	
	if (label == NULL)
	{
		label = new DrawText(name);
		label->setBackgroundColor(.2,.2,.2,.25);
		label->setHorzPadding(5);
		label->setVertPadding(4);
	}
	
	//cout << "label initialized\n";
}	

void LabeledDrawable::drawLabel()
// Draw the name of the star.
{
	label->draw();
}
