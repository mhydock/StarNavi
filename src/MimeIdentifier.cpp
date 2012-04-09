//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		29 March 2012
//
// File name:			MimeIdentifier.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class that identifies the mime-type of a file. Used to
//						be a custom build, now a wrapper for libmagic.
//==============================================================================

#include <fstream>
#include "MimeIdentifier.h"

//==============================================================================
// Private methods.
//==============================================================================
void MimeIdentifier::buildDefaultAppsList()
{
	ifstream default_file("/usr/share/applications/defaults.list");
	string line;
	vector<string>* toks = NULL;
	
	// Begin reading lines and looking for the appropriate type.
	getline(default_file,line);
	getline(default_file,line);
	while (!default_file.eof())
	{
		toks = tokenizeV(line,"=");

		if (toks != NULL) default_apps.push_back(*toks);
		
		getline(default_file,line);
	}
	
	default_file.close();
}
//==============================================================================


//==============================================================================
// Methods that determine MIME attributes given strings.
//==============================================================================
string MimeIdentifier::setFileType(string pathname)
// Seeks through the mime filetype database on the user's computer, and attempts
// to determine the requested file's filetype.
{
//	cout << pathname << endl;
	
	magic_t magic_cookie;
	
	/*MAGIC_MIME tells magic to return a mime of the file, but you can specify different things*/
	magic_cookie = magic_open(MAGIC_MIME);

	if (magic_cookie == NULL)
	{
		printf("unable to initialize magic library\n");
		exit(1);
	}

	if (magic_load(magic_cookie, NULL) != 0)
	{
		printf("cannot load magic database - %s\n", magic_error(magic_cookie));
		magic_close(magic_cookie);
		exit(1);
	}

	string temp = magic_file(magic_cookie, pathname.c_str());
		
	magic_close(magic_cookie);
	
	return temp.substr(0,temp.find_first_of(';'));
}

string MimeIdentifier::setDefaultApp(string mime_type)
// Given a string of a mime type, return the name of the application used to
// open this file.
{
	for (list< vector<string> >::iterator i = default_apps.begin(); i != default_apps.end(); i++)
		if (mime_type.compare((*i)[0]) == 0)
			return (*i)[1].substr(0,(*i)[1].find_last_of('.'));
		
	return "gedit";
}

enum filetype MimeIdentifier::enumFileType(string mime_type)
// Take a string representation of a file type, and turn it into an enumeration.
{
	string type = mime_type.substr(0,mime_type.find_first_of('/'));

	if (type.compare("") == 0)	
		return UNKNOWN;
	else if (type.compare("binary") == 0)
		return BIN;
	else if (type.compare("application") == 0)
		return APP;
	else if (type.compare("audio") == 0)
		return AUDIO;
	else if (type.compare("image") == 0)
		return IMAGE;
	else if (type.compare("text") == 0)
		return TEXT;
	else if (type.compare("video") == 0)
		return VIDEO;
	else	
		return UNKNOWN;
}
//==============================================================================


//==============================================================================
// Public methods.
//==============================================================================
MimeIdentifier::MimeIdentifier()
// Build the defaults list.
{
	buildDefaultAppsList();
}
//==============================================================================
