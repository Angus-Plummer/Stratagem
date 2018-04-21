#pragma once

#include "tile.h"

// class for mountain terrain type
class MountainTile : public Tile {
protected:
	// helper function returns a pointer to a copy of this object
	MountainTile* CloneHelper() const;
public:
	// ctor and dtor
	MountainTile(Map &map, const Coord &map_coords);
	~MountainTile();
};

