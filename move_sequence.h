#pragma once

#include "stdafx.h"
#include "tile.h"

// class for path finding (connects tiles together in sequences)

class MoveSequence{
protected:
	Tile* tile_; // pointer to this tile in chain
	MoveSequence* parent_; // pointer to parent of this tile
	int cost_to_here_; // cost to reach this tile from starting point
	int estimated_left_; // heuristic for cost to reach destination
public:
	// ctor and dtor
	MoveSequence(Tile* tile);
	~MoveSequence();
	
	// accessors and mutators for parent in chain
	void set_parent(MoveSequence &new_parent);
	MoveSequence* get_parent() const;

	// get the tile that this MoveSequence component belongs to
	Tile* get_tile() const;

	// get the cost to reach this tile
	int get_cost() const;

	// updates the cost for this tile
	void Update();


	// check if two move sequences point to the same map tile
	bool operator==(MoveSequence const &rhs);

	// check which of two move sequence objects has lowest score
	bool operator<(MoveSequence const &rhs);

	// copy assignment operator overloading
	//void operator=(MoveSequence const &rhs);

};
