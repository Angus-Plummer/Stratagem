#pragma once

#include "stdafx.h"
#include "game_object.h"

// forward declarations
class Tile;
class MoveSequence;

class Unit : public GameObject {
protected:
	// string identifier (units use 2 chars to identify them)
	std::string unit_marker_;
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
	Unit(const int &team);
	virtual ~Unit();

	// accessor functions
	int get_max_hp() const { return max_hp_; }
	int get_current_hp() const { return current_hp_; }
	void set_current_hp(int const &hp); // will set to 0 if < 0 and max_hp if > max_hp
	double get_armour() const { return armour_; }
	int get_move_distance() const { return move_range_; }
	bool is_alive() const { return alive_; }
	int get_team() const { return team_; }
	bool is_selecting_movement() const { return selecting_movement_; }
	bool is_selecting_attack() const { return selecting_attack_; }


	// apply damage and healing
	void ApplyPhysicalDamage(const int &dmg);
	void ApplyHeal(const int &heal);

	// get if the unit can currently move
	bool has_moved_this_turn() const { return moved_this_turn_; }
	// get if the unit can currently attack
	bool has_attacked_this_turn() const { return attacked_this_turn_; }

	// move unit to a new coordinate. 
	void set_map_coords(const Coord &new_pos);

	// select this unit
	void SelectUnit();
	// deselect this unit
	void DeselectUnit();

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
	void AnimateMovement(const Tile* target_tile) const;

	// Move to a target tile (will not work for an invalid tile or when there is no legal movement to the tile)
	void MoveTo(Tile* target_tile);

};
