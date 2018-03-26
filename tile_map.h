#pragma once

#include "terrain_tile.h"
#include "grass_tile.h"
#include "water_tile.h"
#include "mountain_tile.h"
#include "forest_tile.h"

#include<iostream>
#include<memory>
#include<vector>

// class for the game map. The map is constructed from terrain tiles
class TileMap {
protected:
	// the map. A vector of shared pointers to terrain tiles
	std::vector<std::shared_ptr<TerrainTile>> map_;
	// map dimensions (number of tiles)
	int const width_;
	int const height_;
	// width from edge of map that each player can set up in
	int const set_up_width_;
public:
	// default ctor. makes empty map (just grass)
	TileMap();
	// ctor for loading premade map from 2d array
	TileMap(const int(&map)[10][10]);
	// dtor
	~TileMap();

	// Return number of rows
	int get_height() const { return height_; } 
	// Return number of columns
	int get_width() const { return width_; } 
	// Return position in array of element (m,n)
	int index(int const m, int const n) const;
	// returns the terrain tile at position i,j
	std::shared_ptr<TerrainTile> get_tile(int const i, int const j) const;

	// returns true if a unit is present on a given tile
	//bool UnitPresent(int const i, int const j) const;
	// returns the unit on a given tile (if there is one, else returns null pointer?)
	//Unit get_unit(int const i, int const j) const;

	// print map out to terminal
	void print() const;

};

