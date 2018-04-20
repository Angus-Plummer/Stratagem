// stratagem.cpp : Entry point for the console application.
//
// Notes: Code is highly coupled. Difficult to change this now without major restructuring of the code, implementing e.g. the component pattern. Could move functions from the unit and tile classes that take info from the map into the map class.
//		  Could probably make a base class "Object" from which GameObjects and UIObjects derive, would then have easier access to where GameObjects are on the screen as well as on the map, making rendering and click interaction easier (no more get_x or y_offset everywhere!).
//		  Could update help text to take stat values from actual game objects so that the help text will automatically update when tiles or units are changed
//		  Could make the maps, tile types and stats, and unit types and stats readable from file so that they can be changed without recompiling 
//		  When forcing the console window size and the buffer size I discovered it is possible to change the font, font size, and font dimension ratio, could do this to have higher resolution and more detailed tiles, units, buttons, etc. Doing this would also increase rendering time so would have to find a balance between aesthic quality and performance.

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

