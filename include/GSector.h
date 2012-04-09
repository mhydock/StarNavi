//==============================================================================
// Date Created:		12 March 2011
// Last Updated:		30 March 2012
//
// File name:			GSector.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a chunck of a galaxy, using a file
//						list and dimensions provided by it's patron galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "DirNode.h"
#include "Star.h"

#include "RenderTextureObject.h"

#ifndef GSECTOR
#define GSECTOR

class GSector:public LabeledDrawable
{
	private:
		// Real dimensions of the sector.
		float arc_begin;
		float arc_width;
		float radius;
		float thickness;
		
		// File representation.
		list<Star*> stars;
		list<FileNode*>* files;
		DirNode* root;
		
		float getMinStarDist(Star* s);
		void clearStars();
		
		bool singleSectorMode;
		
	public:
		GSector(DirNode* r, list<FileNode*>* f, float ra, float b, float w, string n = "");
		~GSector();
		
		void buildStars();
		list<Star*>* getStars();
		
		void setName(string n);
		string getName();
		
		float getBiggestStarSize();
		float calcMinArcWidth();
		
		void setRadius(float r);
		void setArcBegin(float b);
		void setArcWidth(float e);
		void setThickness(float t);
		
		float getRadius();
		float getArcBegin();
		float getArcEnd();
		float getArcWidth();
		float getThickness();
		
		list<FileNode*>* getFileList();
		void setDirectory(DirNode *r);
		DirNode* getDirectory();
		
		void setSingleSectorMode(bool m);
		bool isSingleSectorMode();
		
		Star* getSelected();
		
		void activate();
		bool isColliding(float x, float y);
		
		RenderTextureObject* getTexture();
		
		void buildMask();
		void drawMask();
		void draw();
};

#endif
