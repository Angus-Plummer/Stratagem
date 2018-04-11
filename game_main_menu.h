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
	STATE_TEAM_SIZE_SELECT,
	STATE_UNIT_PLACEMENT,
	STATE_GAME_RUNNING,
	STATE_QUITTING
};

// forward declarations
class Screen;
class GameInstance;
class Tile;
class Unit;

class GameMainMenu{
protected:
	MenuState state_;
	std::vector<Button> buttons_;
	GameInstance* instance_;
	Screen* display_;
	int current_map_; // holds the number of the currently selected map
	int team_size_; // number of starting units on each team
	int team_placing_; // the team that is currently allowed to select and place units
	Unit* placing_unit_; // unit that is currently being placed on the map
	std::vector<Unit*> units_;
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

	// move the game into the selecting teams size state
	void StartTeamSizeSelection();

	// --------- unit placement ------------

	// moves the game into the unit placement state
	void StartUnitPlacement();

	// confirms the current unit placement (if the correct number of units have been placed)
	void ConfirmUnitPlacement();

	// remove all units of the team that is currently placing units from the game map (allows them to try placing again. also run for both teams when backing out of unit selection)
	void ClearUnits();

	// returns a vector of the tiles that placing_unit_ can be placed on
	std::vector<Tile*> PlaceableTiles() const;

	// highlight placable tiles (sets placable tiles to highlighted and renders them)
	void HighlightPlaceableTiles();

	// reset tiles (sets all to unhighlighted and renders)
	void ResetTiles();

	// counts the number of units that have been placed by the current team
	int CountUnits();
	
	// shows the number of units the team currently placing units has placed
	void ShowUnitCounter();

	// shows the type of unit that is currently being placed
	void ShowPlacingUnit();

	// select a new unit to be placed
	void SetPlacingUnit(Unit *placing_unit);

	// removes the unit being placed and replaces with a nullptr, also resets the tiles
	void RemovePlacingUnit();

	// --------- end of unit placement ----------

	// starts the actual game running
	void PlayGame();

	// handles a mouse down event (i.e. the user clicking somewhere on the screen)
	void HandleLeftMouseButtonDown(const Coord &screen_location);

	// runs the main game loop until the user quits the game
	void Run();
};

