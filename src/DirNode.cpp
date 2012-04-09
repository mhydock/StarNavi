//==============================================================================
// Date Created:		28 March 2012
// Last Updated:		29 March 2012
//
// File name:			DirNode.cpp
// Programmer:			Matthew Hydock
//
// File description:	Implementation of a class that contains data for a
//						directory. The directory has full knowledge of its
//						contents (files and sub-directories) and its parent.
//==============================================================================

#include "DirNode.h"

DirNode::DirNode(DirNode* p, string n)
// Creates a new DirNode, giving it a name and setting its parent.
{
	name = n;
	parent = p;
}

DirNode::~DirNode()
// Delete this DirNode, and all of the directories and files held within.
// Remember to actually access every item in the lists to delete them before
// clearing the list, as clearing the list only deletes the pointers, and does
// not free the memory that was allocated for them.
{
	list<FileNode*>::iterator fli = files.begin();

	for (; fli != files.end(); fli++)
		delete *fli;
		
	files.clear();	
	
	list<DirNode*>::iterator dli = dirs.begin();
	for (; dli != dirs.end(); dli++)
		delete *dli;
		
	dirs.clear();
}

//==============================================================================
// Methods to locate files or directories in this directory.
//==============================================================================
list<FileNode*>::iterator DirNode::findFile(string fn)
// Tries to find a file, given the name of the file.
{
	list<FileNode*>::iterator fli = files.begin();
	for (; fli != files.end() && ((*fli)->getName().compare(fn) != 0); fli++);
	
	return fli;
}

list<FileNode*>::iterator DirNode::findFile(FileNode* f)
// Tries to find a reference to the given file in this directory.
{	
	list<FileNode*>::iterator fli = files.begin();
	for (; fli != files.end() && (*fli != f); fli++);
	
	return fli;
}

list<DirNode*>::iterator DirNode::findDirectory(string dn)
// Tries to find a sub-directory, given the name of the sub-directory.
{
	list<DirNode*>::iterator dli = dirs.begin();
	for (; dli != dirs.end() && ((*dli)->getName().compare(dn) != 0); dli++);
	
	return dli;
}

list<DirNode*>::iterator DirNode::findDirectory(DirNode* d)
// Tries to find a reference to the given sub-directory in this directory.
{
	list<DirNode*>::iterator dli = dirs.begin();
	for (; dli != dirs.end() && (*dli != d); dli++);
	
	return dli;
}
//==============================================================================

//==============================================================================
// Methods pertaining to files.
//==============================================================================
void DirNode::addFile(string fn)
// Adds a file to this directory. If a file with the same name is already here,
// print a warning, and don't add anything.
{
	list<FileNode*>::iterator fli = findFile(fn);
		
	if (fli == files.end())
		files.push_back(new FileNode(this,fn));
	else
		cout << "WARNING: A file with the name " << fn << " is already in this directory." << endl;
}
	
void DirNode::addFile(FileNode* f)
// Adds a file that already exists to this directory. If the file is already in
// this directory, a warning is printed and nothing is added.
{
	list<FileNode*>::iterator fli = findFile(f);
		
	if (fli == files.end())
		files.push_back(f);
	else
		cout << "WARNING: The file " << f->getName() << " is already in this directory." << endl;
}

FileNode* DirNode::removeFile(string fn)
// Removes a file with the given name from this directory. If the file did exist
// in this directory, a reference is returned, but the file is not actually
// deleted. Deletion is to occur outside this method. Useful when moving files.
// NULL is returned if the file didn't exist.
{
	list<FileNode*>::iterator fli = findFile(fn);
		
	if (fli != files.end())
	{
		files.remove(*fli);
		return *fli;
	}
	else
	{
		cout << "WARNING: The file " << fn << " does not exist in this directory." << endl;
		return NULL;
	}
}

FileNode* DirNode::removeFile(FileNode* f)
// Removes a file given a reference to that file from this directory. If the
// file did exist in this directory, a reference is returned, but the file is
// not actually deleted. Deletion is to occur outside this method. Useful when
// moving files. NULL is returned if the file didn't exist.
{
	list<FileNode*>::iterator fli = findFile(f);
		
	if (fli != files.end())
	{
		files.remove(*fli);
		return *fli;
	}
	else
	{
		cout << "WARNING: The file " << f->getName() << " is already in this directory." << endl;
		return NULL;
	}
}

void DirNode::deleteFile(string fn)
// Given the name of a file, deletes the file from this directory, provided the
// file actually exists in this directory.
{
	FileNode* temp = removeFile(fn);
	
	if (temp != NULL)
		delete temp;
}

