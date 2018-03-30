#include "stdafx.h"
#include "tile_grass.h"


GrassTile::GrassTile(GameInstance* game, Map* map, COORD const map_coords) : Tile(game, map, map_coords) {
	// member data for tiles
	type_ = "plains";
	marker_ = char(176);
	default_colour_scheme_ = 98;
	highlighted_colour_scheme_ = 106;
	move_cost_ = 1;
	def_modifier_ = 0;
	atk_modifier_ = 0;
}


GrassTile::~GrassTile(){
}
