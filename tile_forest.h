#pragma once
#include "tile.h"
class ForestTile :
	public Tile
{
public:
	ForestTile(Map* map, COORD const map_coords);
	~ForestTile();
};

