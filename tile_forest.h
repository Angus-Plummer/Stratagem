#pragma once

#include "tile.h"

class ForestTile :
	public Tile
{
public:
	ForestTile(Map &map, const COORD &map_coords);
	~ForestTile();
};

