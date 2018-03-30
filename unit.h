#pragma once
#include<memory>
#include "game_object.h"
#include "tile_grass.h"
#include "tile_forest.h"
#include "tile_mountain.h"
#include "tile_water.h"
class Tile; //forward declare TerrainTile

class Unit : public GameObject {
protected:
	// max and current hp
	int max_hp_;
	int current_hp_;
	// armor value
	double armour_;
	// magic armour value;
	double magic_resistance_;
	// maximum move distance
	int move_range_;
	// default attack damage
	int attack_damage_;
	// default attack range
	int attack_range_;
	bool can_move_; // whether unit has/hasnt moved yet
	bool alive_; // whether unit is alive or not
	bool team_; // what team the unit belongs to

public:
	Unit(Map* map, bool team);
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
	bool get_team() const { return team_; }

	// get tile unit is currently on
	Tile* GetTile() const;

	// checks if target unit is adjacent to this unit
	bool IsAdjacent(Unit* const target) const;

	// check if unit can traverse a given terrain
	bool virtual CanTraverse(Tile* const terrain_tile) const = 0;

	// move unit to a new coordinate. 
	void set_map_coords(COORD const new_pos);

	// attack another unit
	void virtual Attack(Unit*target) const = 0;

	void Render(Screen &display) const;
};

