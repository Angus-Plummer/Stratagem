#include "stdafx.h"
#include "tile_grass.h"

#define dark 98;
#define soft 106;
#define bright 224;

GrassTile::GrassTile(Map &map, const COORD &map_coords) : Tile(map, map_coords) {
	// member data for tiles
	type_ = "plains";
	marker_ = char(176);
	default_colour_scheme_ = 106;
	highlighted_colour_scheme_ = dark; 
	move_cost_ = 1;
	def_modifier_ = 0;
	atk_modifier_ = 0;
}


GrassTile::~GrassTile(){
}
