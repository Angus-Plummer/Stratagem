#pragma once

#include "stdafx.h"

// forward declarations
class Screen;
class Map;
class Unit;
class Menu;
class Button;

class Game{
protected:
	// there is one copy of the game instance and it is globally available through the static public function instance()
	static Game instance_;

	Screen* display_; // screen that game is output to
	Map* game_map_; // game map
	Unit* selected_unit_; // the currently selected unit (nullptr if no unit selected)
	Menu* context_menu_; // the context menu
	Menu* end_turn_button_menu_; // menu that holds the button to end player turn


	// vector of pointers for each players units
	std::vector<Unit*> p1_units_;
	std::vector<Unit*> p2_units_;
	
	bool running_; // bool indicates whether this instance of the game is currently running
	int player_turn_;	// int to keep track of whose turn it is

public:
	
	Game::Game(); // default ctor
	Game(Screen &display); // ctor taking a screen as arg
	~Game(); // dtor
	
	// global access to the current instance of the game
	static Game& instance() { return instance_; }

	// get whos turn it currently is
	int get_player_turn() const { return player_turn_; }
	
	// get access to the current game map, display and selected unit
	Screen& get_display() const { return *display_; }
	Map& get_map() const { return *game_map_; }
	Unit& get_selected_unit() const { return *selected_unit_; }

	// select the target unit
	void SelectUnit(Unit *unit);
	// unselect the currently selected unit
	void DeselectUnit();

	// load in a map from the 5 premade maps
	void LoadMap(const int &map_id) const;
	// render the whole map
	void RenderMap() const;

	// add a unit to the game
	void AddUnit(Unit *new_unit, const Coord &pos);
	// remove a unit from the game
	void RemoveUnit(Unit *unit);

	// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
	void ShowContextMenu();
	void RemoveContextMenu(); // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS

	// carries out the end of turn and begins a new turn
	void EndTurn();

	// checks if all the units on the current team have acted and ends turn if they have
	void AutoEndTurn();

	// activates a vector of units (resets their has moved or attacked this turn booleans)
	void EnableUnits(std::vector<Unit*> units) const;

	// show / update the label for who's turn it currently is
	void ShowTurnLabel();

	// show end turn button
	void ShowEndTurnButton();

	// show quit game button
	void ShowQuitGameButton();

	// handles a mouse down event (i.e. the user clicking somewhere on the screen)
	void HandleLeftMouseButtonDown(const Coord &location);

	// run the main game loop until the user quits
	void Run();
};
