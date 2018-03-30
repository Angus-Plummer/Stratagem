#include "unit_priest.h"
#include "tile.h"

Priest::Priest(Map* map, bool team) : Unit(map, team) {
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

// function to attack a target
void Priest::Attack(Unit* target) const {
	// only attack if adjacent
	if (IsAdjacent(target) && target->get_team() != team_) {
		// add modifier and apply damage
		int attack_modifier = GetTile()->get_atk_modifier();
		target->ApplyPhysicalDamage(attack_damage_ + attack_modifier);
	}
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
