#pragma once
#include "stdafx.h"
#include "button.h"
#include "game_instance.h"

// the states of the game when in the menu system. can only be in one of these states at any time
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
class Window;
class Tile;
class Unit;

// game manager class handles the menu interaction for game setup as well as the title screen and help screen
class GameManager{
protected:
	MenuState state_; // menu state
	std::vector<Button> buttons_; // vector that holds all the buttons that are currently in the window
	static GameManager game_; 	// there is one copy of the game and it is globally available through the static public function game()
	GameInstance instance_; // the instance of the game owned by the manager
	Window* display_; // pointer to the display being used
	int current_map_num; // holds the number of the currently selected map
	int team_size_; // number of starting units on each team
	int team_placing_; // the team that is currently allowed to select and place units
	std::unique_ptr<Unit> placing_unit_; // unit that is currently being placed on the map, this unit is owned by the game manager so use unique_ptr
	std::vector<std::unique_ptr<Unit>> units_placed_; // vector of all units that have been placed so far (they do not get placed into the game instance so they can be managed separately), the units are owned by the game manager while they are being placed (so use unique_ptr)

	// ---------- internal functions ---------- //

	// clear all menus from the vector of menus
	void ClearButtons();
	// render all buttons in the window
	void RenderButtons() const;

	// moves the game into the title screen state (shows ascii art and updates menus)
	void ShowTitleScreen();

	// moves the game into the help screen state
	void ShowHelpScreen();

	// helper function to wrap help text in window
	std::string WrapString(std::string& str) const;

	// moved the game into the map selection state
	void StartMapSelection();

	// move the game into the selecting teams size state
	void StartTeamSizeSelection();

	// starts the actual game running
	void PlayGame();

	// handles a mouse down event (i.e. the user clicking somewhere on the window)
	void HandleLeftMouseButtonDown(const Coord &window_location);

	// --------- unit placement functions ------------

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
	const int CountUnits() const;

	// shows the number of units the team currently placing units has placed
	void ShowUnitCounter() const;

	// flashes the unit counter red
	void FlashUnitCounter() const;

	// shows the type of unit that is currently being placed
	void ShowPlacingUnit() const;

	// select a new unit to be placed
	void SetPlacingUnit(Unit *placing_unit);

	// removes the unit being placed and replaces with a nullptr, also resets the tiles
	void RemovePlacingUnit();


public:
	// ctors
	GameManager();
	GameManager(Window &display);
	// dtor
	~GameManager();

	// global access to the static game manager object
	static GameManager& game() { return game_; }

	// get a reference to the current game instance
	GameInstance& get_instance();
	
	// get a reference to the display window
	const Window& get_display() const;
	
	// set the display window
	void set_display(Window &display);

	// runs the main game loop until the user quits the game
	void Run();
};

