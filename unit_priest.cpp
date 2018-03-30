#include "unit_priest.h"
#include "tile.h"
#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"

Priest::Priest(GameInstance* game, Map* map, int team) : Unit(game, map, team) {
	// knight stats
	max_hp_ = 15;
	armour_ = 0;
	move_range_ = 2;
	attack_damage_ = 5;
	attack_range_ = 1;
	// set hp
	current_hp_ = max_hp_;
	// game object properties
	marker_ = 'P';
	type_ = "priest";
}

Priest::~Priest(){
}

// returns true if unit can cross a terrain tile. ( priest cant cross water)
bool Priest::CanTraverse(Tile* const terrain_tile) const {
	// if tile is water tile then return false
	if (dynamic_cast<WaterTile*>(terrain_tile)) {
		return false;
	}
	// otherwise return true
	else {
		return true;
	}
}
