#pragma once

#include "unit.h"

class Archer : public Unit {
protected:
	Archer * clone_impl() const { return new Archer(*this); }
public:
	Archer(const int &team);
	~Archer();
	const bool CanTraverse(const Tile *terrain_tile) const;
};
