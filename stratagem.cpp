// stratagem.cpp : Entry point for the console application.

#include "stdafx.h"
#include "game_manager.h"
#include "window.h"

int main() {

	// set up window
	Window display(700, 550);
	// set default colour scheme
	display.set_colour_scheme(ColourScheme(BLACK,WHITE));
	// set the display of the static instance of the game
	GameManager::Game().set_display(display);
	// run the game
	GameManager::Game().Run();
	
	return 0;
}

