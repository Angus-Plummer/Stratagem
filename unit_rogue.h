#pragma once
#include "unit.h"

// dervied unit class for rogue units
class Rogue : public Unit {
protected:
	// clone helper function returns a new rogue
	Rogue* CloneHelper() const;
public:
	// ctor and dtor
	Rogue(const int &team);
	~Rogue();
	// returns whether a rogue can enter a given tile type
	const bool CanTraverse(const Tile *terrain_tile) const;
};
