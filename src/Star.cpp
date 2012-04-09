//==============================================================================
// Date Created:		15 February 2011
// Last Updated:		26 March 2012
//
// File name:			Star.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class to draw stars. Star size, color, and intensity
//						are all based on associated file attributes.
//==============================================================================

#include "Star.h"

TextureObject* Star::star_texture = NULL;
bool Star::draw_textured = true;

//==============================================================================
// Constructor/Deconstructor.
//==============================================================================
Star::Star(FileNode* f)
// Construct a star, and connect it to a filenode, from which metadata will be
// pulled and the star's attributes will be defined.
{
	file = f;
	name = f->getName();
	
	recalc();
	
	label = NULL;
	
	if (star_texture == NULL)	star_texture = new TextureObject("./images/star2.png");
}

Star::~Star()
{
}
//==============================================================================


//==============================================================================
// Automatic/private methods.
//==============================================================================
void Star::calculateRadius()
// Set the radius of the star based on the size of the file.
{
	radius = log10((float)(file->getAttributes().st_size)+1)/log10(1000.0) + 1;
	diameter = radius * 2;
}

void Star::determineColor()
// Uses the attached file's type to set the star's color.
{
	switch (file->getMimeEnum())
	{
		case BIN:		setColorArray(color,0.0,0.0,1.0,1.0);		// blue
						break;
		case APP:		setColorArray(color,0.5,0.5,1.0,1.0);		// light blue
						break;
		case AUDIO:		setColorArray(color,1.0,0.0,0.0,1.0);		// red
						break;
		case IMAGE:		setColorArray(color,1.0,1.0,0.0,1.0);		// yellow
						break;
		case VIDEO:		setColorArray(color,1.0,.25,0.0,1.0);		// red-orange
						break;
		case TEXT:		setColorArray(color,1.0,1.0,1.0,1.0);		// white
						break;
		case UNKNOWN:	setColorArray(color,0.5,0.3,0.0,1.0);		// brown
						break;
	}
}

void Star::recalc()
// In the case that the file has changed in some way, refresh the star data.
{
	calculateRadius();
	determineColor();
}
//==============================================================================


//==============================================================================
// State setters.
//==============================================================================
void Star::setRadius(float r)
// Set the radius of the star.
{
	radius = r;
	diameter = radius * 2;
}

void Star::setDiameter(float d)
// Set the diameter of the star.
{
	diameter = d;
	radius = diameter/2;
}

void Star::setDistance(float d)
// Set the distance from the origin.
{
	distance = d;
	
	xPos = distance*cos(angle*M_PI/180);
	yPos = distance*sin(angle*M_PI/180);
}

void Star::setAngle(float a)
// Set the angle around the origin.
{
	angle = a;
	
	xPos = distance*cos(angle*M_PI/180);
	yPos = distance*sin(angle*M_PI/180);
}

void Star::setDepth(float d)
// Set the distance from the camera.
{
	depth = d;
}
//==============================================================================


//==============================================================================
// State getters.
//==============================================================================
string Star::getName()
// Get the name of the star.
{
	return file->getName();
}

float Star::getRadius()
// Get the radius of the star.
{
	return radius;
}

float Star::getDiameter()
// Get the diameter of the star.
{
	return diameter;
}

float Star::getDistance()
// Get the distance from the origin.
{
	return distance;
}

float Star::getAngle()
// Get the angle around the origin.
{
	return angle;
}

float Star::getDepth()
// Get the distance from the camera.
{
	return depth;
}
//==============================================================================


//==============================================================================
// Convenience methods.
//==============================================================================
void Star::randomPosition(float a1, float a2, float dis1, float dis2, float dep1, float dep2)
// Generate and assign a random position to this star within user defined ranges
{
	MTRand mtrand;
	
	angle		= mtrand.rand(a2-a1)+a1;
	distance	= mtrand.rand(dis2-dis1)+dis1;
	depth		= mtrand.rand(dep2-dep1)+dep1;
	
	xPos = distance*cos(angle*M_PI/180);
	yPos = distance*sin(angle*M_PI/180);
}

void Star::setPosition(float a, float dis, float dep)
// Manually set the position of the star.
{
	angle = a;
	distance = dis;
	depth = dep;
	
	xPos = distance*cos(angle*M_PI/180);
	yPos = distance*sin(angle*M_PI/180);
}
//==============================================================================


//==============================================================================
// Methods for user interaction.
//==============================================================================
void Star::activate()
// Try to open the file with the default app.
{
	int pid = fork();
	if (pid == 0)
	{
		string f = file->getPath() + file->getName();
		char* args[3];
//		args[0] = const_cast<char*>(file->getDefaultApp().c_str());
		args[0] = const_cast<char*>("xdg-open");
		args[1] = const_cast<char*>(f.c_str());
		args[2] = (char*)0;
		
		cout << args[0] << " " << args[1] << endl;
		cout << "Launching child process" << endl;
		execvp(const_cast<char*>(args[0]),args);
		cout << "Child process complete" << endl;
	}
	else
		cout << "parent process\n";
}

bool Star::isColliding(float x, float y)
// If the mouse is within the perimeter of a star.
{
	collide_flag = y <= radius;
	
//	cout << y << endl;
	
//	if (collide_flag)
//		cout << "colliding with file " << file->name << endl;
	
	return collide_flag;
}
//==============================================================================


//==============================================================================
// Drawing methods.
//==============================================================================
void Star::setTexturedDrawMode()
{
	draw_textured = true;
}

void Star::setPointDrawMode()
{
	draw_textured = false;
}

void Star::draw()
// Default draw function.
{
//	if (draw_textured)
		drawTextured();
//	else
//		drawPoint();
}

void Star::drawTextured()
// Draw the star onto the current framebuffer using a texture.
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
		glRotatef(angle,0,0,1);
		glTranslatef(distance,0,depth);
		glTranslatef(-radius,radius,0);
		drawQuad(0,0,diameter,diameter,color,star_texture);	
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Star::drawPoint()
// Draw the star onto the current framebuffer using a point.
{
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPointSize(diameter);
	
	glPushMatrix();
		glRotatef(angle,0,0,1);
		glTranslatef(distance,0,depth);
		glColor4fv(color);
		
		glBegin(GL_POINTS);
			glVertex3f(0,0,0);
		glEnd();
	glPopMatrix();
	
	glDisable(GL_BLEND);
}
//==============================================================================
