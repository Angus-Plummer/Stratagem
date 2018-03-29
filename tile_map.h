#pragma once

#include "grass_tile.h"
#include "water_tile.h"
#include "mountain_tile.h"
#include "forest_tile.h"

#include<iostream>
#include<memory>
#include<vector>

// class for the game map. The map is constructed from terrain tiles
class TileMap : public std::enable_shared_from_this<TileMap> {
protected:
	// the map. A vector of shared pointers to terrain tiles
	std::vector<std::vector<std::shared_ptr<TerrainTile>>> map_;
	// map dimensions (number of tiles)
	int const map_width_;
	int const map_height_;
	// width from edge of map that each player can set up in
	int const set_up_width_;
public:
	// default ctor. makes empty map (just grass)
	TileMap();
	// ctor for making map of specific dimensinos
	TileMap(int const width, int const height, int const set_up_width);
	// dtor
	~TileMap();
	
	// loads in a map in the form of a 2d vector of id numbers
	void TileMap::LoadMap(std::vector<std::vector<int>> const &map);


	// Return number of rows and columns
	int get_map_height() const { return map_height_; } 
	int get_map_width() const { return map_width_; }

	// Return position in array of element (m,n)
	int index(int const m, int const n) const;

	// returns the terrain tile at position i,j
	std::shared_ptr<TerrainTile> get_tile(int const i, int const j) const;

	// returns true if a unit is present on a given tile
	//bool UnitPresent(int const i, int const j) const;
	// returns the unit on a given tile (if there is one, else returns null pointer?)
	//Unit get_unit(int const i, int const j) const;


};

