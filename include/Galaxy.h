//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		29 March 2012
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that draws a list of files as Stars in
//						a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "GSector.h"
#include "RenderTextureObject.h"

#ifndef GALAXY
#define GALAXY

enum cluster_type{DIRECTORY,NAME,DATE,SIZE,TYPE,TAGS,NONE};

class Galaxy:public LabeledDrawable
{
	private:
		// How the galaxy is drawn (external).
		float side;
		
		// Define the motion and angle of galaxy.
		float rotX;
		float rotY;
		float rotZ;
		float rotSpeed;
		
		// Dimensions of galaxy (internal).
		float diameter;
		float radius;
		float thickness;
		
		// Stores galaxy's files and directories.
		list<GSector*>* sectors;
		list<FileNode*>* files;
		DirNode* root;
		
		// The available tags in this galaxy.
		list<string>* tags;
		
		// The currently selected sector.
		GSector* selected;
		
		// Render to texture.
		RenderTextureObject* texture;
		
		// Label for Star Selection Mode. Static because there will only ever be
		// one of these.
		static DrawText starSelectionLabel;
		static bool isSSLabelInitialized;
		
		// How to cluster files in the galaxy.
		cluster_type cluster_mode;
		
		void buildSectors();
		void buildHierarchy();
		void buildByName();
		void buildByDate();
		void buildBySize();
		void buildByType();
		void buildByTags();
		
		void adjustSectorWidths();
		void clearSectors();
		
		void drawNormalMode();
		void drawStarSelectionMode();
		
		void adjustStarSelectionLabel();
		
	public:
		Galaxy(DirNode* r, list<FileNode*>* f = NULL, cluster_type m = DIRECTORY, string n = "", list<string>* t = NULL);
		~Galaxy();
		
		void setName(string n);
		string getName();
		
		void setRotation(float x, float y);
		void setRotationSpeed(float s);
		float getRotationX();
		float getRotationY();
		float getRotationSpeed();
		
		void setClusterMode(cluster_type m);
		cluster_type getClusterMode();
		
		void setTags(list<string>* t);
		void rebuildTags();
		list<string>* getTags();
		
		void setDirectory(DirNode* r);
		DirNode* getDirectory();
		void setFileList(list<FileNode*>* f);
		list<FileNode*>* getFileList();
		
		list<GSector*>* getSectors();
		
		bool isColliding(float x, float y);
		GSector* getSelected();
		
		void refreshTex();
		void getTexture();
		void draw();
};

#endif
