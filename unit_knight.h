#pragma once

#include "unit.h"

class Knight : public Unit {
public:
	Knight(GameInstance* game, Map* map, int team);
	~Knight();
	void Attack(Unit* target) const;
	bool CanTraverse(Tile* const terrain_tile) const;
};
