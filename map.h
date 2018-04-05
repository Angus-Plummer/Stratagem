#pragma once

#include "stdafx.h"
#include "raw_maps.h"

// forward declarations
class Tile;
class Unit;

// class for the game map. The map is constructed from terrain tiles
class Map {
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
	Map(const int &width, const int &height, const int &set_up_width);
	// dtor
	~Map();

	// clears the map. (deletes all tiles and resets 2d map vector)
	void Clear();
	// loads in a map in the form of a 2d vector of id numbers
	void LoadMap(const std::vector<std::vector<int>> &map);
	// adds a unit to the map
	void AddUnit(Unit *new_unit, const Coord &position);

	// Return number of rows and columns
	int get_map_height() const { return map_height_; } 
	int get_map_width() const { return map_width_; }

	// returns the terrain tile a map coordinate
	Tile* GetTile(const Coord &position) const;
	// returns the tile at a console coordinate
	Tile* GetTileFromConsoleCoord(const Coord &position) const;

	// returns a vector of pointers to the units active on the map
	std::vector<Unit*> get_units() const { return units_; }

	// Renders the map on a screen
	void Render() const;

	// unhiglights all tiles on the screen
	void ResetTiles() const;

	// returns true if a unit is present on a given tile
	bool UnitPresent(const Coord &position) const;

	// returns the unit on a given tile (if there is one, else returns null pointer)
	Unit* GetUnit(const Coord &position) const;

	// select a tile on the map using the mouse
	Tile* SelectTile() const;

	// select a unit on the map using the mouse
	Unit* SelectUnit() const;

	// returns a vector of pointers to the tiles that are adjacent to input tile coordinate
	std::vector<Tile*> AdjacentTo(const Tile *tile) const;

};

