#pragma once
#include "stdafx.h"
#include "menu.h"
// NEED: title screen (start, help, quit)
//		 map selection (menu with list of maps / next and previous map buttons with buttons to right of rendered maps) also go back to title screen
//		 unit placement (1 team at a time, place units on map (warrior -> rogue -> archer) undo button and finish button when all added. next team places but can see previous teams units. Then show black screen with click anywhere to start -> game instance starts. also back button to go back to map selection.
//		 when game instance finishes go back to title screen

enum MenuState {
	STATE_TITLE_SCREEN,
	STATE_HELP_SCREEN,
	STATE_MAP_SELECTION,
	STATE_UNIT_PLACEMENT,
	STATE_GAME_RUNNING,
	STATE_QUITTING
};

// forward declarations
class Screen;
class GameInstance;

class GameMainMenu{
protected:
	MenuState state_;
	std::vector<Button> buttons_;
	GameInstance* instance_;
	Screen* display_;
	int current_map_; // holds the number of the currently selected map
public:
	GameMainMenu(Screen &display);
	~GameMainMenu();

	// clear all menus from the vector of menus
	void ClearButtons();
	// render all menus
	void RenderButtons();

	// moves the game into the title screen state (shows ascii art and updates menus)
	void ShowTitleScreen();

	// moves the game into the help screen state
	void ShowHelpScreen();

	// moved the game into the map selection state
	void StartMapSelection();

	// moves the game into the unit placement state
	void StartUnitPlacement();

	// remove all units of a team from the game map (allows them to try placing again. also run for both teams when backing out of unit selection)
	void ResetUnitPlacement(const int &team);

	// starts the actual game running
	void PlayGame();

	// handles a mouse down event (i.e. the user clicking somewhere on the screen)
	void HandleLeftMouseButtonDown(const Coord &screen_location);

	// runs the main game loop until the user quits the game
	void Run();
};

