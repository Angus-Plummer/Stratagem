#include "stdafx.h"
#include "map.h"
#include "screen.h"

// empty map with default map settings
Map::Map() : map_width_(10), map_height_(10), set_up_width_(3) {
	// size vector of map tiles to match 10x10
	map_.resize(map_width_);
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map_height_);
	}
}
// load map from 2d array (1=grass, 2=forest, 3=mountain, 4=water)
Map::Map(int const width, int const height, int const set_up_width) : map_width_(width), map_height_(height), set_up_width_(set_up_width) {
	// size vector of map tiles to match input
	map_.resize(map_width_);
	for (auto iterator = map_.begin(); iterator != map_.end(); iterator++) {
		iterator->resize(map_height_);
	}
}

// dtor. need to clear tiles in map when called
Map::~Map(){
	for (auto iter_vec = map_.begin(); iter_vec != map_.end(); iter_vec++) {
		for (auto iter_tile = iter_vec->begin(); iter_tile != iter_vec->end(); iter_tile++) {
			delete (*iter_tile);
		}
		iter_vec->clear();
	}
	map_.clear();
}

void Map::LoadMap(std::vector<std::vector<int>> const &map) {
	for (int row = 0; row < map_height_; row++) {
		for (int column = 0; column < map_width_; column++) {
			COORD coordinate = { column, row }; // output map is (x, y) i.e (column, row) but input is (row, column)
			// 1 -> grass
			if (map[row][column] == 1) {
				Tile* new_tile = new GrassTile(this, coordinate);
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// 2 -> forest
			else if (map[row][column] == 2) {
				Tile* new_tile = new ForestTile(this, coordinate);
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// 3 -> mountian
			else if (map[row][column] == 3) {
				Tile* new_tile = new MountainTile(this, coordinate);
				map_[coordinate.X][coordinate.Y] = (std::move(new_tile));
			}
			// 4 -> water
			else if (map[row][column] == 4) {
				Tile* new_tile = new WaterTile(this, coordinate);
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

// returns the tile at a given coordinate position
Tile* Map::GetTile(COORD const position) const {
	if (position.X >= 0 && position.X < map_width_ && position.Y >= 0 && position.Y < map_height_) {
		return map_[position.X][position.Y];
	}
	else {
		return nullptr;
	}
}

// Renders the map on a screen
void Map::Render(Screen &display) const {
	int tile_width = display.get_tile_width();
	int tile_height = display.get_tile_height();
	int map_x_offset = display.get_map_x_offset();
	int map_y_offset = display.get_map_y_offset();

	// print numbers above top row (in y_offset region)
	for (int map_i = 0; map_i < map_width_; map_i++) {
		display.GoTo(map_i*tile_width + (int)floor(tile_width / 2) + map_x_offset, (int)floor(map_y_offset / 2)); // places cursor centrally along width of the tile
		std::cout << char(65 + map_i); // char(65) is ASCII code for A
	}
	// print alphabetic characters left of first column (in x_offset region)
	for (int map_j = 0; map_j < map_height_; map_j++) {
		display.GoTo((int)floor(map_x_offset / 2), map_j*tile_height + (int)floor(tile_height / 2) + map_y_offset); // places cursor centrally along the height of the tile and the width of the x_offset
		std::cout << map_j;
	}
	// print map
	// iterate over the map tiles
	for (int map_j = 0; map_j < map_height_; map_j++) {
		for (int map_i = 0; map_i < map_width_; map_i++) {
			map_[map_i][map_j]->Render(display);
		}
	}
	// iterate over the units
	for (auto iterator = units_.begin(); iterator != units_.end(); iterator++) {
		(*iterator)->Render(display); // render unit on display
	}
}

// adds a unit to the map. if unit is on tile or tile coordinate is not valid then it does nothing
void Map::AddUnit(Unit* new_unit, COORD const position) {
	// check if coordinate is a valid position on the map
	if (position.X >= 0 && position.X < map_width_ && position.Y >= 0 && position.Y < map_height_) {
		// if there is no unit currently occupying the tile then add the unit
		if (!UnitPresent(position)) {
			new_unit->set_map_coords(position);
			units_.push_back(new_unit);
		} // otherwise throw error and do nothing
	}
}

// returns true if a unit is present on a given tile
bool Map::UnitPresent(COORD const position) const {
	// iterate through the vector of weak pointers to the units
	for (auto iterator = units_.begin(); iterator != units_.end(); iterator++) {
		// if the coordinate of the unit is the same as the input then return true
		if ((*iterator)->get_map_coords().X == position.X && (*iterator)->get_map_coords().Y == position.Y) {
			return true;
		}
	}
	// if gone through vector and no units found at that location then return false
	return false;
}
// returns the unit on a given tile (if there is one, else returns null pointer?)
Unit* Map::GetUnit(COORD const position) const {
	if (UnitPresent(position)) {
		for (auto iterator = units_.begin(); iterator != units_.end(); iterator++) {
			// if the coordinate of the unit is the same as the input then return unit
			if ((*iterator)->get_map_coords().X == position.X && (*iterator)->get_map_coords().Y == position.Y) {
				return (*iterator);
			}
		}
	}
	// if found nothing then return null pointer
	return nullptr;
}

// select an object on the map
GameObject* Map::Select(Screen &display) const {
	// get location of mouse down ({-1,-1} if no mouse down detected)
	COORD event_location = display.MouseDownPosition();
	if (event_location.X != -1 && event_location.Y != -1) {
		COORD map_location;	// corresponding map tile
		// correct event location offset and convert into tile location
		map_location.X = (event_location.X - display.get_map_x_offset()) / display.get_tile_width();
		map_location.Y = (event_location.Y - display.get_map_y_offset()) / display.get_tile_height();
		// if tile is on the map..
		if (map_location.X >= 0 && map_location.X < map_width_ && map_location.Y >= 0 && map_location.Y < map_height_) {
			// if there is a unit on the tile then return the unit
			if (UnitPresent(map_location)) {
				return GetUnit(map_location);
			}
			// otherwise return the tile
			else {
				return GetTile(map_location);
			}
		}
	}
	// if found nothing at location of click then returns a null pointer
	return nullptr;
}

// returns vector of pointers to tile object that are adjacent to the location
std::vector<Tile*> Map::AdjacentTo(COORD const position) const {
	COORD adjacent_position = position;
	std::vector<Tile*> adjacent_tiles;
	// tile to right
	if (position.X + 1 < map_width_) { // check tile not out of bounds
		adjacent_position.X = position.X + 1;
		adjacent_tiles.push_back(GetTile(adjacent_position));
	}
	// tile to left
	if (position.X - 1 >= 0) { // check tile not out of bounds
		adjacent_position.X = position.X - 1;
		adjacent_tiles.push_back(GetTile(adjacent_position));
	}
	adjacent_position = position;
	// tile below
	if (position.Y + 1 < map_height_) { // check tile not out of bounds
		adjacent_position.Y = position.Y + 1;
		adjacent_tiles.push_back(GetTile(adjacent_position));
	}
	// tile above
	if (position.Y - 1 >= 0) { // check tile not out of bounds
		adjacent_position.Y = position.Y - 1;
		adjacent_tiles.push_back(GetTile(adjacent_position));
	}
	return adjacent_tiles;
}

// returns a vector of pointers to the tiles that are reachable by a given unit (ignores the tile it is currently on)
std::vector<Tile*> Map::ReachableTiles(Unit* unit) const{
	std::vector<Tile*> can_reach; // tiles that the unit can reach
	std::vector<MoveSequence> closed; // tiles that have already been inspected
	std::vector<MoveSequence> open; // tiles that have yet to be inspected
	int max_distance = unit->get_move_distance();

	// start with the tile the unit is currently on, add to the vector of open tiles
	MoveSequence initial_tile(unit->GetTile()); // maybe need to make a copy assignment operator to use this
	open.push_back(initial_tile);
	can_reach.push_back(initial_tile.get_tile());

	std::vector<Tile*> adjacent_tiles;

	// while there are any tiles left to inspect
	while (open.size() > 0) {
		// poitner to the current tile to be analysed is the one which has the lowest cost to reach so far
		auto current_iter = std::min_element(open.begin(), open.end());
		// add it to the closed vector
		closed.push_back(*current_iter);
		// set pointer to the target MoveSequence object in the closed set. The set doesnt change when looping through neighbours so will stay valid
		MoveSequence* current = &(*std::find(closed.begin(), closed.end(), *current_iter));
		// remove it from the open vector
		open.erase(current_iter);
		// clear the vector of adjacent tiles and fill for the current tile
		adjacent_tiles.clear();
		adjacent_tiles = AdjacentTo(current->get_tile()->get_map_coords());
		// go through this vector
		for (auto iter = adjacent_tiles.begin(); iter != adjacent_tiles.end(); iter++) {
			// make a MoveSequence object from this adjacent tile and make its parent the currently inspected
			MoveSequence adjacent(*iter);
			adjacent.set_parent(*current);
			// see if this tile is already in the open and closed sets
			bool in_open_set = std::find(open.begin(), open.end(), adjacent) != open.end();
			bool in_closed_set = std::find(closed.begin(), closed.end(), adjacent) != closed.end();
			// if neither the open or closed sets contain this object already then proceed
			if (!in_closed_set) {
				// compute score
				// if the tile can be traversed by the unit then proceed
				if (unit->CanTraverse(adjacent.get_tile())) {
					if (!in_open_set) { // if not in open set
						// if cost to reach it is less than max possible add it to the reachable tiles and the open set
						if (adjacent.get_cost() <= max_distance) {
							if (UnitPresent(adjacent.get_tile()->get_map_coords())) {
								if (unit->get_team() == GetUnit(adjacent.get_tile()->get_map_coords())->get_team()) {
									open.push_back(adjacent);
								}
							}
							else {
								open.push_back(adjacent);
								can_reach.push_back(adjacent.get_tile());
							}
						}
					}
					else { // (in open set already)
						// pointer to object already in open set
						MoveSequence* in_set = &*std::find(open.begin(), open.end(), adjacent);
						// if adjacent tile costs less to reach from parent as currently inspected tile then update the parent of tile already in set
						if (adjacent < *in_set) {
							in_set->set_parent(*current);
						}
					}
				}
			}
		}
	}
	return can_reach;
}

