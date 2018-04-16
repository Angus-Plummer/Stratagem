#pragma once

#include "stdafx.h"
#include "Menu.h" // have to include menu as the game instance contains menus (not just pointers to them)

enum GameState {
	STATE_SETUP, // pregame setup, before game starts running
	STATE_SELECTING_UNIT, // no unit currently selected
	STATE_UNIT_SELECTING_ACTION, // selected unit is choosing an action
	STATE_UNIT_SELECTING_MOVEMENT, // selected unit is choosing tile for movement
	STATE_UNIT_SELECTING_ATTACK, // selected unit is choosing a unit to attack
	STATE_BETWEEN_TURNS, // game is currently between turns (showing players whos turn it is and awaiting input to start new turn)
	STATE_VICTORY // one of the players has won
};

// forward declarations
class Window;
class Map;
class Unit;

class GameInstance{
protected:

	Window* display_; // window that game is output to
	Map* game_map_; // game map
	Unit* selected_unit_; // the currently selected unit (nullptr if no unit selected)
	Menu context_menu_; // the context menu
	Button end_turn_button_; // button to end player turn
	Button surrender_button_; // surrender button

	// vector of pointers for each players units
	std::vector<Unit*> p1_units_;
	std::vector<Unit*> p2_units_;
	
	GameState state_;
	bool running_; // bool indicates whether this instance of the game is currently running
	int player_turn_;	// int to keep track of whose turn it is

public:
	
	GameInstance::GameInstance(); // default ctor
	GameInstance(Window &display); // ctor taking a window as arg
	// copy ctor
	// move ctor
	~GameInstance(); // dtor

	// copy assignment
	// move assignment
	
	// get access to the current game map, display and selected unit
	Window& get_display() const { return *display_; }
	void set_display(Window &display) { display_ = &display; }
	Map& get_map() const { return *game_map_; }
	Unit& get_selected_unit() const { return *selected_unit_; }

	// get whos turn it currently is
	int get_player_turn() const { return player_turn_; }

	// load in a map from the 5 premade maps
	void LoadMap(const int &map_id) const;
	// render the whole map
	void RenderMap() const;

	// add a unit to the game
	void AddUnit(Unit *new_unit, const Coord &pos);
	// remove a unit from the game
	void RemoveUnit(Unit *unit);

	// activates and deactivate a vector of units (sets their has moved or attacked this turn booleans and their internal state enum)
	void EnableUnits(std::vector<Unit*> units) const;
	void DisableUnits(std::vector<Unit*> units) const;

	// select the target unit
	void SelectUnit(Unit *unit);
	// unselect the currently selected unit
	void DeselectUnit();

	// could instead have state classes with enter and leave functions but this is adequate for now as states not that complex
	// handles entering and leaving the UNIT_SELECTING_ATTACK state 
	void ChooseAttack();
	void UnChooseAttack();
	// handles entering and leaving the UNIT_SELECTING_MOVEMENT state
	void ChooseMovement();
	void UnChooseMovement();

	// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
	void ShowContextMenu();
	void RemoveContextMenu(); // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS

	// sets up the start of a new turn
	void StartTurn();
	// carries out the end of turn, changes team whos turn it is and begins a new turn
	void EndTurn();
	// checks if all the units on the current team have acted and ends turn if they have
	void AutoEndTurn();
	// show / update the label for who's turn it currently is
	void ShowTurnLabel();
	// show end turn button
	void ShowEndTurnButton();
	// show surrender button
	void ShowSurrenderButton();
	// show special screen on turn change so players cant miss that it has happened
	void ShowTurnChangeScreen();

	// handles a mouse down event (i.e. the user clicking somewhere on the window)
	void HandleLeftMouseButtonDown(const Coord &location);

	// handles game victory
	void Victory(const int &team);

	// run the main game playing loop until the game ends
	void Run();
};
