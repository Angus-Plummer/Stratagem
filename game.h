#pragma once

#include "stdafx.h"

// forward declarations
class Screen;
class Map;
class Unit;

class Game{
protected:
	// there is one copy of the game instance and it is globally available through the static public function instance()
	static Game instance_;
	// contains pointers to the game map, the display and the currently selected unit
	Map* game_map_; 
	Screen* display_;
	Unit* selected_unit_; 
	// contains a vector of pointers for each players units
	std::vector<Unit*> p1_units_;
	std::vector<Unit*> p2_units_;
	// int to keep track of whose turn it is
	int player_turn_;
	// UIObject : actions menu
public:
	
	Game::Game();	// default ctor
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
	void Game::DeselectUnit();

	// load in a map from the 5 premade maps
	void LoadMap(const int &map_id) const;
	// render the whole map
	void RenderMap() const;

	// add a unit to the map
	void AddUnit(Unit *unit, const Coord &pos);

	// select a unit
	void SelectUnitOnMap() const;

	// handles a mouse down event (i.e. the user clicking somewhere on the screen)
	void HandleLeftMouseButtonDown(const Coord &location);

};
