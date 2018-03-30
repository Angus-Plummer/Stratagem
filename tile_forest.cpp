#include "stdafx.h"
#include "forest_tile.h"


ForestTile::ForestTile(std::weak_ptr<TileMap> map, COORD const map_coords) : TerrainTile(map, map_coords) {
// member data for tiles
type_ = "forest";
marker_ = char(176);// char(237);
default_colour_scheme_ = 32;
selected_colour_scheme_ = 160;
def_modifier_ = 1.5;
atk_modifier_ = 1;
}


ForestTile::~ForestTile()
{
}
