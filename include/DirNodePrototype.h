//==============================================================================
// Date Created:		28 March 2012
// Last Updated:		29 March 2012
//
// File name:			DirNodePrototype.h
// Programmer:			Matthew Hydock
//
// File description:	A virtual class that exists just so that files can have
//						knowledge of the directory they are in. Needed because
//						there was a loop in class dependencies (Dirnode needed
//						Filenode and Filenode needed Dirnode).						
//==============================================================================

#include <string>

using namespace std;

#ifndef DIRNODE_PROTOTYPE
#define DIRNODE_PROTOTYPE

class DirNodePrototype
{
	public:
		virtual ~DirNodePrototype() {};
		
		virtual string getName() = 0;
		virtual string getPath() = 0;
		virtual DirNodePrototype* getParent() = 0;
};

#endif
