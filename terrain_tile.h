#pragma once

// includes
#include "stdafx.h"
#include<string> // for holding text of tile type
#include<iostream>

// Abstract base class for terrain tile types
class TerrainTile{
protected:
	// member data for tiles
	std::string type_; // string of name of tile type
	char marker_; // representation of tile type as char for when printing map
	int colour_scheme_; // colour scheme of terminal output
	int move_cost_; // cost of movement across the tile (if unit can traverse the tile)
	double def_modifier_; // multiplicative defensive modifier. Affects damage recieved by a unit on this tile.
	double atk_modifier_; // multiplicative attack modifier. Affects damage dealt by a unit on this tile.
public:
	// default ctor
	TerrainTile();
	// copy ctor
	TerrainTile(TerrainTile const &input_tile);
	// dtor
	virtual ~TerrainTile();

	// accessor functions
	std::string get_type() const { return type_; }
	char get_marker() const { return marker_; }
	int get_colour_scheme() const { return colour_scheme_; }
	int get_move_cost() const { return move_cost_; }
	double get_def_modifier() const { return def_modifier_; }
	double get_atk_modifier() const { return atk_modifier_; }
	
};

