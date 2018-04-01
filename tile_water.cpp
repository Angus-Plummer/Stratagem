#include "stdafx.h"
#include "tile_water.h"

#define dark 16;
#define soft 27;
#define bright 144;

WaterTile::WaterTile(Map &map, const COORD &map_coords) : Tile(map, map_coords) {
	type_ = "water";
	marker_ = char(176);
	default_colour_scheme_ = 27;
	highlighted_colour_scheme_ = dark;
	move_cost_ = 2;
	def_modifier_ = -3;
	atk_modifier_ = -5;
}


WaterTile::~WaterTile(){
}
