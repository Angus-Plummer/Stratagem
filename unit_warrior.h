#pragma once

#include "unit.h"

class Warrior : public Unit {
protected:
	Warrior * clone_impl() const { return new Warrior(*this); }
public:
	Warrior(const int &team);
	~Warrior();
	const bool CanTraverse(const Tile *terrain_tile) const;
};
