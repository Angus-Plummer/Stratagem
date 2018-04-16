#include "unit_warrior.h"
#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"


Warrior::Warrior(const int &team) : Unit(team) {
	// warrior stats
	max_hp_ = 30;
	armour_ = 3;
	move_range_ = 2;
	attack_damage_ = 8;
	attack_range_ = 1;
	armour_pierce_ = 1;
	// set hp
	current_hp_ = max_hp_;
	// game object properties
	unit_marker_ = "Wr";
	type_ = "warrior";
}


Warrior::~Warrior(){
}

// returns true if the warrior can cross the terrain. (warrior cant enter water or mountains)
const bool Warrior::CanTraverse(const Tile *terrain_tile) const {
	// if tile is a water tile then return false
	if (dynamic_cast<WaterTile*>(const_cast<Tile*>(terrain_tile))) { // have to remove const qualifier first
		return false;
	}
	// if tile is a mountain tile then return false
	else if (dynamic_cast<MountainTile*>(const_cast<Tile*>(terrain_tile))) { // have to remove const qualifier first
		return false;
	}
	// otherwise return true
	else {
		return true;
	}
}

