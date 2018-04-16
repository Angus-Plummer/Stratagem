#pragma once

#include "tile.h"

class MountainTile : public Tile {
protected:
	// helper function returns a pointer to a copy of this object
	MountainTile * clone_impl() const { return new MountainTile(*this); };
public:
	MountainTile(Map &map, const Coord &map_coords);
	~MountainTile();
};

