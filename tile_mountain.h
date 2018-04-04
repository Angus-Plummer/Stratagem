#pragma once

#include "tile.h"

class MountainTile :
	public Tile
{
public:
	MountainTile(Map &map, const Coord &map_coords);
	~MountainTile();
};

