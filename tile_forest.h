#pragma once

#include "tile.h"

// class for forest terrain type
class ForestTile : public Tile {
protected:
	// helper function returns a pointer to a copy of this object
	ForestTile * clone_impl() const;
public:
	// ctor and dtor
	ForestTile(Map &map, const Coord &map_coords);
	~ForestTile();
};

