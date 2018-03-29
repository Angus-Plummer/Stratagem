#include "stdafx.h"
#include "tile_map.h"

// empty map with default map settings
TileMap::TileMap() : map_width_(10), map_height_(10), set_up_width_(3) {
	// size vector of map tiles to match 10x10
	map_.resize(map_width_);
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map_height_);
	}
}
// load map from 2d array (1=grass, 2=forest, 3=mountain, 4=water)
TileMap::TileMap(int const width, int const height, int const set_up_width) : map_width_(width), map_height_(height), set_up_width_(set_up_width) {
	// size vector of map tiles to match input
	map_.resize(map_width_);
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map_height_);
	}
}

// dtor. no need to clear map_ as smart pointers handle it
TileMap::~TileMap(){}

void TileMap::LoadMap(std::vector<std::vector<int>> const &map) {
	std::weak_ptr<TileMap> this_map(shared_from_this());
	for (int row = 0; row < map_height_; row++) {
		for (int column = 0; column < map_width_; column++) {
			COORD coordinate = { column, row }; // output map is (x, y) i.e (column, row) but input is (row, column)
			// 1 -> grass
			if (map[row][column] == 1) {
				std::shared_ptr<TerrainTile> new_tile(new GrassTile(this_map, coordinate));
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// 2 -> forest
			else if (map[row][column] == 2) {
				std::shared_ptr<TerrainTile> new_tile(new ForestTile(this_map, coordinate));
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// 3 -> mountian
			else if (map[row][column] == 3) {
				std::shared_ptr<TerrainTile> new_tile(new MountainTile(this_map, coordinate));
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// 4 -> water
			else if (map[row][column] == 4) {
				std::shared_ptr<TerrainTile> new_tile(new WaterTile(this_map, coordinate));
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// if any value is not 1,2,3,4 then print error message and abort
			else {
				std::cout << "Error: Map contains invalid tile IDs." << std::endl;
				exit(1);
			}
		}
	}
}

std::shared_ptr<TerrainTile> TileMap::get_tile(int const x, int const y) const {
	return map_[x][y];
}
