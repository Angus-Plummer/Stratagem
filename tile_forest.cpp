#include "stdafx.h"
#include "tile_forest.h"

// ctor
ForestTile::ForestTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
// tile output info
type_ = "forest";
tile_marker_ = char(237); // Y with an accent (looks like a tree)
default_colour_scheme_ = ColourScheme(DARK_GREEN, BROWN);
highlighted_colour_scheme_ = ColourScheme(DARK_GREEN, RED);
// stats
move_cost_ = 2;
def_modifier_ = 2;
atk_modifier_ = 0;
}

// dtor
ForestTile::~ForestTile(){}

// helper function returns a pointer to a copy of this object
ForestTile* ForestTile::clone_impl() const { return new ForestTile(*this); }
