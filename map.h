#pragma once

#include "tile_grass.h"
#include "tile_water.h"
#include "tile_mountain.h"
#include "tile_forest.h"
#include "unit.h"
#include "move_sequence.h"

#include<iostream>
#include<memory>
#include<vector>
#include<algorithm>
#include<set>

// class for the game map. The map is constructed from terrain tiles
class Map : public std::enable_shared_from_this<Map> {
protected:
	// the map. A vector of shared pointers to terrain tiles
	std::vector<std::vector<Tile*>> map_;
	// map dimensions (number of tiles)
	int const map_width_;
	int const map_height_;
	// width from edge of map that each player can set up in
	int const set_up_width_;
	// the list of units on the map
	std::vector<Unit*> units_;
public:
	// default ctor. makes empty map (just grass)
	Map();
	// ctor for making map of specific dimensinos
	Map(int const width, int const height, int const set_up_width);
	// dtor
	~Map();
	
	// loads in a map in the form of a 2d vector of id numbers
	void LoadMap(std::vector<std::vector<int>> const &map);
	// adds a unit to the map
	void AddUnit(Unit* new_unit, COORD const position);

	// Return number of rows and columns
	int get_map_height() const { return map_height_; } 
	int get_map_width() const { return map_width_; }

	// returns the terrain tile at position i,j
	Tile* GetTile(COORD const position) const;

	// Renders the map on a screen
	void Render(Screen &display) const;

	// returns true if a unit is present on a given tile
	bool UnitPresent(COORD const position) const;

	// returns the unit on a given tile (if there is one, else returns null pointer)
	Unit* GetUnit(COORD const position) const;

	// select an object on the map
	GameObject* Select(Screen &display) const;

	std::vector<Tile*> AdjacentTo(COORD const position) const;

	// returns a vector of pointers to the tiles that are reachable by a given unit
	std::vector<Tile*> ReachableTiles(Unit* unit) const;

};

