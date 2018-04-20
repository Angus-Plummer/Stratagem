#include "stdafx.h"
#include "tile.h"
#include "game_manager.h"
#include "game_instance.h"
#include "window.h"
#include "map.h"

// ---------- ctor and dtor ---------- //

// ctor
Tile::Tile(Map &map, const Coord &map_coords) : GameObject(), parent_map_(&map), highlighted_(false){
	// check if the coordinates are valid on the map
	assert(map_coords.x >= 0 && map_coords.x < map.get_map_width() && map_coords.y >= 0 && map_coords.y < map.get_map_width());
	map_coords_ = map_coords;
}

// dtor
Tile::~Tile() {};

// --------- accessors and mutators --------- //

// movement cost to enter the tile
const int& Tile::get_move_cost() const { return move_cost_; }

// attack and defense modifiers
const int& Tile::get_def_modifier() const { return def_modifier_; }
const int& Tile::get_atk_modifier() const { return atk_modifier_; }

// get and set the highlighted state of the tile
const bool& Tile::get_highlighted() const { return highlighted_; }
void Tile::set_highlighted(const bool &highlighted) { highlighted_ = highlighted; }

// ---------- public functions ---------- //

// get a unique pointer to a copy of the tile
std::unique_ptr<Tile> Tile::clone() const { return std::unique_ptr<Tile>(clone_impl()); }

// get the colour scheme
const ColourScheme& Tile::get_colour_scheme() const {
	if (highlighted_) {
		return highlighted_colour_scheme_;
	}
	else {
		return default_colour_scheme_;
	}
}

// function to check if two tiles are adjacent
const bool Tile::AdjacencyTest(const Tile *test_tile) const {
	// get vector of adjacent tiles
	std::vector<Tile*> adjacent_tiles = GameManager::game().get_instance().get_map().AdjacentTo(this);
	// if go through vector without finding the test_tile then return false, else return true
	return std::find(adjacent_tiles.begin(), adjacent_tiles.end(), test_tile) != adjacent_tiles.end();
}

// render the tile
void Tile::Render() const {
	Window display = GameManager::game().get_display();
	ColourScheme original_colour_scheme = display.get_colour_scheme(); // save original colour scheme to set back later
	// set colour scheme for tyle type
	display.set_colour_scheme(get_colour_scheme());
	// iterate over console cells for tile width and height
	for (int tile_j = 0; tile_j < parent_map_->get_tile_height(); tile_j++) {
		for (int tile_i = 0; tile_i < parent_map_->get_tile_width(); tile_i++) {
			Coord tile_component = { parent_map_->get_map_x_offset() + map_coords_.x * parent_map_->get_tile_width() + tile_i, parent_map_->get_map_y_offset() + map_coords_.y * parent_map_->get_tile_height() + tile_j };
			// set console cursor position to given tile component
			display.GoTo(tile_component);
			// output the tile marker
			std::cout << tile_marker_;
		}
	}
	// revert to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}
