#pragma once

#include "unit.h"

class Warrior : public Unit {
public:
	Warrior(const int &team);
	~Warrior();
	bool CanTraverse(const Tile *terrain_tile) const;
};
