#pragma once

#include "unit.h"

// dervied unit class for archer units
class Archer : public Unit {
protected:
	// clone helper function returns a new archer
	Archer * CloneHelper() const;
public:
	// ctor and dtor
	Archer(const int &team);
	~Archer();
	// returns whether an archer can enter a given tile type
	const bool CanTraverse(const Tile *terrain_tile) const;
};
