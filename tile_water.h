#pragma once

#include "tile.h"

// class for water terrain type
class WaterTile : public Tile {
public:
	// ctor
	WaterTile(Map &map, const COORD &map_coords);
	// dtor
	~WaterTile();
};

