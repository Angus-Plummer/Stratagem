#pragma once
#include "tile.h"

// class for water terrain type
class WaterTile : public Tile {
public:
	// ctor
	WaterTile(Map* map, COORD const map_coords);
	// dtor
	~WaterTile();
};

