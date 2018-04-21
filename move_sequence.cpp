#include "move_sequence.h"
#include "tile.h"

// ---------- ctor, dtor, and operator overloading ---------- //

// ctor
MoveSequence::MoveSequence(Tile *tile) {//: tile_(tile){
	tile_ = tile;
	parent_ = nullptr;
	cost_to_here_ = 0;
	heuristic_to_target = 0;
}

// dtor
MoveSequence::~MoveSequence(){
}

// check if two move sequences point to the same map tile
const bool MoveSequence::operator==(const MoveSequence &rhs) {
	return (tile_ == rhs.tile_);
}

// ---------- accessors and mutators ---------- //

// parent in chain
void MoveSequence::set_parent(MoveSequence &new_parent){
	parent_ = &new_parent;
	Update();
}
const MoveSequence* MoveSequence::get_parent() const{ return parent_;}

// the tile this object corresponds to
Tile* MoveSequence::get_tile() const { return tile_;}

// cost to reach this tile from the starting tile
const int& MoveSequence::get_cost() const { return cost_to_here_;}

// ---------- other public functions ---------- //

// get the score of the tile (cost to reach tile + heuristic to reach target tile)
const int MoveSequence::Score() const {
	return cost_to_here_ + heuristic_to_target;
}

// get the heuristic for this tile. uses manhattan distance
void MoveSequence::Target(const Tile *target_tile) {
	int x_delta = target_tile->get_map_coords().x - tile_->get_map_coords().x;
	int y_delta = target_tile->get_map_coords().y - tile_->get_map_coords().y;
	heuristic_to_target =  abs(x_delta) + abs(y_delta);
}

// update the cost to reach this tile
void MoveSequence::Update() {
	// if tile has a parent then its cost is parents cost + the tile movement cost
	if (get_parent() != nullptr) {
		cost_to_here_ = parent_->cost_to_here_+ tile_->get_move_cost();
	}
	// if it has no parent then it is the starting tile and thus has 0 cost
	else {
		cost_to_here_ = 0;
	}
}