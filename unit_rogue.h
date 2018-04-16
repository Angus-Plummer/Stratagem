#pragma once
#include "unit.h"

class Rogue : public Unit {
protected:
	Rogue * clone_impl() const { return new Rogue(*this); }
public:
	Rogue(const int &team);
	~Rogue();
	const bool CanTraverse(const Tile *terrain_tile) const;
};
