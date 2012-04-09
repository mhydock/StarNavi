//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		17 May 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a list of files as Stars in a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "Galaxy.h"

DrawText Galaxy::starSelectionLabel(" Star Selection Mode");
bool Galaxy::isSSLabelInitialized = false;

//==============================================================================
// Constructors/Deconstructors
//==============================================================================
Galaxy::Galaxy(DirNode *r, list<FileNode*>* f, cluster_type m, string n, list<string>* t)
{
	cout << "making a galaxy...\n";
	
	if (r != NULL)
		setDirectory(r);
	else
	{
		root = NULL;
		files = f;
	}
	
	if (root != NULL && n == "")
		name = root->getPath();
	else
		name = n;
	
	xPos = 0;
	yPos = 0;
	side = 0;
	
	// Inherited from Drawable. Will be repurposed to keep track of the size of
	// the viewport.
	width = 0;
	height = 0;
	
	diameter = 64.0 * pow((3.0*(double)files->size())/(4.0*M_PI),(1.0/3.0));
	radius = diameter/2;
	thickness = pow(radius*2.0,.5);
	
	setRotation(0,0);
	setRotationSpeed(0.02);
	rotZ = 0;
	
	tags = t;
	if (tags == NULL)
		rebuildTags();
	
	cluster_mode = m;
	
	sectors = NULL;
	selected = NULL;
	buildSectors();
	
	texture = NULL;
	refreshTex();
	
	label = NULL;
	initLabel();
	
	adjustStarSelectionLabel();
	
	cout << "galaxy built\n";
}

Galaxy::~Galaxy()
{
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
		delete *i;

//	cout << "deleted individual sectors\n";

	delete sectors;

//	cout << "deleted sectors list\n";
	
//	if (root != NULL)
//		delete files;
	
//	cout << "deleted files\n";
	
	delete texture;
	
//	cout << "deleted texture\n";
}

void Galaxy::adjustStarSelectionLabel()
{
	if (!isSSLabelInitialized)
	{
		cout << starSelectionLabel.getText() << endl;
		isSSLabelInitialized = true;
		starSelectionLabel.setAlignment(LEFT);
		starSelectionLabel.setTextColor(0,0,0,1);
		starSelectionLabel.refreshTexture();
	}
}
//==============================================================================


//==============================================================================
// Methods related to the angle and velocity of the galaxy.
//==============================================================================
void Galaxy::setRotation(float x, float y)
{
	rotX = x;
	rotY = y;
}

void Galaxy::setRotationSpeed(float s)
{
	rotSpeed = s;
}

float Galaxy::getRotationX()
{
	return rotX;
}

float Galaxy::getRotationY()
{
	return rotY;
}

float Galaxy::getRotationSpeed()
{
	return rotSpeed;
}
//==============================================================================


//==============================================================================
// Miscellanious getters and setters.
//==============================================================================
void Galaxy::setName(string n)
// Set the galaxy's name (to be used if root == NULL)
{
	name = n;
}

string Galaxy::getName()
// Obtain the galaxy's name.
{
	return name;
}

void Galaxy::setTags(list<string>* t)
// In the event an outside object already made a set of tags, might as well just
// use those.
{
	tags = t;
}

void Galaxy::rebuildTags()
// Build the galaxy's tag list.
{
	cout << "building tags" << endl;
	
	// Clean out the original tags list, if it wasn't empty to begin with.
	if (tags != NULL)	delete(tags);
	tags = new list<string>;
	
	// Temporary list for the current file's tags.
	list<string>* temp = NULL;
	
	// Go through all of the file nodes, and try to add that file's tags to the
	// tag list. If the list already contains the current tag, skip it.
	for (list<FileNode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		
		temp = (*i)->getTags();

		if (temp != NULL)
			for (list<string>::iterator j = temp->begin(); j != temp->end(); j++)
				if (!contains(tags,*j))
				{
					cout << *j << endl;
					tags->push_back(*j);
				}
	}
	
	cout << "size of tags list: " << tags->size() << endl;	
}

list<string>* Galaxy::getTags()
// Return the galaxy's list of tags.
{
	return tags;
}

void Galaxy::setClusterMode(cluster_type m)
// Set the galaxy's clustering mode.
{
	cluster_mode = m;
}

cluster_type Galaxy::getClusterMode()
// Return the galaxy's clustering mode.
{
	return cluster_mode;
}

