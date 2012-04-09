//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		14 March 2012
//
// File name:			Container.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that can contain objects that extend
//						the Drawable class. It draws objects by building a
//						viewport of a requested size in a specified location,
//						and attaches an ortho space to it to draw in.
//==============================================================================

#include "Drawable.h"
#include "StateManager.h"

#ifndef CONTAINER
#define CONTAINER

class Container:public Drawable
{
	private:
		float originalX;
		float originalY;
		float originalW;
		float originalH;
		
		bool rounded;
		bool corners[4];
		int cornerRadius;
		
		bool filled;
		bool lined;
		
		float fill_color[4];
		float line_color[4];
		
		Drawable* content;
		AbstractFunctor* act;
		
		void drawFilled();

	public:
		Container(Drawable* d,AbstractFunctor* fn, float x, float y, float w, float h);
		~Container();
		
		Drawable* getContent();
		
		float getOriginalX();
		float getOriginalY();
		
		void setRounded(bool r);
		bool isRounded();
		
		void setCorners(bool lu, bool ru, bool rd, bool ld);
		
		void setCornerRadius(int r);
		int getCornerRadius();
		
		void setFilled(bool f);
		bool isFilled();
		
		void setLined(bool l);
		bool isLined();
		
		void setFillColor(float r, float g, float b, float a);
		void setLineColor(float r, float g, float b, float a);
		
		void scale(float x, float y);
		void translate(float x, float y);
		
		void activate();
		bool isColliding(float x, float y);
		
		void draw();
};

#endif
