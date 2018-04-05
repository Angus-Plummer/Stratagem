#include "stdafx.h"
#include "tile_grass.h"


GrassTile::GrassTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
	// member data for tiles
	type_ = "plains";
	tile_marker_ = char(176);
	default_colour_scheme_ = 106;
	highlighted_colour_scheme_ = 108; 
	move_cost_ = 1;
	def_modifier_ = 0;
	atk_modifier_ = 0;
}


GrassTile::~GrassTile(){
}
