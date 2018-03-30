#include "unit.h"

class Archer : public Unit {
public:
	Archer(Map* map, bool team);
	~Archer();
	void Attack(Unit* target) const;
	bool CanTraverse(Tile* const terrain_tile) const;
};
