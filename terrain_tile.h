#pragma once

// includes
#include "stdafx.h"
#include<string> // for holding text of tile type
#include<memory>
#include "game_object.h"

class TileMap; // forward declaration of the TileMap class

// Abstract base class for terrain tile types
class TerrainTile : public GameObject {
protected:
	// member data for tiles
	int move_cost_; // cost of movement across the tile (if unit can traverse the tile)
	double def_modifier_; // multiplicative defensive modifier. Affects damage recieved by a unit on this tile.
	double atk_modifier_; // multiplicative attack modifier. Affects damage dealt by a unit on this tile.
	std::weak_ptr<TileMap> parent_map_; // pointer to the tilemap that this terrain tile belongs to.
public:
	// default ctor, has weak pointer to the tile map to avoid circular referancing
	TerrainTile(std::weak_ptr<TileMap> map, COORD const map_coords);
	//TerrainTile(COORD const map_coords);
	// dtor
	virtual ~TerrainTile();

	// accessor functions
	int get_move_cost() const { return move_cost_; }
	double get_def_modifier() const { return def_modifier_; }
	double get_atk_modifier() const { return atk_modifier_; }
	
};

