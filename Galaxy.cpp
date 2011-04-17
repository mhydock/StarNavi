//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		6 April 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a list of files as Stars in a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "Galaxy.h"

//==============================================================================
// Constructors/Deconstructors
//==============================================================================
Galaxy::Galaxy(dirnode *r, list<filenode*> *f, cluster_type m)
{
	cout << "making a galaxy...\n";
	
	if (r != NULL)
		setDirectory(r);
	else
	{
		root = NULL;
		files = f;
	}
	
	xPos = 0;
	yPos = 0;
	side = 0;
		
	diameter = 64.0 * pow((3.0*(double)files->size())/(4.0*M_PI),(1.0/3.0));
	radius = diameter/2;
	thickness = pow(radius*2.0,.5);
	
	setRotation(0,0);
	setRotationSpeed(.02);
	
	rotZ = 0;
	
	mode = DIRECTORY;
	
	sectors = NULL;
	buildSectors();
	
	tex_data = NULL;
	
	initTexture();
	refreshTex();
	
	cout << "galaxy built\n";
}

Galaxy::~Galaxy()
{
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
		delete (*i);

	delete(sectors);
}
//==============================================================================


//==============================================================================
// Methods related to the angle and velocity of the galaxy.
//==============================================================================
void Galaxy::setRotation(float x, float y)
{
	rotX = x;
	rotY = y;
}

void Galaxy::setRotationSpeed(float s)
{
	rotSpeed = s;
}

float Galaxy::getRotationX()
{
	return rotX;
}

float Galaxy::getRotationY()
{
	return rotY;
}

float Galaxy::getRotationSpeed()
{
	return rotSpeed;
}
//==============================================================================


//==============================================================================
// Methods related to sector management.
//==============================================================================
void Galaxy::buildSectors()
{
	cout << "building sectors\n";
	clearSectors();
	
	sectors = new list<GSector*>();
	
	if (mode == DIRECTORY)
		buildHierarchy();
		
	cout << "sectors built\n";
}

void Galaxy::buildHierarchy()
{
	cout << "hierarchy build mode\n";
		
	float arc_begin = 0;
	float arc_end = 360.0*((float)root->files.size()/(float)root->all_files.size());
	sectors->push_back(new GSector(NULL,&(root->files),radius,arc_begin,arc_end));
	cout << "root sector built\n";
	
	cout << "creating sectors for directories\n";
	for (list<dirnode*>::iterator i = root->dirs.begin(); i != root->dirs.end(); i++)
	{
		arc_begin = arc_end;
		arc_end = arc_begin + 360.0*((float)(*i)->all_files.size()/(float)root->all_files.size());
		sectors->push_back(new GSector(*i,NULL,radius,arc_begin,arc_end));
	}
}

void Galaxy::clearSectors()
{
	if (sectors != NULL)
	{
		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			delete (*i);
		delete (sectors);
	}
}
//==============================================================================


void Galaxy::setMode(cluster_type m)
// Set the galaxy's clustering mode.
{
	mode = m;
}

cluster_type Galaxy::getMode()
// Return the galaxy's clustering mode.
{
	return mode;
}

void Galaxy::setDirectory(dirnode *r)
// Set the galaxy's file indexer.
{
	root = r;
	files = &(r->all_files);
}

dirnode* Galaxy::getDirectory()
// Obtain a pointer to the indexer used by the galaxy.
{
	return root;
}


//==============================================================================
// Methods related to drawing.
//==============================================================================
void Galaxy::initTexture()
{
	glGenTextures((GLsizei)1, &texture);	
	glBindTexture(GL_TEXTURE_2D, texture);
//------------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D,0);

	cout << "created texture\n";
}

void Galaxy::refreshTex()
// Update the galaxy texture.
{
	tex_size = (int)(1024.0 * (diameter/500.0));

	cout << diameter << "  " << tex_size << endl;

	// Create a depth buffer for the framebuffer object.
	GLuint fbo_depth;
	glGenRenderbuffers(1, &fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_size, tex_size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);

	printGlError();

	// Prepare the texture for rendering.
	glBindTexture(GL_TEXTURE_2D, texture);
	if (tex_data = NULL) delete(tex_data);
	tex_data = new GLbyte[tex_size*tex_size*4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_size, tex_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	printGlError();
	
	// Create and bind the frame buffer object.
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
	
	//printFramebufferError();

	// Push the viewport to an attribute stack, and render as usual.
	glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,tex_size,tex_size);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-radius,radius,-radius,radius,-thickness,thickness);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			(*i)->draw();
			
		glFlush();
	glPopAttrib();
	
	// Release the buffers, and return to the defaults.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Galaxy::draw()
// Draw the galaxy.
{
	// Set the size and origin of the galaxy, based on the viewport.
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);
	side = (p[2]<p[3])?p[2]:p[3];
	xPos = ((float)p[2])/2.0;
	yPos = ((float)p[3])/2.0;
	
	// Bind the previously rendered texture.
	glBindTexture(GL_TEXTURE_2D, texture);

	glPushMatrix();
		// Rotate the galaxy 
		glRotatef(rotZ,0,0,1);
		glScalef(side,side,1);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
	
		glFlush();
		
		if (selected != NULL) selected->drawMask();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	rotZ += rotSpeed;
		if (rotZ > 360) rotZ -= 360;
	
	glFlush();
}
//==============================================================================


//==============================================================================
// Checking for mouse-overs/clicking.
//==============================================================================
bool Galaxy::isColliding(float x, float y)
{
	selected = NULL;
	
	float localX = x-xPos;
	float localY = y-yPos;
	
	float angle_r = atan2(localY,localX);
	float angle_d = angle_r*(180.0/M_PI);
	float magnitude = sqrt(pow(localX,2.0)+pow(localY,2.0));
	
	if (magnitude > side)
		return collide_flag = false;
	
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
		if ((*i)->isColliding(angle_d, magnitude))
			selected = (*i);
		
	return collide_flag = true;
}

GSector* Galaxy::getSelected()
{
	return selected;	
}
//==============================================================================




