#include "stdafx.h"
#include "tile_water.h"

WaterTile::WaterTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
	type_ = "water";
	tile_marker_ = char(176);
	default_colour_scheme_ = 27;
	highlighted_colour_scheme_ = 28;
	move_cost_ = 2;
	def_modifier_ = -3;
	atk_modifier_ = -5;
}


WaterTile::~WaterTile(){
}
