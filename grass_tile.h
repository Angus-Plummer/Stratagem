#pragma once
#include "terrain_tile.h"

// class for grass terrain type
class GrassTile : public TerrainTile {
public:
	// ctor
	GrassTile(std::weak_ptr<TileMap> map, COORD const map_coords);
	// dtor
	~GrassTile();
};

