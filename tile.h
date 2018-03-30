#pragma once

// includes
#include "stdafx.h"
#include<string> // for holding text of tile type
#include "game_object.h"


// Abstract base class for terrain tile types
class Tile : public GameObject {
protected:
	// member data for tiles
	int move_cost_; // cost of movement across the tile (if unit can traverse the tile)
	int def_modifier_; // additive defensive modifier. Affects damage recieved by a unit on this tile.
	int atk_modifier_; // additive attack modifier. Affects damage dealt by a unit on this tile.
public:
	// default ctor, has weak pointer to the tile map to avoid circular referancing
	Tile(Map* map, COORD const map_coords);
	// dtor
	virtual ~Tile();

	// accessor functions
	int get_move_cost() const { return move_cost_; }
	int get_def_modifier() const { return def_modifier_; }
	int get_atk_modifier() const { return atk_modifier_; }

	// render
	void Render(Screen &display) const;

	
};

