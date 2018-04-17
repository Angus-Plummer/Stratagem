// stratagem.cpp : Defines the entry point for the console application.
//
// TODO: check whether it is approriate to include accessors, mutators and other simple function definitions in the header files or if they should actually be in cpp files
//		 check whether all accessors and mutators are actually used and appropriate. If not then remove.
//		 go through all functions and check whether I should be returning a const reference instead of reference so that the original object data cannot be modified

// Notes: Code is highly coupled. Difficult to change this now without major restructuring of the code, implementing e.g. the component pattern. Could move functions from the unit and tile classes that take info from the map into the map class.
//		  Could probably make a base class "Object" from which GameObjects and UIObjects derive, would then have easier access to where GameObjects are on the screen as well as on the map, making rendering and click interaction easier (no more get_x or y_offset everywhere!).
//		  Could update help text to take stat values from actual game objects so that the help text will automatically update when tiles or units are changed
//		  Could make the maps, tile types and stats, and unit types and stats readable from file so that they can be changed without recompiling 

// TODO: make it so a change in window size is detected and everything on window renders again if there is a change (perhaps also limit size or just lock size)

#include "stdafx.h"
#include "game_manager.h"
#include "window.h"

int main() {

	// set up window
	Window display(700, 550);
	// set default colour scheme
	display.set_colour_scheme(ColourScheme(BLACK,WHITE));
	// set the display of the static instance of the game
	GameManager::game().set_display(display);
	// run the game
	GameManager::game().Run();
	
	return 0;
}

