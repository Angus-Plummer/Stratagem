#include "stdafx.h"
#include "tile_mountain.h"


MountainTile::MountainTile(GameInstance* game, Map* map, COORD const map_coords) : Tile(game, map, map_coords) {
	// member data for tiles
	type_ = "mountains";
	//marker_[0] = '/'; marker_[1] = char(238); marker_[2] = '\\';
	marker_= char(176);// char(237);
	default_colour_scheme_ = 128;
	highlighted_colour_scheme_ = 143;
	move_cost_ = 3;
	def_modifier_ = 3;
	atk_modifier_ = -2;
}


MountainTile::~MountainTile()
{
}
