#include "stdafx.h"
#include "tile.h"
#include "screen.h"
#include "map.h"

Tile::Tile(Map* map, COORD const map_coords) : GameObject(map){
	// check if the coordinates are valid on the map
	if (map_coords.X >= 0 && map_coords.X < map->get_map_width() && map_coords.Y >= 0 && map_coords.Y < map->get_map_width()) {
		map_coords_ = map_coords;
	}
	else {
		// if not then throw error and exit
		exit(1);
	}
}

Tile::~Tile() {};

// render the tile
void Tile::Render(Screen& display) const {
	int original_colour_scheme = display.get_colour_scheme(); // save original colour scheme to set back later
	// set colour scheme for tyle type
	display.set_colour_scheme(get_colour_scheme());
	// iterate over console cells for tile width and height
	for (int tile_j = 0; tile_j < display.get_tile_height(); tile_j++) {
		for (int tile_i = 0; tile_i < display.get_tile_width(); tile_i++) {
			// set console cursor position to given tile component
			display.GoTo(display.get_map_x_offset() + map_coords_.X * display.get_tile_width() + tile_i, display.get_map_y_offset() + map_coords_.Y * display.get_tile_height() + tile_j);
			// output the tile marker
			std::cout << marker_;
		}
	}
	// revert to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}
