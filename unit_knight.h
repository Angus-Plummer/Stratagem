#pragma once

#include "unit.h"

class Knight : public Unit {
public:
	Knight(const int &team);
	~Knight();
	bool CanTraverse(const Tile *terrain_tile) const;
};
