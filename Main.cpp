//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		17 April 2011
//
// File name:			MainClass.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class to test the functionality of the Galaxy	drawing
//						class. Renamed from test_main.cpp, now that there's
//						enough to work with to stop calling it a test file.
//==============================================================================

#include <GL/glut.h>
#include "Container.h"
#include "StateManager.h"


#define START_W 800
#define START_H 600

GLuint Star::star_texture = 0;
list<Container*> containers;
int oldW = START_W, oldH = START_H;

void init()
// Set clear color and shading model, initialize variables, and make menu.
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.01);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}

void display()
{	
	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
		(*i)->draw();
	
	glFlush();
	glutSwapBuffers();
}

void idleFunc()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{	
	float wRatio = (float)w/(float)START_W;
	float hRatio = (float)h/(float)START_H;
	
	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
	{
		(*i)->scale(wRatio, hRatio);
		
		float oldX = (*i)->getPosX();
		float oldY = (*i)->getPosY();
		
		float xOffset = (oldX * wRatio)-oldX;
		float yOffset = (oldY * hRatio)-oldY;
		
		(*i)->translate(xOffset,yOffset);
	}
	
	oldW = w;
	oldH = h;
	
	cout << oldW << "  " << oldH << endl;
	
	glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
	int newY = oldH-y;
	
	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
		if ((*i)->isColliding(x,newY))
			(*i)->activate();
}	

void mouseHover(int x, int y)
{
	int newY = oldH-y;
	
	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
		(*i)->isColliding(x,newY);
}

int main(int argc, char *argv[])
{
	// set up main window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(oldW, oldH);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("StarNavi");
	
	init();

	// Set up the galaxy.
	string path;
	
	if (argc > 2)
		return 1;
		
	if (argc == 1)
		path = "./";
	else
		path = (string)argv[1];
	
	StateManager *sm = new StateManager(path);
	Functor<StateManager> *f = new Functor<StateManager>(sm, &StateManager::navigate);
	Container *c = new Container(sm,f,200,0,600,525);
	
	containers.push_back(c);

	// register display methods
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(reshape);
	
	// register input methods.
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseHover);
	
	glutMainLoop();
	
	return 0;
}
