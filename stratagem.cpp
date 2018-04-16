// stratagem.cpp : Defines the entry point for the console application.
//
// TODO: check where passing pointers to functions and see if just a reference will do instead
//		 make the constructors and destructors of abstracrt classes protected
//		 go through classes and determine whether public, protected, and private status are right for members
//		 try using switch(condition){ case (condition1): ... case (condition2) ... instead of messy if else if blocks
//		 find where enums may be appropriate as a way of giving different objects ids
//		 check whether it is approriate to include accessors, mutators and other simple function definitions in the header files or if they should actually be in cpp files
//		 check whether all accessors and mutators are actually used and appropriate. If not then remove.
//		 go through all functions and check whether I should be returning a const reference instead of reference so that the original object data cannot be modified

// Sad realities: Code is highly coupled. Difficult to change this now without major restructuring of the code, implementing e.g. the component pattern

// TODO: make it so a change in window size is detected and everything on window renders again if there is a change (perhaps also limit size or just lock size)

#include "stdafx.h"
#include "game_manager.h"
#include "window.h"

int main() {

	// set up window
	Window display(620, 550);
	// set default colour scheme
	display.set_colour_scheme(ColourScheme(BLACK,WHITE));
	// set the display of the static instance of the game
	GameManager::game().set_display(display);
	// run the game
	GameManager::game().Run();
	
	return 0;
}

