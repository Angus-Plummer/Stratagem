#pragma once

#include "stdafx.h"
#include "game_object.h"

// current state of the unit
enum UnitState {
	STATE_IDLE, // unit is unselected and can still be used
	STATE_ASLEEP, // unit is unselected and has already acted this turn
	STATE_DEAD, // unit is dead
	STATE_SELECTED // unit is selected
};

// forward declarations
class Tile;
class MoveSequence;

class Unit : public GameObject {
protected:
	UnitState state_; // the current state of the unit
	std::string unit_marker_; // string identifier (2 chars that are drawn on the map to represent the unit)
	int max_hp_; // maximum hit points of the unit
	int current_hp_; // the current hit points of the unit
	int armour_; // armor value ( reduces damage additatively)
	int armour_pierce_; // armour piercing value of attack
	//int magic_resistance_; // magic armour value;
	int move_range_; // maximum move distance
	int attack_damage_; // default attack damage
	int attack_range_; 	// default attack range
	bool moved_this_turn_; // whether unit has/hasnt moved yet
	bool attacked_this_turn_; // whether unit has/hasnt attacked yet
	int team_; // what team the unit belongs to

	ColourScheme acted_colour_scheme_; // colour scheme when unit has already acted on a turn
	ColourScheme dead_colour_scheme_; // colour scheme when the unit is dead

public:
	Unit(const int &team);
	virtual ~Unit();

	// accessors and mutators
	int get_max_hp() const { return max_hp_; }
	int get_current_hp() const { return current_hp_; }
	void set_current_hp(int const &hp); // will set to 0 if < 0 and max_hp if > max_hp
	double get_armour() const { return armour_; }
	int get_move_distance() const { return move_range_; }
	int get_team() const { return team_; }
	bool has_moved_this_turn() const { return moved_this_turn_; }
	bool has_attacked_this_turn() const { return attacked_this_turn_; }

	// get the appropriate colour scheme for the unit
	ColourScheme get_colour_scheme() const;

	// apply damage and healing
	void AttackedBy(const Unit *target);
	//void ApplyHeal(const int &heal);

	// move unit to a new coordinate. 
	void set_map_coords(const Coord &new_pos);

	// find out if this unit can be selected (if it has either moved or attacked already)
	bool CanSelect() const;
	// select this unit
	void SelectUnit();
	// deselect this unit
	void DeselectUnit();

	// reset the has_moved_this_turn and has_attacked_this_turn flags to false and set the state to idle
	void EnableActions();
	// set the flags to true and state to asleep
	void DisableActions();

	// do unit death
	void Kill();

	// function return true if unit can move
	bool CanMove() const;
	// function return true if unit can attack
	bool CanAttack() const;

	// check if a target unit is attackable by this unit
	bool CanAttackTarget(const Unit *target) const;

	// attack another unit
	void Attack(Unit *target);

	// returns a vector of all units that are currently attackable by this unit
	std::vector<Unit*> AttackableUnits() const;

	// highlights tiles/units that are attackable
	void HighlightAttackableUnits(const bool &highlight) const;

	// get tile unit is currently on
	Tile* GetTile() const;

	// checks how many tiles away another unit is
	int DistanceTo(const Unit *target) const;

	// check if unit can traverse a given terrain
	bool virtual CanTraverse(const Tile *terrain_tile) const = 0;

	// renders the unit in the console
	void Render() const;

	// returns true if the unit can legally reach the target tile in one movement
	bool CanReach(const Tile* target_tile);

	// returns a vector of pointers to the tiles (as movesequences) that can be reached by this unit
	std::vector<Tile*> ReachableTiles() const;

	// highlight the tiles that the unit can reach
	void Unit::HighlightReachableTiles(const bool &highlight) const;

	// animates the unit moving to an adjacent tile
	void AnimateMovement(const Tile* start_tile, const Tile* target_tile) const;

	// Move to a target tile (will not work for an invalid tile or when there is no legal movement to the tile)
	void MoveTo(Tile* target_tile);

};
