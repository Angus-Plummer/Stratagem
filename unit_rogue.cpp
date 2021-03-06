 #include "unit_rogue.h"
#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"

// ctor
Rogue::Rogue(const int &team) : Unit(team) {
	// Rogue stats
	max_hp_ = 25;
	armour_ = 1;
	move_range_ = 3;
	attack_damage_ = 9;
	attack_range_ = 1;
	armour_pierce_ = 0;
	// set starting
	current_hp_ = max_hp_;
	// output properties
	unit_marker_ = "Rg";
	type_ = "rogue";
}

// dtor
Rogue::~Rogue(){}

// clone helper function returns a new rogue
Rogue* Rogue::CloneHelper() const { return new Rogue(*this); }

// returns true if unit can cross a terrain tile. ( rogue cant cross water)
const bool Rogue::CanTraverse(const Tile *terrain_tile) const {
	// if tile is water tile then return false
	if (dynamic_cast<WaterTile*>(const_cast<Tile*>(terrain_tile))) {
		return false;
	}
	// otherwise return true
	else {
		return true;
	}
}
