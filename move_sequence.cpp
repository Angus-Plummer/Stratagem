#include "move_sequence.h"
#include "tile.h"



MoveSequence::MoveSequence(Tile *tile) : tile_(tile){
	parent_ = nullptr;
	cost_to_here_ = 0;
	estimated_left_ = 0;
}


MoveSequence::~MoveSequence(){
}

// accessors and mutators for parent in chain
void MoveSequence::set_parent(MoveSequence &new_parent){
	parent_ = &new_parent;
	Update();
}
MoveSequence* MoveSequence::get_parent() const{
	return parent_;
}

// get the tile that this MoveSequence component belongs to
Tile* MoveSequence::get_tile() const{
	return tile_;
}
// get the cost of reaching this tile from the intial tile
int MoveSequence::get_cost() const {
	return cost_to_here_;
}

// update the cost to reach this tile
void MoveSequence::Update() {
	// if tile has a parent then its cost is parents cost + the tile movement cost
	if (get_parent() != nullptr) {
		cost_to_here_ = parent_->cost_to_here_+ tile_->get_move_cost();
	}
	// if it has not parent then it is the starting tile and thus has 0 cost
	else {
		cost_to_here_ = 0;
	}
}

// check if two move sequences point to the same map tile
bool MoveSequence::operator==(const MoveSequence &rhs){
	return (tile_ == rhs.tile_);
}

// check which of two move sequence objects has lowest score
bool MoveSequence::operator<(const MoveSequence &rhs) {
	return ( cost_to_here_ < rhs.cost_to_here_);
}

/*// copy assignment operator overloading
void MoveSequence::operator=(MoveSequence const &rhs) {
	tile_ = rhs.tile_;
	parent_ = rhs.parent_;
	cost_to_here_ = rhs.cost_to_here_;
	estimated_left_ = rhs.estimated_left_;
}
*/