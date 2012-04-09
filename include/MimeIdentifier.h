//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		29 March 2012
//
// File name:			MimeIdentifier.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that identifies the mime-type of a
//						file, using libmagic.
//==============================================================================

#include "global_header.h"

#include <magic.h>

#ifndef MIMEIDENTIFIER
#define MIMEIDENTIFIER

// To represent the different types of files.
enum filetype {BIN, APP, AUDIO, IMAGE, TEXT, VIDEO, UNKNOWN};

class MimeIdentifier
{
	private:
		list< vector<string> > default_apps;
		
		void buildDefaultAppsList();
		
	public:
		MimeIdentifier();
		
		string setDefaultApp(string pathname);
		string setFileType(string mime_type);
		enum filetype enumFileType(string mime_type);
};

#endif
