#pragma once

#include "stdafx.h"
#include "game_object.h"

// forward declarations
class Map;

// Abstract base class for terrain tile types
class Tile : public GameObject {
protected:
	// member data for tiles
	char tile_marker_; // representation of tile type as char for when printing map
	int move_cost_; // cost of movement across the tile (if unit can traverse the tile)
	int def_modifier_; // additive defensive modifier. Affects damage recieved by a unit on this tile.
	int atk_modifier_; // additive attack modifier. Affects damage dealt by a unit on this tile.
	bool highlighted_; // if tile is currently highlighted for movement then this is true, otherwise false
	// pointer to the map that this tile belongs to
	Map* parent_map_;

	// helper function returns a pointer to a copy of this object
	virtual Tile* clone_impl() const = 0;

public:
	// default ctor, has weak pointer to the tile map to avoid circular referancing
	Tile(Map &map, const Coord &map_coords);
	// dtor
	virtual ~Tile();

	// get a unique pointer to a copy of the tile
	std::unique_ptr<Tile> clone() const { return std::unique_ptr<Tile>(clone_impl()); }

	// accessor functions
	const int& get_move_cost() const { return move_cost_; }
	const int& get_def_modifier() const { return def_modifier_; }
	const int& get_atk_modifier() const { return atk_modifier_; }
	void set_highlighted(const bool &highglighted);
	const bool& get_highlighted() const { return highlighted_; }

	// get the colour scheme (highlighted scheme if highlighted is true)
	const ColourScheme& get_colour_scheme() const;

	// function to check if two tiles are adjacent
	const bool AdjacencyTest(const Tile *test_tile) const;

	// render the tile on the console
	void Render() const;

	
};

