#include "stdafx.h"
#include "map.h"
#include "game_manager.h"
#include "game_instance.h"
#include "window.h"
#include "unit.h"
#include "tile_grass.h"
#include "tile_water.h"
#include "tile_mountain.h"
#include "tile_forest.h"

// ---------- ctors, dtors, assignment overloading ---------- //

// empty map with default map settings
Map::Map() : map_width_(10), map_height_(10), set_up_width_(3) {
	// size vector of map tiles to match 10x10
	map_.resize(map_width_);
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map_height_);
	}
}
// load map from 2d array (1=grass, 2=forest, 3=mountain, 4=water)
Map::Map( const int &width, const int &height, const int &set_up_width) : map_width_(width), map_height_(height), set_up_width_(set_up_width) {
	// size vector of map tiles to match input
	map_.resize(map_width_);
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map_height_);
	}
}

// copy ctor (does not make new units)
Map::Map(const Map &map) : map_width_(map.map_width_), map_height_(map.map_height_), set_up_width_(map.set_up_width_){
	// size vector of map tiles to match input
	map_.resize(map_width_);
	for (auto col_iter = map_.begin(); col_iter != map_.end(); col_iter++) {
		col_iter->resize(map_height_);
	}
	// copy the tiles over from the input map (makes new tile objects)
	for (int col = 0; col < map_width_; col++) {
		for (int row = 0; row < map_height_; row++) {
			if (map.map_[col][row]) {
				map_[col][row] = map.map_[col][row]->clone();
			}
		}
	}
	// copy the units over from the input map ( does not make new unit objects as the map does not have ownership of the units)
	for (auto unit_iter = map.units_.begin(); unit_iter != map.units_.end(); unit_iter++) {
		units_.push_back(*unit_iter);
	}
}

// move ctor
Map::Map(Map &&map) : Map() {
	std::swap(map_width_, map.map_width_);
	std::swap(map_height_, map.map_height_);
	std::swap(set_up_width_, map.set_up_width_);
	std::swap(map_, map.map_);
	std::swap(units_, map.units_);
}

// dtor. need to clear tiles in map when called
Map::~Map() {
	Clear();
}

// copy assigment
Map& Map::operator=(const Map &map) {
	// clear the current 2d tile vector
	for (auto col_iter = map_.begin(); col_iter != map_.end(); col_iter++) {
		col_iter->clear();
	}
	map_.clear();

	// size vector of map tiles to match input
	map_.resize(map_width_);
	for (auto col_iter = map_.begin(); col_iter != map_.end(); col_iter++) {
		col_iter->resize(map_height_);
	}
	// copy the tiles over from the input map (makes new tile objects)
	for (int col = 0; col < map_width_; col++) {
		for (int row = 0; row < map_height_; row++) {
			if (map.map_[col][row]) {
				map_[col][row] = map.map_[col][row]->clone();
			}
		}
	}
	// clear the current vector of units and copy the units over from the input map ( does not make new unit objects as the map does not have ownership of the units)
	units_.clear();
	for (auto unit_iter = map.units_.begin(); unit_iter != map.units_.end(); unit_iter++) {
		units_.push_back(*unit_iter);
	}
	return *this;
}
// move assigment
Map& Map::operator=(Map &&map) {
	std::swap(map_width_, map.map_width_);
	std::swap(map_height_, map.map_height_);
	std::swap(set_up_width_, map.set_up_width_);
	std::swap(map_, map.map_);
	std::swap(units_, map.units_);

	return *this;
}

// -------- accessors --------- //

// map dimensions
const int& Map::get_map_height() const { return map_height_; }
const int& Map::get_map_width() const { return map_width_; }

// set up width
const int& Map::get_set_up_width() const { return set_up_width_; }

// uunits on the map
const std::vector<Unit*> Map::get_units() const { return units_; }

// --------- public functions ---------- //

// clears the map. (deletes all tiles and resets 2d map vector)
void Map::Clear() {
	for (auto iter_vec = map_.begin(); iter_vec != map_.end(); iter_vec++) {
		iter_vec->clear();
	}
	map_.clear();
}

