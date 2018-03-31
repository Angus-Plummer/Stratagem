#pragma once

#include "tile.h"

// class for grass terrain type
class GrassTile : public Tile {
public:
	// ctor
	GrassTile(Map &map, const COORD &map_coords);
	// dtor
	~GrassTile();
};

