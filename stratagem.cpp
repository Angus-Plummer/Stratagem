// stratagem.cpp : Defines the entry point for the console application.
//
// TODO: include proper error handling ( usually needed in places with exit(1) )
//		 use smart pointers instead of regular pointers where appropriate
//		 implement rule of 3 (or 5) : make copy and move constructors and assignment operators for each class that needs it
//		 check where passing pointers to functions and see if just a reference will do instead
//		 make the constructors and destructors of abstracrt classes protected
//		 go through classes and determine whether public, protected, and private status are right for members
//		 try using switch(condition){ case (condition1): ... case (condition2) ... instead of messy if else if blocks
//		 find where enums may be appropriate as a way of giving different objects ids
//		 check whether it is approriate to include accessors, mutators and other simple function definitions in the header files or if they should actually be in cpp files
//		 check whether all accessors and mutators are actually used and appropriate. If not then remove.
//		 go through all functions and check whether I should be returning a const reference instead of reference so that the original object data cannot be modified

// Sad realities: Code is highly coupled. Difficult to change this now without major restructuring of the code, implementing e.g. the component pattern

// TODO: sort out destructors and deleting units etc (all places where new is used must have a delete somewhere) or use smart pointers
//		 make it so a change in window size is detected and everything on window renders again if there is a change (perhaps also limit size or just lock size)

#include "stdafx.h"
#include "game_manager.h"
#include "game_instance.h"
#include "window.h"
#include "map.h"
#include "move_sequence.h"
#include "raw_maps.h"
#include "unit_archer.h"
#include "unit_warrior.h"
#include "unit_rogue.h"
#include "button.h"
#include "menu.h"

GameObject* selected;

int main() {

	// set up window
	Window display(620, 550);
	// set default colour scheme
	display.set_colour_scheme(ColourScheme(BLACK,WHITE));
	// instanciate a GameManager
	GameManager::game().set_display(display);
	// run the game
	GameManager::game().Run();
	
	return 0;
}

