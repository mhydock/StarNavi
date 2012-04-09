//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		29 March 2012
//
// File name:			Drawable.h
// Programmer:			Matthew Hydock
//
// File description:	An abstract class, made to make generic objects that can
//						draw to a framebuffer.
//==============================================================================

#define GL_GLEXT_PROTOTYPES

#include "global_header.h"
#include "TextureObject.h"
#include <GL/gl.h>
#include <GL/glu.h>

#ifndef DRAWABLE
#define DRAWABLE

class Drawable
{
	protected:
		static float WHITE[4];
	
		float xPos;
		float yPos;
		float width;
		float height;
		
		string name;
		
		bool collide_flag;

	public:
		virtual ~Drawable() {};
		
		virtual string getName();
		virtual void setName(string n);
		
		virtual void setPosition(float x, float y);
		virtual void setPosX(float x);
		virtual void setPosY(float y);
		virtual float getPosX();
		virtual float getPosY();
		
		virtual void setWidth(float w);
		virtual void setHeight(float h);
		virtual float getWidth();
		virtual float getHeight();
		
		virtual bool isColliding(float x, float y);
		virtual bool getCollideFlag();

		virtual void activate() {};
		virtual void draw() = 0;

		void setColorArray(float* c, float r, float g, float b, float a);

		void drawQuad(float x, float y, float w, float h, float* color, TextureObject* tex);
		void drawQuad(float x, float y, float w, float h, float** color, TextureObject* tex);
		
		void drawSlice(float x, float y, float arc_begin, float arc_end, float r, int steps, float* color);
		
		void printGlError();
};
#endif
