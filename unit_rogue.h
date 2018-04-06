#pragma once
#include "unit.h"

class Rogue : public Unit {
public:
	Rogue(const int &team);
	~Rogue();
	bool CanTraverse(const Tile *terrain_tile) const;
};
