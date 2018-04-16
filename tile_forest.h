#pragma once

#include "tile.h"

class ForestTile : public Tile {
protected:
	// helper function returns a pointer to a copy of this object
	ForestTile * clone_impl() const { return new ForestTile(*this); };
public:
	ForestTile(Map &map, const Coord &map_coords);
	~ForestTile();
};

