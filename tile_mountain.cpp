#include "stdafx.h"
#include "tile_mountain.h"

MountainTile::MountainTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
	// member data for tiles
	type_ = "mountains";
	tile_marker_ = char(94);// char(237); //char(94)
	default_colour_scheme_ = ColourScheme(DARK_GREY, LIGHT_GREY);
	highlighted_colour_scheme_ = ColourScheme(DARK_GREY, RED);
	move_cost_ = 3;
	def_modifier_ = 3;
	atk_modifier_ = -1;
}


MountainTile::~MountainTile()
{
}
