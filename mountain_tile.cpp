#include "stdafx.h"
#include "mountain_tile.h"


MountainTile::MountainTile(std::weak_ptr<TileMap> map, COORD const map_coords) : TerrainTile(map, map_coords) {
	// member data for tiles
	type_ = "mountains";
	//marker_[0] = '/'; marker_[1] = char(238); marker_[2] = '\\';
	marker_= char(176);// char(237);
	default_colour_scheme_ = 128;
	selected_colour_scheme_ = 127;
	move_cost_ = 3;
	def_modifier_ = 2;
	atk_modifier_ = 0.8;
}


MountainTile::~MountainTile()
{
}
