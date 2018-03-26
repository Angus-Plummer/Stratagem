#include "stdafx.h"
#include "tile_map.h"
#include "console_management.h"

// empty map with default map settings
TileMap::TileMap() : map_width_(10), map_height_(10), set_up_width_(3), tile_width_(6), tile_height_(3) {
	// fills the map with grass tiles
	for (int i = 0; i < map_width_*map_height_; i++) {
		std::shared_ptr<TerrainTile> new_tile(new GrassTile());
		map_.push_back(std::move(new_tile));
	}
}
// load map from 2d array (1=grass, 2=forest, 3=mountain, 4=water)
TileMap::TileMap(std::vector<std::vector<int>> &map) : map_width_(map[0].size()), map_height_(map.size()), set_up_width_(3), tile_width_(6), tile_height_(3) {
	// go through the 2d map vector and add the corresponding tile type to the map vector
	for (int j = 0; j < map_height_; j++) {
		for (int i = 0; i < map_width_; i++) { 
			// 1 -> grass
			if (map[i][j] == 1) {
				std::shared_ptr<TerrainTile> new_tile(new GrassTile());
				map_.push_back(std::move(new_tile));
			}
			// 2 -> forest
			else if (map[i][j] == 2){
				std::shared_ptr<TerrainTile> new_tile(new ForestTile());
				map_.push_back(std::move(new_tile));
			}
			// 3 -> mountian
			else if (map[i][j] == 3) {
				std::shared_ptr<TerrainTile> new_tile(new MountainTile());
				map_.push_back(std::move(new_tile));
			}
			// 4 -> water
			else if (map[i][j] == 4) {
				std::shared_ptr<TerrainTile> new_tile(new WaterTile());
				map_.push_back(std::move(new_tile));
			}
			// if any value is not 1,2,3,4 then print error message and abort
			else {
				std::cout << "Error: Map contains invalid tile IDs." << std::endl;
				exit(1);
			}
		}
	}
}

// dtor. no need to clear map_ as smart pointers handle it
TileMap::~TileMap(){}

// Return position in array of element (m,n)
int TileMap::index(int m, int n) const {
	// if within bounds then return index otherwise print error and exit
	if (m>=0 && m < map_height_ && n>=0 && n < map_width_) return n + m*map_width_;
	else {
		std::cout << "Error: out of range (" << m << ", " << n << ")" << std::endl;
		exit(1);
	}
}

// prints map out to the screen
void TileMap::print() const {
	int original_colour_scheme = get_colour_scheme(); // save original colour scheme to set back later
	int tile_height = 3; // width of tile in console cell units
	int tile_width = 6; // height of tile in console cell units
	// offset from edge of screen to make space for cell labels
	int x_offset = 3; 
	int y_offset = 1;

	// print numbers above top row (in y_offset region)
	for (int map_i = 0; map_i < map_width_; map_i++) {
		set_console_cursor_position(map_i*tile_width + (int)floor(tile_width / 2) + x_offset, (int)floor(y_offset / 2)); // places cursor centrally along width of the tile
		std::cout << map_i;
	}
	for (int map_j = 0; map_j < map_height_; map_j++) {
		set_console_cursor_position((int)floor(x_offset / 2), map_j*tile_height + (int)floor(tile_height / 2) + y_offset); // places cursor centrally along the height of the tile and the width of the x_offset
		std::cout << char(65 + map_j); // char(65) is ASCII code for A
	}
	// print map
	// iterate over the map tiles
	for (int map_j = 0; map_j < map_height_; map_j++) {
		for (int map_i = 0; map_i < map_width_; map_i++) {
			// set colour scheme for tyle type
			set_colour_scheme(map_[index(map_i, map_j)]->get_colour_scheme());
			// iterate over console cells for tile width and height
			for (int tile_j = 0; tile_j < tile_height; tile_j++) {
				for (int tile_i = 0; tile_i < tile_width; tile_i++) {
					// set console cursor position to given tile component
					set_console_cursor_position(x_offset + map_i*tile_width + tile_i, y_offset + map_j*tile_height + tile_j);
					// output the tile marker
					std::cout << map_[index(map_i, map_j)]->get_marker();

				}
			}
		}
	}
	// set colour scheme back to original
	set_colour_scheme(original_colour_scheme);
}

std::shared_ptr<TerrainTile> TileMap::get_tile(int const i, int const j) const {
	return map_[index(i, j)];
}
