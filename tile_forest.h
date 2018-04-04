#pragma once

#include "tile.h"

class ForestTile :
	public Tile
{
public:
	ForestTile(Map &map, const Coord &map_coords);
	~ForestTile();
};

