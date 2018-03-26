#include "stdafx.h"
#include "tile_map.h"
#include "console_management.h"

// empty map
TileMap::TileMap() : width_(10), height_(10), set_up_width_(3) {
	// fills the map with grass tiles
	for (int i = 0; i < width_*height_; i++) {
		std::shared_ptr<TerrainTile> new_tile(new GrassTile());
		map_.push_back(std::move(new_tile));
	}
}
// load map from 2d array (1=grass, 2=forest, 3=mountain, 4=water)
TileMap::TileMap(const int(&map)[10][10]) : width_(10), height_(10), set_up_width_(10) {
	// go through the 2d map array and add the corresponding tile type to the map vector
	for (int i = 0; i < width_; i++) {
		for (int j = 0; j < height_; j++) { 
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
	if (m>=0 && m < height_ && n>=0 && n < width_) return n + m*width_;
	else {
		std::cout << "Error: out of range (" << m << ", " << n << ")" << std::endl;
		exit(1);
	}
}

// prints map out to the screen
void TileMap::print() const {
	// print numbers along the the top for indentifying columns
	for (int i = 0; i < width_; i++) {
		std::cout << "   " << i;
	}
	std::cout << std::endl;
	// top row
	// top left corner ┌
	std::cout << " " << char(218); 
	// print ───┬ for each tile in width except for final tile
	for (int i = 0; i < width_ - 1; i++) { std::cout << char(196) << char(196) << char(196) << char(194);}
	// print ───┐ for end tile
	std::cout << char(196) << char(196) << char(196) << char(191) << std::endl;

	// go through map tile by tile
	for (int i = 0; i < width_; i++){
		for (int j = 0; j < height_; j++){
			// first is left tile edge: │
			if (j == 0) {
				std::cout << "" << char(65 + i) << char(179);
			}
			// set the appropriate colour scheme, save current to restore after
			int old_scheme(get_colour_scheme());
			set_colour_scheme(map_[index(i, j)]->get_colour_scheme());
			// print out the three components of the tile marker
			std::cout << map_[index(i, j)]->get_marker(0) << map_[index(i,j)]->get_marker(1) << map_[index(i, j)]->get_marker(2);
			set_colour_scheme(old_scheme);

			// last is right line edge: 
			std::cout << char(179);
		}
		std::cout << std::endl;
		// middle rows
		if (i < width_ - 1) {
			// left edge ├
			std::cout << " " << char(195);
			// print ───┼ for each tile in width except for final tile
			for (int i = 0; i < width_ - 1; i++) { std::cout << char(196) << char(196) << char(196) << char(197); }
			// print ───┤ for end tile
			std::cout << char(196) << char(196) << char(196) << char(180) << std::endl;
		}
	}
	// bottom row
	// bottom left corner └
	std::cout << " " << char(192);
	// print ───┴ for each tile in width except for final tile
	for (int i = 0; i < width_ - 1; i++) { std::cout << char(196) << char(196) << char(196) << char(193); }
	// print ───┘ for end tile
	std::cout << char(196) << char(196) << char(196) << char(217) << std::endl;
}

std::shared_ptr<TerrainTile> TileMap::get_tile(int const i, int const j) const {
	return map_[index(i, j)];
}
