//==============================================================================
// Date Created:		26 March 2012
// Last Updated:		29 March 2012
//
// File name:			FileNode.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that contains data for a file.
//==============================================================================

#include <sys/stat.h>
#include <dirent.h>

#include "DirNodePrototype.h"
#include "MimeIdentifier.h"

#ifndef FILENODE
#define FILENODE

class FileNode
{
	private:
		static MimeIdentifier mrmime;
		
		string name;
		string mime_type;
		string default_app;
		enum filetype mime_enum;
		struct stat attr;
		list<string> tags;
		
		DirNodePrototype* parent;
		
		void setTags();
		void obtainType();
		
	public:
		FileNode(DirNodePrototype* p, string n);
		~FileNode();
		
		string getName();
		string getPath();
		string getMimetype();
		string getDefaultApp();
		
		void setName(string n);

		DirNodePrototype* getParent();
		void setParent(DirNodePrototype* p);
		
		enum filetype getMimeEnum();
		struct stat getAttributes();
		
		void rebuildTags();
		list<string>* getTags();
};

#endif	
