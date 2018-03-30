#include "unit.h"
class Priest : public Unit {
public:
	Priest(Map* map, bool team);
	~Priest();
	void Attack(Unit* target) const;
	bool CanTraverse(Tile* const terrain_tile) const;
};
