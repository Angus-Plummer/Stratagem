#pragma once

#include "stdafx.h"
#include "raw_maps.h"

// forward declarations. This seems avoidable but it works for now
class Screen;
class GameInstance;
class Tile;
class Unit;

// class for the game map. The map is constructed from terrain tiles
class Map : public std::enable_shared_from_this<Map> {
protected:
	GameInstance * the_game_;
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
	Map(GameInstance* game);
	// ctor for making map of specific dimensinos
	Map(GameInstance* game, int const width, int const height, int const set_up_width);
	// dtor
	~Map();
	// clears the map. (deletes all tiles and resets 2d map vector)
	void Clear();
	
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
	void Render(Screen const &display) const;

	// returns true if a unit is present on a given tile
	bool UnitPresent(COORD const position) const;

	// returns the unit on a given tile (if there is one, else returns null pointer)
	Unit* GetUnit(COORD const position) const;

	// select a tile on the map using the mouse
	Tile* SelectTile(Screen const &display) const;

	// select a unit on the map using the mouse
	Unit* SelectUnit(Screen const &display) const;

	// returns a vector of pointers to the tiles that are adjacent to input tile coordinate
	std::vector<Tile*> AdjacentTo(COORD const position) const;

};

