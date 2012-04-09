//==============================================================================
// Date Created:		5 March 2011
// Last Updated:		29 March 2012
//
// File name:			DirTree.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class that allows for the creation and navigation of a
//						tree-type structure designed for file system indexing.
//==============================================================================

#include "DirTree.h"

//==============================================================================
// Constructors/Deconstructors
//==============================================================================
DirTree::DirTree(string dir = "./")
// Creates a directory tree with the root node at dir.
{
//	cout << "making a dirtree\n";

	numfiles = 0;
	
//	cout << "made the mime identifier\n";
	
	if (dir.compare("./") != 0)	
		root = new DirNode(NULL,dir);
	else
	{
		cout << "trying to identify directory\n";
		char* currpath = getcwd(NULL, PATH_MAX);
		if (currpath != NULL)
			root = new DirNode(NULL,(string)currpath);
		else
		{
			cout << "error occurred while obtaining path\n";
			exit(1);
		}
	}
	
	cout << "starting in: " << root->getName() << endl;	
//	cout << "dirtree made\n";
}


DirTree::~DirTree()
// Directory tree deconstructor.
{
	clearTree();
	
	cout << "DIRTREE DELETED\n";
}
//==============================================================================


//==============================================================================
// Node-based methods
//==============================================================================
void DirTree::add(string p, string n)
// Method to insert a file into its appropriate place in the file list.
{	
	// Tokenize the given path, and store in a vector.
	vector<string>* path_toks = tokenizeV(p.substr(root->getName().size()),"/");
	string dir_path = root->getName();
	
	// Set the current node to the root of the directory tree, and start an
	// iterator on the current node's directory list. Also, save a reference to
	// the current directory's sub-directory list, for convenience.
	DirNode* curr = root;
	list<DirNode*>* dirs = curr->getDirectories();
	list<DirNode*>::iterator d = dirs->begin();
	
	// While there are still more paths to navigate...
	for (size_t i = 0; path_toks != NULL && i < path_toks->size();)
	{		
		// Scan the directory list for the current sub-directory.
		for (;d != dirs->end() && path_toks->at(i).compare((*d)->getName()) != 0 && !isLessThan(path_toks->at(i),(*d)->getName());d++);
		
		// If current sub-directory exists, go to it.
		if (d != dirs->end() && path_toks->at(i).compare((*d)->getName()) == 0)
		{
			curr = *d;
			dirs = curr->getDirectories();
			d = dirs->begin();
			i++;
		}
		// If current sub-directory does not exist, make it. Insertion is done
		// directly to the directory list, instead of using the DirNode's
		// addDirectory() method, so that the new sub-directory is in the right
		// order in the list.
		else
		{
			//cout << "creating a new directory named " << path_toks->at(i) << " in directory " << curr->getName() << endl;
			DirNode* tempd = new DirNode(curr,path_toks->at(i));
			//cout << "created new directory " << tempd->getName() << " in directory " << tempd->getParent()->getName() << endl;
			dirs->insert(d,tempd);
			d--;
		}
	}
	
	// Now that we're in the right directory, add the file to the directory.
	//cout << "Adding file " << n << " to directory " << curr->getName() << endl;
	curr->addFile(n);
	
//	cout << "file added to dirtree\n";
	numfiles++;
}


DirNode* DirTree::getDir(string p)
// Try to obtain a directory given its name.
{
	vector<string>* path_toks = tokenizeV(p.substr(root->getName().size()),"/");
	
	// Set the current node to the root of the directory tree, and start an
	// iterator on the current node's directory list.
	DirNode* curr = root;
	list<DirNode*>* dirs = curr->getDirectories();
	list<DirNode*>::iterator d = dirs->begin();
	
	// While there are still more paths to navigate...
	for (size_t i = 0; i < path_toks->size();)
	{
		// Scan the directory list for the current directory.
		for (;d != dirs->end() && path_toks->at(i).compare((*d)->getName()) != 0 && !isLessThan(path_toks->at(i),(*d)->getName());d++);
		
		// If current directory exists, go to it.
		if (d != dirs->end() && path_toks->at(i).compare((*d)->getName()) == 0)
		{
			curr = *d;
			dirs = curr->getDirectories();
			d = dirs->begin();
			i++;
		}
		// If current directory does not exist, return null
		else
			return NULL;
	}
	
	return *d;
}


FileNode* DirTree::getFile(string p, string n)
// Tries to find a file, given its path and name.
{
	// Get the directory that the file is supposed to be in.
	DirNode* dir = getDir(p);
	
	// If directory doesn't exist, return null.
	if (dir == NULL) return NULL;
	
	list<FileNode*>* files = dir->getFiles();
	
	// Now scan the current directory's file list
	list<FileNode*>::iterator f = files->begin();
	for (; f != files->end() && (*f)->getName().compare(n) == 0; f++);
	
	// If the file doesn't exist, return null.
	if (f == files->end())
		return NULL;
	
	return *f;
}
//==============================================================================


//==============================================================================
// Whole-tree methods.
//==============================================================================
void DirTree::clearTree()
// Empties the entire tree.
{
	dropBranch(root);
}


void DirTree::dropBranch(DirNode* d)
// Drop a branch from the tree.
{
	delete d;
}
//==============================================================================


//==============================================================================
// Convenience methods.
//==============================================================================
list<FileNode*>* DirTree::getAllFilesFrom(DirNode* d)
// Return a pointer to the list of all files in the given directory.
{
	return d->getAllFiles();
}

list<FileNode*>* DirTree::getFileList()
// Return a pointer to the list of all files in this tree.
{
	return getAllFilesFrom(root);
}

string DirTree::getRootPath()
// Returns the name (path) of the root node.
{
	return root->getName();
}

DirNode* DirTree::getRootNode()
// Return the root of the dirtree.
{
	return root;
}

int DirTree::getNumFiles()
// Get the number of files in the file tree.
{
	return numfiles;
}
//==============================================================================
