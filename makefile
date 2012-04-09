vpath %.cpp ./src/
vpath %.o ./bin/
vpath %.h ./include/

SOURCES =	MimeIdentifier.cpp \
			FileNode.cpp \
			DirNode.cpp \
			DirTree.cpp \
			Indexer.cpp \
			TextureObject.cpp \
			RenderTextureObject.cpp \
			Drawable.cpp \
			DrawableList.cpp \
			DrawText.cpp \
			LabeledDrawable.cpp \
			Container.cpp \
			Button.cpp \
			ListItem.cpp \
			TagsList.cpp \
			Star.cpp \
			GSector.cpp \
			Galaxy.cpp \
			StateManager.cpp \
			StatusBar.cpp \
			Main.cpp
			
OBJECTS = 	MimeIdentifier.o \
			FileNode.o \
			DirNode.o \
			DirTree.o \
			Indexer.o \
			TextureObject.o \
			RenderTextureObject.o \
			Drawable.o \
			DrawableList.o \
			DrawText.o \
			LabeledDrawable.o \
			Container.o \
			Button.o \
			ListItem.o \
			TagsList.o \
			Star.o \
			GSector.o \
			Galaxy.o \
			StateManager.o \
			StatusBar.o \
			Main.o
			
HEADERS =	$(wildcard *.h)

CC = g++
CFLAGS = -g \
	-Wall \
	-Wextra \
	-pedantic \
# endlist

CPPFLAGS := $(CFLAGS) -I include

LDFLAGS :=  -lm -lmagic -lGL -lGLU -lglut -lSDL -lSDL_ttf -lSDL_image -lX11

.PHONY: default
default: normbuild

.PHONY: clean
clean:
	cd bin
	rm -f $(OBJECTS) starnavi
	cd ..

normbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o starnavi

testbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o starnavi	
	mv -f ./starnavi ~/
	chmod +x ~/starnavi

