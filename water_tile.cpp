#include "stdafx.h"
#include "water_tile.h"


WaterTile::WaterTile(std::weak_ptr<TileMap> map, COORD const map_coords) : TerrainTile(map, map_coords) {
	type_ = "water";
	marker_ = char(176);
	default_colour_scheme_ = 27;
	selected_colour_scheme_ = 191;
	move_cost_ = 2;
	def_modifier_ = 0.5;
	atk_modifier_ = 0.5;
}


WaterTile::~WaterTile(){
}