void Galaxy::setDirectory(DirNode* r)
// Set the galaxy's file indexer.
{
	root = r;
	files = r->getAllFiles();
}

DirNode* Galaxy::getDirectory()
// Obtain a pointer to the root of the indexer used by the galaxy.
{
	return root;
}

void Galaxy::setFileList(list<FileNode*>* f)
// Set the galaxy's file list (to be used if root == NULL)
{
	files = f;
}

list<FileNode*>* Galaxy::getFileList()
// Obtain the galaxy's file list (to be used if root == NULL)
{
	return files;
}
//==============================================================================


//==============================================================================
// Sector building.
//==============================================================================
void Galaxy::buildSectors()
// Build the galaxy's sectors based on the current build mode.
{
	cout << "building sectors\n";
	clearSectors();
	
	sectors = new list<GSector*>();
	
	switch (cluster_mode)
	{
		case NONE:
		case DIRECTORY:	buildHierarchy();
						break;
		case NAME:		buildByName();
						break;
		case DATE:		buildByDate();
						break;
		case SIZE:		buildBySize();
						break;
		case TYPE:		buildByType();
						break;
		case TAGS:		buildByTags();
						break;
		default:		break;
	}
		
	cout << "sectors built\n";
	
	if (sectors->size() == 1)
		(*(sectors->begin()))->setSingleSectorMode(true);
	else
		adjustSectorWidths();
}

void Galaxy::buildHierarchy()
// Build a galaxy using the file hierarchy for the structure.
{
	cout << "hierarchy build mode\n";
	
	list<DirNode*>* dirs = root->getDirectories();
	
	if (root == NULL)
	{
		name += " [files]";
		sectors->push_back(new GSector(NULL,files,radius,0,360,name));
		return;
	}
	
	float arc_begin = 0;
	float arc_width = 360.0*((float)root->getFiles()->size()/(float)files->size());
	
	// Make the sector that holds the current directories loose files.
	sectors->push_back(new GSector(NULL,root->getFiles(),radius,arc_begin,arc_width,"./"));
	cout << "root sector built\n";
	
	// Make sectors for the other subdirectories. 
	cout << "creating sectors for directories\n";
	
	for (list<DirNode*>::iterator i = dirs->begin(); i != dirs->end(); i++)
	{
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->getAllFiles()->size()/(float)files->size());
		sectors->push_back(new GSector(*i,NULL,radius,arc_begin,arc_width));
	}
}

void Galaxy::buildByName()
// Build a galaxy by organizing files by their names.
{
	list<FileNode*> sorted_files;
	list<list<FileNode*>*> file_lists;
	size_t charindex = 0;
	
	// Sort the galaxy's file list by name of the files.
	list<FileNode*>::iterator i = files->begin();
	list<FileNode*>::iterator j;
	sorted_files.push_back(*i);
	i++;
	while (sorted_files.size() < files->size())
	{
		j = sorted_files.begin();
		for(;j != sorted_files.end() && isLessThan((*j)->getName(),(*i)->getName());j++);
		if (j == sorted_files.end())	sorted_files.push_back(*i);
		else							sorted_files.insert(j,(*i));
		i++;
	}
	// Done sorting the sectors.
	
	// Preliminary test. Go through the sorted list, and see at what index do
	// the file names start being different.
	i = sorted_files.end();
	string firstName = (*(sorted_files.begin()))->getName();
	string currName = "";
	
	for (charindex = 0; i == sorted_files.end(); charindex++)
	{
		for (i = sorted_files.begin(); i != sorted_files.end(); i++)
		{
			currName = (*i)->getName();
			if (charindex < currName.size() && charindex < firstName.size())
				if (currName[charindex] != firstName[charindex])
					break;
		}
	}
	// End preliminary test.
	
	// Begin splitting sorted list.
	i = sorted_files.begin();
	list<FileNode*>* temp = new list<FileNode*>;
	temp->push_back(*i);
	
	j = i;
	j++;
	
	string iName;
	string jName;
	while (j != sorted_files.end())
	{
		iName = (*i)->getName();
		jName = (*j)->getName();
		
		if (charindex >= jName.size() || charindex >= iName.size() || jName[charindex] != iName[charindex])
		{
			file_lists.push_back(temp);
			temp = new list<FileNode*>;
		}
		
		temp->push_back(*j);
		i = j;
		j++;
	}
	
	file_lists.push_back(temp);
	// End splitting sorted list.
	
	// Start making the sectors.
	float arc_begin = 0;
	float arc_width = 0;
	 
	cout << "creating sectors divided by name\n";
	for (list<list<FileNode*>*>::iterator i = file_lists.begin(); i != file_lists.end(); i++)
	{
		string name = (*((*i)->begin()))->getName();
		name = name.substr(0,charindex);
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->size()/(float)sorted_files.size());
		sectors->push_back(new GSector(NULL,(*i),radius,arc_begin,arc_width,name));
	}
	// End making sectors.
}

