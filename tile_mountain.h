#pragma once

#include "tile.h"

class MountainTile :
	public Tile
{
public:
	MountainTile(GameInstance* game, Map* map, COORD const map_coords);
	~MountainTile();
};

