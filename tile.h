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
	bool highlighted_; // if tile is currently highlighted (changes colour scheme of tile)
	Map* parent_map_; // pointer to the map that this tile belongs to

	// helper function returns a pointer to a copy of the object
	virtual Tile* CloneHelper() const = 0;

public:
	// constructor and destructor
	Tile(Map &map, const Coord &map_coords);
	virtual ~Tile();

	// accessor and mutator functions
	const int& get_move_cost() const;
	const int& get_def_modifier() const;
	const int& get_atk_modifier() const;
	const bool& get_highlighted() const;
	void set_highlighted(const bool &highlighted);

	// get a unique pointer to a copy of the tile
	std::unique_ptr<Tile> Clone() const;

	// get the colour scheme (highlighted scheme if highlighted is true)
	const ColourScheme& get_colour_scheme() const;

	// function to check if two tiles are adjacent on the map
	const bool AdjacencyTest(const Tile *test_tile) const;

	// render the tile on the console
	void Render() const;

	
};

