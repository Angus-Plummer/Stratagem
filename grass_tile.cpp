#include "stdafx.h"
#include "grass_tile.h"


GrassTile::GrassTile(std::weak_ptr<TileMap> map, COORD const map_coords) : TerrainTile(map, map_coords) {
	// member data for tiles
	type_ = "plains";
	marker_ = char(176);
	default_colour_scheme_ = 106;
	selected_colour_scheme_ = 234;
	move_cost_ = 1;
	def_modifier_ = 1;
	atk_modifier_ = 1;
}


GrassTile::~GrassTile(){
}
