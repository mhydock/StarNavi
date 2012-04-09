//==============================================================================
// Date Created:		28 March 2012
// Last Updated:		29 March 2012
//
// File name:			DirNode.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that contains data for a directory.
//==============================================================================

#include <list>
#include <string>

#ifndef DIRNODE
#define DIRNODE

#include "DirNodePrototype.h"
#include "FileNode.h"

typedef struct dirent dirent;

class DirNode:public DirNodePrototype
{
	private:
		string name;
		DirNode* parent;
		list<FileNode*> files;
		list<DirNode*> dirs;
		
		list<FileNode*>::iterator findFile(string fn);
		list<FileNode*>::iterator findFile(FileNode* f);
		list<DirNode*>::iterator findDirectory(string dn);
		list<DirNode*>::iterator findDirectory(DirNode* d);
		
	public:
		DirNode(DirNode* p, string n);
		~DirNode();
		
		void addFile(string fn);
		void addFile(FileNode* f);
		void addDirectory(string dn);
		void addDirectory(DirNode* d);
		
		FileNode* removeFile(string fn);
		FileNode* removeFile(FileNode* f);
		DirNode* removeDirectory(string dn);
		DirNode* removeDirectory(DirNode* d);
		
		void deleteFile(string fn);
		void deleteFile(FileNode* f);
		void deleteDirectory(string dn);
		void deleteDirectory(DirNode* d);
		
		list<FileNode*>* getFiles();
		list<DirNode*>* getDirectories();
		
		string getName();
		string getPath();
		DirNode* getParent();
		
		void rename(string n);
		void setParent(DirNode* p);
		
		// Convenience method to get all files from this node on down.
		list<FileNode*>* getAllFiles();
};
#endif
