#include "stdafx.h"
#include "mountain_tile.h"


MountainTile::MountainTile(){
	// member data for tiles
	type_ = "mountain";
	marker_[0] = '/'; marker_[1] = char(238); marker_[2] = '\\';
	colour_scheme_ = 112;
	move_cost_ = 3;
	def_modifier_ = 2;
	atk_modifier_ = 0.8;
}


MountainTile::~MountainTile()
{
}