void Galaxy::buildByDate()
{}

void Galaxy::buildBySize()
{}

void Galaxy::buildByType()
{}

void Galaxy::buildByTags()
// Build sectors by separating files according to what tags they have.
{
	list<list<FileNode*>*>* temp_list = new list<list<FileNode*>*>;
	int total_size = 0;
	
	// Make a file list for each tag, containing files that have those tags.
	for (list<string>::iterator i = tags->begin(); i != tags->end(); i++)
	{
		cout << "collecting files with tag " << *i << endl;
		// Make a temporary list for FileNodes.
		list<FileNode*>* temp_files = new list<FileNode*>;
		
		// Fill it with files that have the current tag.
		for (list<FileNode*>::iterator j = files->begin(); j != files->end(); j++)
			if (contains((*j)->getTags(),*i))
			{
				cout << "found a file!\n";
				temp_files->push_back(*j);
			}
				
		// Keep track of the total size, then add it to the list of lists.
		total_size += temp_files->size();
		temp_list->push_back(temp_files);
		
		cout << total_size << " " << temp_files->size() << endl;
	}
	
	float arc_begin = 0;
	float arc_width = 0;
	
	// Make sectors for the tags.
	cout << "creating sectors for tags\n";
	list<string>::iterator n = tags->begin();
	for (list<list<FileNode*>*>::iterator i = temp_list->begin(); i != temp_list->end(); i++)
	{
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->size()/total_size);
		sectors->push_back(new GSector(NULL,*i,radius,arc_begin,arc_width,*n));
		n++;
	}

	if (sectors->size() <= 1)
		rebuildTags();
}
//==============================================================================


//==============================================================================
// Sector management.
//==============================================================================
void Galaxy::adjustSectorWidths()
// Dynamically resize the sectors' widths, in case some are too small. This
// method will probably derp hard if there are a ton of sectors that are too
// small. Until I fix that, just try not to have ~90 directories with only 1
// file in each of them.
{	
	list<GSector*> *temp = new list<GSector*>();
	list<GSector*>::iterator i = sectors->begin();
	list<GSector*>::iterator j;
	
	// Sort the sectors into another list.
	temp->push_back(*i);
	i++;
	while (temp->size() < sectors->size())
	{
		j = temp->begin();
		while(j != temp->end() && (*i)->getArcWidth() > (*j)->getArcWidth())
			j++;
		if (j == temp->end())	temp->push_back(*i);
		else					temp->insert(j,(*i));
		i++;
	}
	// Done sorting the sectors.
		
	// If the current sector is too small, grow it and shrink all larger ones.
	for (i = temp->begin(); i != temp->end() && (*i)->getArcWidth() < ((*i)->calcMinArcWidth()+4); i++)
	{
		float diff = ((*i)->calcMinArcWidth()+4)-(*i)->getArcWidth();
		(*i)->setArcWidth((*i)->getArcWidth()+diff);
		
		int remain = 0;
		for (j = i; j != temp->end(); j++)
			remain++;
		remain--;
		
		float distr = diff/(float)remain;
		
		j = i;
		j++;
		while (j != temp->end())
		{
			(*j)->setArcWidth((*j)->getArcWidth()-distr);
			j++;
		}
	}
	// Done shrinking the sectors.
	
	// Shift all of the sectors into proper positions
	i = sectors->begin();
	j = sectors->begin();
	
	j++;
	for(;j != sectors->end(); j++)
	{
		(*j)->setArcBegin((*i)->getArcEnd());
		i = j;
	}
	// Done shifting sectors.
	
	// Make sure all the stars are within their sector's bounds.
	for (i = sectors->begin(); i != sectors->end(); i++)
	{
		float beg = (*i)->getArcBegin();
		float wid = (*i)->getArcWidth();
		float thk = (*i)->getThickness();
		float rad = (*i)->getRadius();
		
		for (list<Star*>::iterator k = (*i)->getStars()->begin(); k != (*i)->getStars()->end(); k++)
			if ((*k)->getAngle() > beg+wid || (*k)->getAngle() < beg)
				(*k)->randomPosition(beg,beg+wid,0,rad,-thk,thk);
	}
	// Done repositioning stars.
}
		

