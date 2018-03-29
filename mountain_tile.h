#pragma once
#include "terrain_tile.h"
class MountainTile :
	public TerrainTile
{
public:
	MountainTile(std::weak_ptr<TileMap> map, COORD const map_coords);
	~MountainTile();
};

