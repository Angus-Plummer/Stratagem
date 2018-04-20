#include "stdafx.h"
#include "tile_water.h"

// ctor
WaterTile::WaterTile(Map &map, const Coord &map_coords) : Tile(map, map_coords) {
	// tile output info
	type_ = "water";
	tile_marker_ = char(176); // lattice of dots, light blue on dark blue looks like refelcetions/ highlights on water
	default_colour_scheme_ = ColourScheme(DARK_BLUE, LIGHT_BLUE);
	highlighted_colour_scheme_ = ColourScheme(DARK_BLUE, RED);
	// stats
	move_cost_ = 2;
	def_modifier_ = -3;
	atk_modifier_ = -5;
}

// dtor
WaterTile::~WaterTile() {}

// helper function returns a pointer to a copy of this object
WaterTile* WaterTile::clone_impl() const { return new WaterTile(*this); }