// clears any current map stored and loads a new one
void Map::LoadMap(const std::vector<std::vector<int>> &map) {
	// clear map
	Clear();
	// resize to new map dimensions
	map_.resize(map[0].size()); // map width (input map is row x column)
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map.size()); // map height
	}
	// set the tiles cell by cell. note that the input maps are the transpose of desired maps
	for (int row = 0; row < map_height_; row++) {
		for (int column = 0; column < map_width_; column++) {
			Coord coordinate = { column, row }; // output map is (x, y) i.e (column, row) but input is (row, column)
			// 1 -> grass
			if (map[row][column] == 1) {
				std::unique_ptr<Tile> new_tile(new GrassTile(*this, coordinate));
				map_[coordinate.x][coordinate.y] = (std::move(new_tile));
			}
			// 2 -> forest
			else if (map[row][column] == 2) {
				std::unique_ptr<Tile> new_tile(new ForestTile(*this, coordinate));
				map_[coordinate.x][coordinate.y] = (std::move(new_tile));
			}
			// 3 -> mountian
			else if (map[row][column] == 3) {
				std::unique_ptr<Tile> new_tile(new MountainTile(*this, coordinate));
				map_[coordinate.x][coordinate.y] = (std::move(new_tile));
			}
			// 4 -> water
			else if (map[row][column] == 4) {
				std::unique_ptr<Tile> new_tile(new WaterTile(*this, coordinate));
				map_[coordinate.x][coordinate.y] = (std::move(new_tile));
			}
			// if any value is not 1,2,3,4 then print error message and abort
			else {
				std::cout << "Error: Map contains invalid tile IDs." << std::endl;
				exit(1);
			}
		}
	}
}

// returns the tile at a given coordinate position
Tile* Map::GetTile(const Coord &position) const {
	// if position is on the map then return the tile
	if (position.x >= 0 && position.x < map_width_ && position.y >= 0 && position.y < map_height_) {
		return &*map_[position.x][position.y];
	}
	// otherwise return a null pointer
	else {
		return nullptr;
	}
}

// returns the tile at a console coordinate
Tile* Map::GetTileFromConsoleCoord(const Coord &position) const {
	Window display = GameManager::game().get_display();
	Coord map_location;	// corresponding map tile
	// correct for map offset and convert into tile location
	map_location.x = (position.x - display.get_map_x_offset()) / display.get_tile_width();
	map_location.y = (position.y - display.get_map_y_offset()) / display.get_tile_height();
	// if tile is on the map..
	if (map_location.x >= 0 && map_location.x < map_width_ && map_location.y >= 0 && map_location.y < map_height_) {
		// return the tile
		return GetTile(map_location);
	}
	// if found nothing at location then returns a null pointer
	return nullptr;
}

// adds a unit to the map. if unit is on tile or tile coordinate is not valid then it does nothing
void Map::AddUnit(Unit* new_unit, const Coord &position) {
	// check if coordinate is a valid position on the map
	if (position.x >= 0 && position.x < map_width_ && position.y >= 0 && position.y < map_height_) {
		// if there is no unit currently occupying the tile then add the unit
		if (!UnitPresent(position)) {
			new_unit->set_map_coords(position);
			units_.push_back(new_unit);
		} // otherwise throw error and do nothing
	}
}

// remove a unit from the map
void Map::RemoveUnit(Unit *unit) {
	// use the Erase-Remove idiom to delete the unit
	units_.erase(std::remove(units_.begin(), units_.end(), unit), units_.end());
	// render the tile where the unit was to update it
	Render(unit->get_map_coords());
}

// returns true if a unit is present on a given tile
const bool Map::UnitPresent(const Coord &position) const {
	// iterate through the vector of weak pointers to the units
	for (auto iterator = units_.begin(); iterator != units_.end(); iterator++) {
		// if the coordinate of the unit is the same as the input then return true
		if ((*iterator)->get_map_coords() == position) {
			return true;
		}
	}
	// if gone through vector and no units found at that location then return false
	return false;
}

