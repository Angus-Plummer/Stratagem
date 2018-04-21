#pragma once

#include "stdafx.h"
#include "Menu.h" // have to include menu as the game instance contains menus (not just pointers to them)

// enum for unique game instance states, the game instance may only be in one state at a time
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

// game instance class handles the actual gameplay of the game
class GameInstance{
protected:

	std::unique_ptr<Map> game_map_; // game map
	Unit* selected_unit_; // the currently selected unit (nullptr if no unit selected)
	Menu context_menu_; // the context menu
	Button end_turn_button_; // button to end player turn
	Button surrender_button_; // surrender button

	// vector of pointers for each players units
	std::vector<std::unique_ptr<Unit>> p1_units_;
	std::vector<std::unique_ptr<Unit>> p2_units_;
	
	GameState state_; // current game state (enums defined above)
	bool running_; // bool indicates whether this instance of the game is currently running
	int player_turn_;	// int to keep track of whose turn it is

	//---------- internal functions ----------//

	// sets up the start of a new turn
	void StartTurn();
	// carries out the end of turn, changes team whos turn it is and begins a new turn
	void EndTurn();
	// checks if all the units on the current team have acted and ends turn if they have
	void AutoEndTurn();

	// handles game victory
	void Victory(const int &team);

	// handles entering and leaving the UNIT_SELECTING_ATTACK state 
	// (could instead have state classes with enter and leave functions but this is adequate for now as states not that complex)
	void ChooseAttack();
	void UnChooseAttack();
	// handles entering and leaving the UNIT_SELECTING_MOVEMENT state
	void ChooseMovement();
	void UnChooseMovement();
	
	// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
	void ShowContextMenu();
	void RemoveContextMenu();

	// show / update the label for who's turn it currently is
	void ShowTurnLabel() const;
	// show end turn button
	void ShowEndTurnButton();
	// show surrender button
	void ShowSurrenderButton();
	// show special screen on turn change so players cant miss that it has happened
	void ShowTurnChangeScreen();

	// handles a mouse down event (i.e. the user clicking somewhere on the window)
	void HandleMouseClick(const Coord &location);

public:
	
	GameInstance::GameInstance(); // default ctor
	GameInstance(const GameInstance &instance); // copy ctor
	GameInstance(GameInstance &&instance); // move ctor
	~GameInstance(); // dtor

	// copy assignment
	GameInstance& operator=(const GameInstance &instance);
	// move assignment
	GameInstance& operator=(GameInstance &&instance);
	
	// accessors and mutators
	Map& get_map() const;
	Unit& get_selected_unit() const;
	const int& get_player_turn() const;

	// load in a map from the 5 premade maps using an integer map id
	void LoadMap(const int &map_id) const;
	// render the whole map (tiles + units)
	void RenderMap() const;

	// add a unit to the game (takes unique pointer as transferring ownership)
	void AddUnit(std::unique_ptr<Unit> new_unit, const Coord &pos);
	// remove a unit from the game
	void RemoveUnit(Unit *unit);

	// select the target unit
	void SelectUnit(Unit *unit);
	// unselect the currently selected unit
	void DeselectUnit();

	// run the main game playing loop until the game ends
	void Run();
};