void DirNode::deleteFile(FileNode* f)
// Given a reference to a file, deletes the file from this directory, provided
// the file actually exists in this directory.
{
	FileNode* temp = removeFile(f);
	
	if (temp != NULL)
		delete temp;
}
//==============================================================================


//==============================================================================
// Methods pertaining to directories.
//==============================================================================
void DirNode::addDirectory(string dn)
// Adds a sub-directory to this directory. If a sub-directory with the same name
// is already here, print a warning, and don't add anything.
{
	list<DirNode*>::iterator dli = findDirectory(dn);
		
	if (dli == dirs.end())
		dirs.push_back(new DirNode(this,dn));
	else
		cout << "WARNING: A directory with the name " << dn << " is already in this directory." << endl;
}
	
void DirNode::addDirectory(DirNode* d)
// Adds a sub-directory that already exists to this directory. If the sub-
// directory is already in this directory, print a warning, and don't add
// anything.
{
	list<DirNode*>::iterator dli = findDirectory(d);
		
	if (dli == dirs.end())
		dirs.push_back(d);
	else
		cout << "WARNING: The sub-directory " << d->getName() << " is already in this directory." << endl;
}

DirNode* DirNode::removeDirectory(string dn)
// Removes a sub-directory with the given name from this directory. If the sub-
// directory did exist in this directory, a reference is returned, but the sub-
// directory is not actually deleted. Deletion is to occur outside this method. 
// Useful when moving sub-directories. NULL is returned if the sub-directory
// didn't exist.
{
	list<DirNode*>::iterator dli = findDirectory(dn);
		
	if (dli != dirs.end())
	{
		dirs.remove(*dli);
		return *dli;
	}
	else
	{
		cout << "WARNING: The sub-directory " << dn << " does not exist in this directory." << endl;
		return NULL;
	}
}

DirNode* DirNode::removeDirectory(DirNode* d)
// Removes a sub-directory given a reference to that sub-directory from this
// directory. If the sub-directory did exist in this directory, a reference is
// returned, but the sub-directory is not actually deleted. Deletion is to occur
// outside this method. Useful when moving sub-directories. NULL is returned if
// the sub-directory didn't exist.
{
	list<DirNode*>::iterator dli = findDirectory(d);
		
	if (dli != dirs.end())
	{
		dirs.remove(*dli);
		return *dli;
	}
	else
	{
		cout << "WARNING: The file " << d->getName() << " is already in this directory." << endl;
		return NULL;
	}
}

void DirNode::deleteDirectory(string dn)
// Given the name of a sub-directory, deletes the sub-directory from this
// directory, provided the sub-directory actually exists in this directory.
{
	DirNode* temp = removeDirectory(dn);
	
	if (temp != NULL)
		delete temp;
}

void DirNode::deleteDirectory(DirNode* d)
// Given a reference to a sub-directory, deletes the sub-directory from this
// directory, provided the sub-directory actually exists in this directory.
{
	DirNode* temp = removeDirectory(d);
	
	if (temp != NULL)
		delete temp;
}
//==============================================================================


//==============================================================================
// Simple getters and setters.
//==============================================================================
list<FileNode*>* DirNode::getFiles()
// Returns a reference to this directory's file list.
{
	return &files;
}

list<DirNode*>* DirNode::getDirectories()
// Returns a reference to this directory's list of sub-directories.
{
	return &dirs;
}
		
string DirNode::getName()
// Obtains the name of this directory.
{
	return name;
}

void DirNode::rename(string n)
// Changes the name of this directory.
{
	name = n;
}

DirNode* DirNode::getParent()
// Obtains the parent directory of this directory (if it has one).
{
	return parent;
}

void DirNode::setParent(DirNode* p)
// Sets the parent directory of this directory.
{
	parent = p;
}
//==============================================================================


//==============================================================================
// Getters for recursively generated data.
//==============================================================================
string DirNode::getPath()
// Recursively builds the path by examining the names of this directory's
// ancestors.
{
	if (parent == NULL)
		return name;
		
	return parent->getPath() + name + "/";
}

		
list<FileNode*>* DirNode::getAllFiles()
// Generate and return a list of all files in this directory and all descendent
// directories.
{
	if (dirs.empty())
		return getFiles();
	
	list<FileNode*>* f = new list<FileNode*>;
	
	append(f,getFiles());
	
	list<DirNode*>::iterator dli = dirs.begin();
	for (; dli != dirs.end(); dli++)
		append(f,(*dli)->getAllFiles());
	
	return f;
}
//==============================================================================
