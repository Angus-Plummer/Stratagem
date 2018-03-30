#include "unit.h"
class Knight : public Unit {
public:
	Knight(Map* map, bool team);
	~Knight();
	void Attack(Unit* target) const;
	bool CanTraverse(Tile* const terrain_tile) const;
};
