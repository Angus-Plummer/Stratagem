#pragma once
#include "terrain_tile.h"
class ForestTile :
	public TerrainTile
{
public:
	ForestTile(std::weak_ptr<TileMap> map, COORD const map_coords);
	~ForestTile();
};

