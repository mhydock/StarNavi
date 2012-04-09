//==============================================================================
// Date Created:		26 March 2012
// Last Updated:		29 March 2012
//
// File name:			FileNode.h
// Programmer:			Matthew Hydock
//
// File description:	Methods for a class that represents a file in a file
//						systems. Includes getters and setters.
//==============================================================================

#include "FileNode.h"
#include <iostream>

MimeIdentifier FileNode::mrmime = MimeIdentifier();

//==============================================================================
// Constructor and deconstructor.
//==============================================================================
FileNode::FileNode(DirNodePrototype* p, string n)
// Create a filenode and set its attributes.	
{
	//cout << "Trying to load file " << n << " located in " << p->getName() << endl;
	name = n;
	parent = p;
	
	string temp_string = getPath() + n;
	stat(temp_string.c_str(), &attr);
	
	obtainType();

	setTags();
	
	//cout << "File " << name << " loaded.\n";
}

FileNode::~FileNode()
// Trivial deconstructor.
{
}
//==============================================================================


//==============================================================================
// Getters and setters for the path and name of the file.
//==============================================================================		
string FileNode::getPath()
{
	return parent->getPath();
}

string FileNode::getName()
{
	return name;
}

void FileNode::setName(string n)
{
	name = n;
}

DirNodePrototype* FileNode::getParent()
{
	return parent;
}

void FileNode::setParent(DirNodePrototype* p)
{
	parent = p;
}
//==============================================================================


//==============================================================================
// Getters for MIME data and real file attributes. This data cannot be changed,
// only retrieved.
//==============================================================================
string FileNode::getMimetype()
{
	return mime_type;
}

string FileNode::getDefaultApp()
{
	return default_app;
}

enum filetype FileNode::getMimeEnum()
{
	return mime_enum;
}

struct stat FileNode::getAttributes()
{
	return attr;
}
//==============================================================================


//==============================================================================
// Utility function to find and store tags related to this file.
//==============================================================================
void FileNode::setTags()
// Set the filenode's tags depending on the existance of a tag file.
{
	string tag_file = getPath() + "." + getName() + ".tags";
	struct stat tempstat;
	
	// If there is a tags file with this file's name, import the tags	
	if (stat(tag_file.c_str(),&tempstat) == 0)
	{
//		cout << "tag file found (?)\n";
		string line = "";
		list<string>* temp_tags = NULL;
	
//		cout << "trying to open " << tag_file << endl;

		// Read in the tag file one line at a time until EOF.
		ifstream tag_stream(const_cast<char*>(tag_file.c_str()));
		while (!tag_stream.eof())
		{
			getline(tag_stream,line);
			temp_tags = tokenizeL(line," \n");
			if (temp_tags != NULL)	append(&tags,temp_tags);
		}
		// Done reading in tags.
	}
}

void FileNode::rebuildTags()
{
	tags.clear();
	
	setTags();
}

list<string>* FileNode::getTags()
{
	return &tags;
}

void FileNode::obtainType()
// Scan through the mime table, and return the type of the requested file.
{
//	cout << "Obtaining MIME data...\n";
	mime_type = mrmime.setFileType(getPath()+getName());
//	cout << "MIME type determined.\n";
	mime_enum = mrmime.enumFileType(mime_type);
//	cout << "MIME enum set.\n";
	default_app = mrmime.setDefaultApp(mime_type);
//	cout << "Default application set.\n";
}
//==============================================================================
