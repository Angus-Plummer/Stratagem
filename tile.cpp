#include "stdafx.h"
#include "tile.h"

#include "game_instance.h"
#include "screen.h"
#include "map.h"

Tile::Tile(Map &map, const Coord &map_coords) : GameObject(), parent_map_(&map), highlighted_(false){
	// check if the coordinates are valid on the map
	if (map_coords.x >= 0 && map_coords.x < map.get_map_width() && map_coords.y >= 0 && map_coords.y < map.get_map_width()) {
		map_coords_ = map_coords;
	}
	else {
		// if not then throw error and exit
		exit(1);
	}
}

Tile::~Tile() {};

// get the colour scheme
int Tile::get_colour_scheme() const {
	if (highlighted_) {
		return highlighted_colour_scheme_;
	}
	else {
		return default_colour_scheme_;
	}
}

// set and get highlighted
void Tile::set_highlighted(bool const &highlighted) {
	highlighted_ = highlighted;
}

// function to check if two tiles are adjacent
bool Tile::AdjacencyTest(const Tile *test_tile) const {
	std::vector<Tile*> adjacent_tiles = GameInstance::instance().get_map().AdjacentTo(this);
	return std::find(adjacent_tiles.begin(), adjacent_tiles.end(), test_tile) != adjacent_tiles.end();
}

// render the tile
void Tile::Render() const {
	Screen display = GameInstance::instance().get_display();
	int original_colour_scheme = display.get_colour_scheme(); // save original colour scheme to set back later
	// set colour scheme for tyle type
	display.set_colour_scheme(get_colour_scheme());
	// iterate over console cells for tile width and height
	for (int tile_j = 0; tile_j < display.get_tile_height(); tile_j++) {
		for (int tile_i = 0; tile_i < display.get_tile_width(); tile_i++) {
			Coord tile_component = { display.get_map_x_offset() + map_coords_.x * display.get_tile_width() + tile_i, display.get_map_y_offset() + map_coords_.y * display.get_tile_height() + tile_j };
			// set console cursor position to given tile component
			display.GoTo(tile_component);
			// output the tile marker
			std::cout << tile_marker_;
		}
	}
	// revert to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}
