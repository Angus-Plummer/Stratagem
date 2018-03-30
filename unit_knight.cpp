#include "unit_knight.h"
#include "tile.h"
#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"


Knight::Knight(GameInstance* game, Map* map, int team) : Unit(game, map, team) {
	// knight stats
	max_hp_ = 30;
	armour_ = 2;
	move_range_ = 2;
	attack_damage_ = 10;
	attack_range_ = 1;
	// set hp
	current_hp_ = max_hp_;
	// game object properties
	marker_ = 'K';
	type_ = "knight";
}


Knight::~Knight(){
}

// returns true if the knight can cross the terrain. (knight cant enter water or mountains)
bool Knight::CanTraverse(Tile* const terrain_tile) const {
	// if tile is a water tile then return false
	if (dynamic_cast<WaterTile*>(terrain_tile)) {
		return false;
	}
	// if tile is a mountain tile then return false
	else if (dynamic_cast<MountainTile*>(terrain_tile)) {
		return false;
	}
	// otherwise return true
	else {
		return true;
	}
}

