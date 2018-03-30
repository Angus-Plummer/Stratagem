#pragma once

#include "stdafx.h"
#include "game_object.h"

// Abstract base class for terrain tile types
class Tile : public GameObject {
protected:
	// member data for tiles
	int move_cost_; // cost of movement across the tile (if unit can traverse the tile)
	int def_modifier_; // additive defensive modifier. Affects damage recieved by a unit on this tile.
	int atk_modifier_; // additive attack modifier. Affects damage dealt by a unit on this tile.
	int highlighted_colour_scheme_; // colour scheme when highlighted
	bool highlighted_; // if tile is currently highlighted for movement then this is true, otherwise false

public:
	// default ctor, has weak pointer to the tile map to avoid circular referancing
	Tile(GameInstance* game, Map* map, COORD const map_coords);
	// dtor
	virtual ~Tile();

	// accessor functions
	int get_move_cost() const { return move_cost_; }
	int get_def_modifier() const { return def_modifier_; }
	int get_atk_modifier() const { return atk_modifier_; }
	void set_highlighted(bool const highglighted);
	bool get_highlighted() const { return highlighted_; }

	// get the colour scheme (highlighted scheme if highlighted is true)
	int get_colour_scheme() const;

	// render the tile on a screen
	void Render(Screen const &display) const;

	
};

