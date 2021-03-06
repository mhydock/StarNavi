//==============================================================================
// Date Created:		6 May 2011
// Last Updated:		13 May 2011
//
// File name:			StatusBar.h
// Programmer:			Matthew Hydock
//
// File description:	A header for a simple status bar that displays the
//						current directory's name and the number of files in that
//						directory tree.
//==============================================================================

#include "DrawText.h"
#include "StateManager.h"

#ifndef STATUS_BAR
#define STATUS_BAR

class StatusBar:public Drawable
{
	private:
		StateManager* state;
		Galaxy* curr;
		DrawText* directory;
		DrawText* num_files;
		
	public:
		StatusBar(StateManager* sm);
		~StatusBar();
		
		void setStateManager(StateManager* sm);
		StateManager* getStateManager();
		
		void refreshState();
		
		void draw();
};
#endif
