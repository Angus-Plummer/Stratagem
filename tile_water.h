#pragma once

#include "tile.h"

// class for water terrain type
class WaterTile : public Tile {
protected:
	// helper function returns a pointer to a copy of this object
	WaterTile* clone_impl() const;
public:
	// ctor and dtor
	WaterTile(Map &map, const Coord &map_coords);
	~WaterTile();
};

