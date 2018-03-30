#pragma once

#include "stdafx.h"
#include "game_object.h"

// forward declarations
class Tile;


class Unit : public GameObject {
protected:
	// max and current hp
	int max_hp_;
	int current_hp_;
	// armor value
	int armour_;
	// magic armour value;
	int magic_resistance_;
	// maximum move distance
	int move_range_;
	// default attack damage
	int attack_damage_;
	// default attack range
	int attack_range_;
	bool moved_this_turn_; // whether unit has/hasnt moved yet
	bool attacked_this_turn_; // whether unit has/hasnt attacked yet
	bool selecting_movement_; // true if unit is currently selected for movement
	bool selecting_attack_; // true if unit is currently selected for attacking
	bool alive_; // whether unit is alive or not
	int team_; // what team the unit belongs to

	int selected_colour_scheme_; // colour scheme when selected

public:
	Unit(GameInstance* game, Map* map, int team);
	virtual ~Unit();

	// accessor functions
	int get_max_hp() const { return max_hp_; }
	int get_current_hp() const { return current_hp_; }
	void set_current_hp(int const hp); // will set to 0 if < 0 and max_hp if > max_hp
	void ApplyPhysicalDamage(int const dmg);
	void ApplyHeal(int const heal);
	double get_armour() const { return armour_; }
	int get_move_distance() const { return move_range_; }
	bool is_alive() const { return alive_; }
	int get_team() const { return team_; }

	// select this unit
	void SelectUnit();
	// deselect this unit
	void DeselectUnit();

	// attack another unit
	void Attack(Unit*target) const;

	// get tile unit is currently on
	Tile* GetTile() const;

	// checks how many tiles away another unit is
	int DistanceTo(Unit* const target) const;

	// check if unit can traverse a given terrain
	bool virtual CanTraverse(Tile* const terrain_tile) const = 0;

	// move unit to a new coordinate. 
	void set_map_coords(COORD const new_pos);

	// renders the unit on a screen
	void Render(Screen const &display) const;
	
	// returns a vector of pointers to the tiles that can me reached by this unit
	std::vector<Tile*> ReachableTiles() const;
};
