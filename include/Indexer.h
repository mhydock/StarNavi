//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		29 March 2012
//
// File name:			Indexer.h
// Programmer:			Matthew Hydock
//
// File description:	Class definition for file indexer built in C++.
//==============================================================================

#include "global_header.h"
#include "DirTree.h"

#ifndef INDEXER
#define INDEXER

class Indexer
{
	private:
		DirTree* dir_tree;
		
		void build(string dir);
		
	public:
		Indexer(string root_path);
		~Indexer();
		
		void build();
		void changeRoot(string new_root);
		void clearTree();
		DirTree* getDirectoryTree();
		list<FileNode*>* getFileList();
};

#endif
