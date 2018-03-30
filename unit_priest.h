#pragma once

#include "unit.h"

class Priest : public Unit {
public:
	Priest(GameInstance* game, Map* map, int team);
	~Priest();
	void Attack(Unit* target) const;
	bool CanTraverse(Tile* const terrain_tile) const;
};
