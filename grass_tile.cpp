#include "stdafx.h"
#include "grass_tile.h"


GrassTile::GrassTile(){
	// member data for tiles
	type_ = "plains";
	marker_[0] = char(176); marker_[1] = char(176); marker_[2] = char(176);
	colour_scheme_ = 106;
	move_cost_ = 1;
	def_modifier_ = 1;
	atk_modifier_ = 1;
}


GrassTile::~GrassTile(){
}
