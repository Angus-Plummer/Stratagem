#pragma once

#include "unit.h"

// derived unit class for warrior units
class Warrior : public Unit {
protected:
	// clone helper function returns a new warrior
	Warrior* clone_impl() const;
public:
	// ctor and dtor
	Warrior(const int &team);
	~Warrior();
	// returns whether a warrior can enter a given tile type
	const bool CanTraverse(const Tile *terrain_tile) const;
};