// returns the unit on a given tile (if there is one, else returns null pointer?)
Unit* Map::GetUnit(const Coord &position) const {
	if (UnitPresent(position)) {
		for (auto iterator = units_.begin(); iterator != units_.end(); iterator++) {
			// if the coordinate of the unit is the same as the input then return unit
			if ((*iterator)->get_map_coords() == position) {
				return (*iterator);
			}
		}
	}
	// if found nothing then return null pointer
	return nullptr;
}

// returns vector of pointers to tiles that are adjacent to the location
std::vector<Tile*> Map::AdjacentTo(const Tile *tile) const {
	
	Coord adjacent_position = tile->get_map_coords(); // Coord to hold coordinate of the adjacent tiles
	std::vector<Tile*> adjacent_tiles; // vector of valid adjacent tiles
	
	// each position we check if the target coordinate is valid. If so then change adjacent_coordinate, 
	// add the tile at that coordinate and then set adjacent coordinate back to original value
	
	// tile to right
	if (adjacent_position.x + 1 < map_width_) { 
		adjacent_tiles.push_back(GetTile(adjacent_position + Coord{ 1, 0 }));
	}
	// tile to left
	if (adjacent_position.x - 1 >= 0) { 
		adjacent_tiles.push_back(GetTile(adjacent_position + Coord{ -1, 0 }));
	}
	// tile below
	if (adjacent_position.y + 1 < map_height_) {
		adjacent_tiles.push_back(GetTile(adjacent_position + Coord{ 0, 1 }));
	}
	// tile above
	if (adjacent_position.y - 1 >= 0) {
		adjacent_tiles.push_back(GetTile(adjacent_position + Coord{ 0, -1 }));
	}
	return adjacent_tiles;
}

// Renders the map on a the console
void Map::Render() const {
	Window display = GameManager::game().get_display();
	int tile_width = display.get_tile_width();
	int tile_height = display.get_tile_height();
	int map_x_offset = display.get_map_x_offset();
	int map_y_offset = display.get_map_y_offset();

	// print numbers above top row (in y_offset region)
	for (int map_i = 0; map_i < map_width_; map_i++) {
		display.GoTo({ map_i*tile_width + (int)floor(tile_width / 2) + map_x_offset, (int)floor(map_y_offset / 2) }); // places cursor centrally along width of the tile
		std::cout << char(65 + map_i); // char(65) is ASCII code for A
	}
	// print alphabetic characters left of first column (in x_offset region)
	for (int map_j = 0; map_j < map_height_; map_j++) {
		display.GoTo({ (int)floor(map_x_offset / 2), map_j*tile_height + (int)floor(tile_height / 2) + map_y_offset }); // places cursor centrally along the height of the tile and the width of the x_offset
		std::cout << map_j;
	}
	// print map
	// iterate over the map tiles
	for (int map_j = 0; map_j < map_height_; map_j++) {
		for (int map_i = 0; map_i < map_width_; map_i++) {
			Render(Coord{ map_i, map_j }); // will also render the unit on the tile if there is one
		}
	}
}

// Renders a specific tile the unit on it if there is one
void Map::Render(Coord coord) const {
	// check coord corresponds to a map coordinate
	if (coord.x >= 0 && coord.x < map_width_ && coord.y >= 0 && coord.x < map_height_) {
		// if the coordinate is on the map then render the corresponding tile
		GetTile(coord)->Render();
		// if there is a unit present on the tile then render that too.
		if (UnitPresent(coord)) {
			GetUnit(coord)->Render();
		}
	}
}

// unhiglights all tiles on the console
void Map::ResetTiles() const {
	for (auto iter_vec = map_.begin(); iter_vec != map_.end(); iter_vec++) {
		for (auto iter_tile = iter_vec->begin(); iter_tile != iter_vec->end(); iter_tile++) {
			(*iter_tile)->set_highlighted(false);
		}
	}
}
