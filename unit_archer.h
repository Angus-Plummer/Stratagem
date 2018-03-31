#pragma once

#include "unit.h"

class Archer : public Unit {
public:
	Archer(const int &team);
	~Archer();
	bool CanTraverse(const Tile *terrain_tile) const;
};
