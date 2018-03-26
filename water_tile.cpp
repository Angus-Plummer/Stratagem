#include "stdafx.h"
#include "water_tile.h"


WaterTile::WaterTile(){
	type_ = "water";
	marker_ = char(176);
	colour_scheme_ = 27;
	move_cost_ = 2;
	def_modifier_ = 0.5;
	atk_modifier_ = 0.5;
}


WaterTile::~WaterTile(){
}
