//==============================================================================
// Date Created:		5 March 2011
// Last Updated:		29 March 2012
//
// File name:			DirTree.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that allows for the creation and
//						navigation of a tree-type structure designed for file
//						system indexing.
//==============================================================================

#include "global_header.h"
#include "MimeIdentifier.h"


#ifndef DIRTREE
#define DIRTREE

#include "DirNode.h"

class DirTree
{
	private:
		int numfiles;
		DirNode* root;
		
	public:
		DirTree(string s);
		~DirTree();
		
		void add(string p, string n);
		DirNode* getDir(string p);
		FileNode* getFile(string p, string n);
		
		void clearTree();
		void dropBranch(DirNode* d);
		
		list<FileNode*>* getAllFilesFrom(DirNode* d);
		list<FileNode*>* getFileList();
		string getRootPath();
		DirNode* getRootNode();
		int getNumFiles();
};

#endif
