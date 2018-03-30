#include "tile.h"
#include "unit_archer.h"


Archer::Archer(Map* map, bool team) : Unit(map, team) {
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

// function to attack a target
void Archer::Attack(Unit* target) const {
	if (IsAdjacent(target) && target->get_team() != team_) {
		int attack_modifier = GetTile()->get_atk_modifier();
		target->ApplyPhysicalDamage(attack_damage_ + attack_modifier);
	}
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

