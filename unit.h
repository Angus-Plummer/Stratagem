#pragma once

#include "stdafx.h"
#include "game_object.h"

// forward declarations
class Tile;
class MoveSequence;

class Unit : public GameObject {
protected:
	std::string unit_marker_; // string identifier (units use 2 chars to identify them)
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
	bool selecting_movement_; // true if unit is currently selected for movement
	bool selecting_attack_; // true if unit is currently selected for attacking
	bool alive_; // whether unit is alive or not
	int team_; // what team the unit belongs to
	int selected_colour_scheme_; // colour scheme when selected
	int acted_colour_scheme_; // colour scheme when unit has already acted on a turn

public:
	Unit(const int &team);
	virtual ~Unit();

	// accessors and mutators
	int get_max_hp() const { return max_hp_; }
	int get_current_hp() const { return current_hp_; }
	void set_current_hp(int const &hp); // will set to 0 if < 0 and max_hp if > max_hp
	double get_armour() const { return armour_; }
	int get_move_distance() const { return move_range_; }
	bool is_alive() const { return alive_; }
	int get_team() const { return team_; }
	bool is_selecting_movement() const { return selecting_movement_; }
	bool is_selecting_attack() const { return selecting_attack_; }
	bool has_moved_this_turn() const { return moved_this_turn_; }
	bool has_attacked_this_turn() const { return attacked_this_turn_; }

	// get the appropriate colour scheme for the unit
	int get_colour_scheme() const;

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

	// reset the has_moved_this_turn and has_attacked_this_turn flags to false
	void EnableActions();

	// do unit death
	void Die();

	// function return true if unit can move
	bool CanMove() const;
	// function return true if unit can attack
	bool CanAttack() const;

	// function to run when movement option is chosen from the action menu
	void ChooseMovement();
	// cancels the state of selecting movement
	void UnChooseMovement();

	// function to run when attack option is chosen from the action menu
	void ChooseAttack();
	// cancels the state of selecting attack
	void UnChooseAttack();

	// marks the possible actions on the screen, depending on the type of action
	void ShowPossibleAction(const int &action_type);

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

	// renders the unit on a screen
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