void Galaxy::clearSectors()
{
	if (sectors != NULL)
	{
		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			delete *i;
		delete sectors;
	}
}


list<GSector*>* Galaxy::getSectors()
// Return a list of the galaxy's sectors.
{
	return sectors;
}
//==============================================================================


//==============================================================================
// Methods for user interaction.
//==============================================================================
bool Galaxy::isColliding(float x, float y)
{
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);

	selected = NULL;
	
	// Turn the given coordinates to local coordinates.
	float localX = x-xPos-width/2;
	float localY = y-yPos+height/2;
	
	// Turn the local cartesian coordinates into local polar coordinates.
	float angle_r = atan2(localY,localX);
	float angle_d = angle_r*(180.0/M_PI);
	float magnitude = sqrt(pow(localX,2.0)+pow(localY,2.0));
	float norm_mag = magnitude/(side/2);

	// Shift the angle to match the galaxy's rotation
	angle_d -= rotZ;	
	angle_d = (angle_d < 0)?angle_d+360:angle_d;

//	cout << xPos << ", " << yPos << endl;
//	cout << x << ", " << y << " | " << localX << ", " << localY << endl;
//	cout << angle_d << ", " << norm_mag << endl;
	
	if (norm_mag > 1.0)
		return collide_flag = false;
	
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end() && selected == NULL; i++)
		if ((*i)->isColliding(angle_d, norm_mag))
			selected = (*i);
		
//	if (selected != NULL) cout << "colliding with sector " << selected->getName() << endl;
	
	return collide_flag = true;
}

GSector* Galaxy::getSelected()
{
	return selected;	
}
//==============================================================================


//==============================================================================
// Methods related to drawing.
//==============================================================================
void Galaxy::refreshTex()
// Update the galaxy texture.
{
	int tex_size;
	tex_size = (int)(1024.0 * (diameter/500.0));
	tex_size = (tex_size < 512)?512:tex_size;

	cout << diameter << "  " << tex_size << endl;
	
	if (texture != NULL)
		delete texture;
	
	texture = new RenderTextureObject(tex_size,tex_size);
	texture->initTexture();
	texture->startRendering();
	
	// Push the viewport to an attribute stack, and render as usual.
	glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,tex_size,tex_size);
		
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-radius,radius,-radius,radius,-thickness,thickness);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//		if (files->size() > 200)
//			Star::setPointDrawMode();
//		else
//			Star::setTexturedDrawMode();

		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			(*i)->draw();
			
		glFlush();
	glPopAttrib();
	
	texture->stopRendering();
}

void Galaxy::drawNormalMode()
{
	// Draw the sector division lines.
	glPushMatrix();
		glTranslatef(0,0,1);
		glRotatef(rotZ,0,0,1);
		glScalef((side-5)/2,(side-5)/2,1);
			
		glBegin(GL_LINES);
			for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			{
				float arc_begin = (*i)->getArcBegin();
				float arc_begin_r = arc_begin * M_PI/180;
	
				glColor4d(1,1,1,1);
				glVertex2d(0.0,0.0);
				glColor4d(0,0,0,0);
				glVertex2d(cos(arc_begin_r),sin(arc_begin_r));
			}
		glEnd();		
	glPopMatrix();
	// Done drawing sector division lines.
			
	// Draw the selection mask.
	if (collide_flag && selected != NULL)
	{			
		glPushMatrix();
			glTranslatef(0,0,2);
			glRotatef(rotZ,0,0,1);
			glScalef((side)/2,(side)/2,1);
			selected->drawMask();
		glPopMatrix();
		
		// Try to initialize the text label. If it already exists, this does
		// nothing.
		selected->initLabel();
		
		float angle = rotZ + selected->getArcBegin() + (selected->getArcWidth()/2);			
		float x = ((side-5)/4) * cos(angle*M_PI/180);
		float y = ((side-5)/4) * sin(angle*M_PI/180);
	
//		cout << angle << " " << x << " " << y << endl;
			
		glPushMatrix();
			glTranslatef(x,y,3);
			selected->drawLabel();
		glPopMatrix();
	}
	// Done drawing selection mask.
}

