#pragma once

#include "unit.h"

class Archer : public Unit {
public:
	Archer(GameInstance* game, Map* map, int team);
	~Archer();
	void Attack(Unit* target) const;
	bool CanTraverse(Tile* const terrain_tile) const;
};
