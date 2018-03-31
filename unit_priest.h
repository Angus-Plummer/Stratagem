#pragma once

#include "unit.h"

class Priest : public Unit {
public:
	Priest(const int &team);
	~Priest();
	bool CanTraverse(const Tile *terrain_tile) const;
};
