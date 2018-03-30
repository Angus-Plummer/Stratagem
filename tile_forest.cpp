#include "stdafx.h"
#include "tile_forest.h"


ForestTile::ForestTile(GameInstance* game, Map* map, COORD const map_coords) : Tile(game, map, map_coords) {
// member data for tiles
type_ = "forest";
marker_ = char(176);// char(237);
default_colour_scheme_ = 32;
highlighted_colour_scheme_ = 47;
move_cost_ = 2;
def_modifier_ = 1;
atk_modifier_ = 0;
}


ForestTile::~ForestTile()
{
}
