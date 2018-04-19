#pragma once

#include "stdafx.h"
// forward declaration
class Tile;

// class for path finding (connects tiles together in sequences)
class MoveSequence{
protected:
	Tile* tile_; // pointer to this tile in chain
	MoveSequence* parent_; // pointer to parent of this tile
	int cost_to_here_; // cost to reach this tile from starting point
	int heuristic_to_target; // heuristic for cost to reach destination
public:
	// ctor and dtor
	MoveSequence(Tile *tile);
	~MoveSequence();
	
	// accessors and mutators
	void set_parent(MoveSequence &new_parent);
	const MoveSequence* get_parent() const;
	Tile* get_tile() const; // get tile that the movesequence object corresponds to
	const int& get_cost() const; // cost_to_here

	// set the heuristic for this tile
	void set_heuristic_to(const Tile *target_tile);

	// get the score of the tile (cost to reach tile + heuristic to reach target tile)
	const int get_score() const;

	// updates the cost for this tile
	void Update();

	// check if two move sequences point to the same map tile
	const bool operator==(const MoveSequence &rhs);

};
