#pragma once
#include "terrain_tile.h"

// class for water terrain type
class WaterTile : public TerrainTile {
public:
	// ctor
	WaterTile(std::weak_ptr<TileMap> map, COORD const map_coords);
	// dtor
	~WaterTile();
};

