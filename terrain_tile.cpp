#include "stdafx.h"
#include "terrain_tile.h"
#include "tile_map.h"

TerrainTile::TerrainTile(std::weak_ptr<TileMap> map, COORD const map_coords) : GameObject(){
	// check if the coordinates are valid on the map
	if (auto map_ptr = map.lock()) { // make shared ptr of the map to make checks with
		if (map_coords.X >= 0 && map_coords.X < map_ptr->get_map_width() && map_coords.Y >= 0 && map_coords.Y < map_ptr->get_map_width()) {
			map_coords_ = map_coords;
		}
		else {
			// if not then throw error and exit
			exit(1);
		}
	}
}
/*
TerrainTile::TerrainTile(COORD const map_coords) : GameObject() {
	map_coords_ = map_coords;
}
*/

TerrainTile::~TerrainTile() {};