void Galaxy::drawStarSelectionMode()
// Draw the various features present under star selection mode.
{
	// Draw the Star Selection Mode banner.
	glPushMatrix();
		glTranslatef(width/-2,height/2,1);
		glScalef(starSelectionLabel.getTextWidth(),starSelectionLabel.getTextHeight(),1);
		glBegin(GL_QUADS);
			glColor4d(1,1,1,1);
			glVertex2d(0,0);
			glVertex2d(0,-1);
			glColor4d(1,1,1,0);
			glVertex2d(2,-1);
			glVertex2d(2,0);
		glEnd();
	glPopMatrix();
				
	glPushMatrix();
		glTranslatef(0,0,2);
		starSelectionLabel.setPosition(width/-2,height/2);
		starSelectionLabel.draw();
	glPopMatrix();
	// Done drawing the Star Selection Mode banner.

	// If the user is mousing over the galaxy...
	if (collide_flag)
		// If the user is mousing over a sector...
		if (selected != NULL)
		{
			Star* star = selected->getSelected();
			
			// If the user is mousing over a star, draw the star's label.
			if (star != NULL)
			{
				//cout << "User is mousing over star " << star->getName() << endl;
				
				// Try to initialize the text label. If it already exists,
				// this does nothing.
				star->initLabel();
				
				// Galaxy is being scaled to window. The star's label's
				// coordinates also need to be scaled, if they are to hover
				// over their star.
				float d = star->getDistance();
				float a = star->getAngle() + rotZ;
				
				float x = d*cos(a*M_PI/180);
				float y = d*sin(a*M_PI/180);
				float w = star->getLabel()->getWidth();
				float h = star->getLabel()->getHeight();
				
				x *= ((side-5)/2)/radius;
				y *= ((side-5)/2)/radius;

				// Get the label away from the edges of the viewport
				if (x-w/2 < -width/2.0) x = -width/2.0 + w/2;
				if (x+w/2 > width/2.0) x = width/2.0 - w/2;
				if (y-h/2 < -height/2.0) y = -height/2.0 + h/2;
				if (y+h/2 > height/2.0) y = height/2.0 - h/2;					
						
				// Set the label's new position.
				star->getLabel()->setPosition(x,y);
				
				// Shift the label up a little, and draw.
				glPushMatrix();
					glTranslatef(0,0,2);
					star->drawLabel();
				glPopMatrix();
			}
		}
}

void Galaxy::draw()
// Draw the galaxy.
{
	// Set the size of the galaxy, based on the viewport.
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);

	width = p[2];
	height = p[3];
	
	side = (width < height)?width:height;

	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
		// Move everything to the center of the viewport.
		glTranslatef(p[2]/2,-p[3]/2,0);
		
		// Draw the texture-mapped galaxy.
		glPushMatrix();
			// Rotate the galaxy
			glRotatef(rotZ,0,0,1);
		
			// Scale the galaxy to fit in the viewport.
			glScalef((side-5)/2,(side-5)/2,1);
		
			// Bind the previously rendered texture, and map it to a quad.
			texture->loadTexture();
			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
				glTexCoord2f(0,1);	glVertex2d(-1,1);
				glTexCoord2f(0,0);	glVertex2d(-1,-1);
				glTexCoord2f(1,0);	glVertex2d(1,-1);
				glTexCoord2f(1,1);	glVertex2d(1,1);
			glEnd();
			texture->unloadTexture();
		glPopMatrix();
		// Done drawing the texture-mapped galaxy.
	
		// If the user is not in star selection mode, draw the sector lines and the
		// sector selection mask.
		if (!Star::starSelectionMode && sectors->size() > 1)
			drawNormalMode();
		
		// User is in star selection mode.
		if (Star::starSelectionMode || sectors->size() <= 1)
			drawStarSelectionMode();

	glPopMatrix();
			
	// Turn off blending.
	glDisable(GL_BLEND);
	
	rotZ += rotSpeed;
		if (rotZ > 360) rotZ -= 360;
}
//==============================================================================
