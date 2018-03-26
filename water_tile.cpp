#include "stdafx.h"
#include "water_tile.h"


WaterTile::WaterTile(){
	type_ = "water";
	marker_[0] = char(176); marker_[1] = char(176); marker_[2] = char(176);
	colour_scheme_ = 27;
	move_cost_ = 2;
	def_modifier_ = 0.5;
	atk_modifier_ = 0.5;
}


WaterTile::~WaterTile(){
}
