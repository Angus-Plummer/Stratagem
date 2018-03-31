#pragma once

#include "tile.h"

class MountainTile :
	public Tile
{
public:
	MountainTile(Map &map, const COORD &map_coords);
	~MountainTile();
};

