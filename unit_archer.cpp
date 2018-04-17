#include "unit_archer.h"

#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"

Archer::Archer(const int &team) : Unit(team) {
	// archer stats
	max_hp_ = 20;
	armour_ = 0;
	move_range_ = 2;
	attack_damage_ = 8;
	attack_range_ = 2;
	armour_pierce_ = 3;
	// set hp
	current_hp_ = max_hp_;
	// game object properties
	unit_marker_ = "Ar";
	type_ = "archer";
}


Archer::~Archer()
{
}


// returns true if unit can cross a terrain tile. ( archer cant cross water or mountains)
const bool Archer::CanTraverse(const Tile *terrain_tile) const {
	// if tile is water tile then return false
	if (dynamic_cast<WaterTile*>(const_cast<Tile*>(terrain_tile))) {
		return false;
	}
	// if tile is a mountain tile then return false
	else if (dynamic_cast<MountainTile*>(const_cast<Tile*>(terrain_tile))) {
		return false;
	}
	// otherwise return true
	else {
		return true;
	}
}

