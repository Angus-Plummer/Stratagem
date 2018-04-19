#include "stdafx.h"
#include "tile_grass.h"


GrassTile::GrassTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
	// tile output info
	type_ = "plains";
	tile_marker_ = char(176); // lattice of dots, make background brown and text green and looks like plains
	default_colour_scheme_ = ColourScheme(BROWN, GREEN);
	highlighted_colour_scheme_ = ColourScheme(BROWN, RED);
	// stats
	move_cost_ = 1;
	def_modifier_ = 0;
	atk_modifier_ = 0;
}


GrassTile::~GrassTile(){
}
