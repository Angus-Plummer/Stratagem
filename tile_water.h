#pragma once

#include "tile.h"

// class for water terrain type
class WaterTile : public Tile {
public:
	// ctor
	WaterTile(Map &map, const Coord &map_coords);
	// dtor
	~WaterTile();
};

