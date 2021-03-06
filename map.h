#pragma once
#include "stdafx.h"
#include "raw_maps.h"

// forward declarations
class Tile;
class Unit;

// class for the game map. The map is constructed from terrain tiles and has access to the units on it
class Map {
protected:
	// map dimensions (number of tiles)
	int map_width_;
	int map_height_;
	// width from edge of map that each player can set up in
	int set_up_width_;
	// the map. A vector of shared pointers to terrain tiles (map is sole owner the tiles so use unique_ptr)
	std::vector<std::vector<std::unique_ptr<Tile>>> map_;
	// the list of units on the map (map just has access to the units but does not own them so use raw pointer)
	std::vector<Unit*> units_;

	// height and width of game map tiles in console cells
	int tile_width_;
	int tile_height_;
	// offset of tiles from edge of window in console cells
	int map_x_offset_;
	int map_y_offset_;


public:
	// default ctor. makes empty map (just grass)
	Map();
	// ctor for making map of specific dimensinos
	Map(const int &width, const int &height, const int &set_up_width);
	// copy ctor
	Map(const Map &map);
	// move ctor
	Map(Map &&map);
	// dtor
	~Map();

	// copy assigment
	Map& operator=(const Map &map);
	// move assigment
	Map& operator=(Map &&map);

	// accessors and mutators
	const int& get_map_height() const;
	const int& get_map_width() const;
	const int& get_set_up_width() const;
	const std::vector<Unit*> get_units() const;
	const int& get_tile_width() const;
	const int& get_tile_height() const;
	const int& get_map_x_offset() const;
	const int& get_map_y_offset() const;

	// clears the map. (deletes all tiles and resets 2d map vector)
	void Clear();
	// loads in a map in the form of a 2d vector of id numbers
	void LoadMap(const std::vector<std::vector<int>> &map);

	// returns the terrain tile a map coordinate
	Tile* GetTile(const Coord &position) const;
	// returns the tile at a console coordinate
	Tile* GetTileFromConsoleCoord(const Coord &position) const;

	// adds a unit to the map
	void AddUnit(Unit *new_unit, const Coord &position);
	// removes a unit from the map
	void RemoveUnit(Unit *unit);

	// returns true if a unit is present on a given tile
	const bool UnitPresent(const Coord &position) const;

	// returns the unit on a given tile (if there is one, else returns null pointer)
	Unit* GetUnit(const Coord &position) const;

	// returns a vector of pointers to the tiles that are adjacent to input tile coordinate
	std::vector<Tile*> AdjacentTo(const Tile *tile) const;

	// Renders the map on the console
	void Render() const;
	// Renders a specific tile the unit on it if there is one
	void Render(Coord coord) const;

	// unhiglights all tiles on the console
	void ResetTiles() const;

};

