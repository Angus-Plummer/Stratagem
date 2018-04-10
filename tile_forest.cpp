#include "stdafx.h"
#include "tile_forest.h"

ForestTile::ForestTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
// member data for tiles
type_ = "forest";
tile_marker_ = char(237);// char(237); // char(190)
default_colour_scheme_ = ColourScheme(DARK_GREEN, BROWN);
highlighted_colour_scheme_ = ColourScheme(DARK_GREEN, RED);
move_cost_ = 2;
def_modifier_ = 1;
atk_modifier_ = 0;
}


ForestTile::~ForestTile()
{
}
