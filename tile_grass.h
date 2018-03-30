#pragma once

#include "tile.h"

// class for grass terrain type
class GrassTile : public Tile {
public:
	// ctor
	GrassTile(GameInstance* game, Map* map, COORD const map_coords);
	// dtor
	~GrassTile();
};

