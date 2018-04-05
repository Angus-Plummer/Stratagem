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

	// vector of pointers for each players units
	std::vector<Unit*> p1_units_;
	std::vector<Unit*> p2_units_;
	
	int player_turn_;	// int to keep track of whose turn it is

public:
	
	Game::Game(); // default ctor
	Game(Screen &display); // ctor taking a screen as arg
	~Game(); // dtor
	
	// global access to the current instance of the game
	static Game& instance() { return instance_; }
	
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

	// add a unit to the map
	void AddUnit(Unit *unit, const Coord &pos);

	// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
	void ShowContextMenu();
	void RemoveContextMenu(); // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS

	// handles a mouse down event (i.e. the user clicking somewhere on the screen)
	void HandleLeftMouseButtonDown(const Coord &location);

};
