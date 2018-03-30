#include "unit_archer.h"
#include "tile.h"
#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"


Archer::Archer(GameInstance* game, Map* map, int team) : Unit(game, map, team) {
	// knight stats
	max_hp_ = 20;
	armour_ = 1;
	move_range_ = 3;
	attack_damage_ = 8;
	attack_range_ = 2;
	// set hp
	current_hp_ = max_hp_;
	// game object properties
	marker_ = 'A';
	type_ = "archer";
}


Archer::~Archer()
{
}


// returns true if unit can cross a terrain tile. ( archer cant cross water)
bool Archer::CanTraverse(Tile* const terrain_tile) const {
	// if tile is water tile then return false
	if (dynamic_cast<WaterTile*>(terrain_tile)) {
		return false;
	}
	// otherwise return true
	else {
		return true;
	}
}
