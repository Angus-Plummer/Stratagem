#pragma once

#include "stdafx.h"
#include "game_object.h"

// current state of the unit
enum UnitState {
	STATE_IDLE, // unit is unselected and can still be used
	STATE_ASLEEP, // unit is unselected and has already acted this turn, or it is currently the opposing team's turn
	STATE_DEAD, // unit is dead
	STATE_SELECTED // unit is selected
};

// forward declarations
class Tile;
class MoveSequence;

// abstract base class for units, the different unit types inherit from this class
class Unit : public GameObject {
protected:
	UnitState state_; // the current state of the unit
	std::string unit_marker_; // string identifier (2 chars that are drawn on the map to represent the unit)
	int team_; // what team the unit belongs to
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

	ColourScheme acted_colour_scheme_; // colour scheme when unit has already acted on a turn
	ColourScheme dead_colour_scheme_; // colour scheme when the unit is dead

	// ---------- internal functions ---------- //

	// get a deep copy of the unit
	virtual Unit* CloneHelper() const = 0;

	// checks how many tiles away another unit is
	const int DistanceTo(const Unit *target) const;

	// returns a vector of all units that are currently attackable by this unit
	const std::vector<Unit*> AttackableUnits() const;

	// get pointer to the tile unit is currently on
	Tile* GetTile() const;

	// returns a vector of pointers to the tiles (as movesequences) that can be reached by this unit
	std::vector<Tile*> ReachableTiles() const;

	// animates the unit moving from a starting tile to and adjacent tile
	void AnimateMovement(const Tile* start_tile, const Tile* target_tile) const;


public:
	// constructor and destructor
	Unit(const int &team);
	virtual ~Unit();

	// accessors 
	bool has_moved_this_turn() const;
	bool has_attacked_this_turn() const;
	const int get_team() const;

	// get a unique pointer to a copy of this object
	std::unique_ptr<Unit> Clone() const;

	// set hp (will set to 0 if < 0 and max_hp if > max_hp)
	void set_current_hp(int const &hp);

	// get the appropriate colour scheme for the unit
	const ColourScheme& get_colour_scheme() const;

	// move unit to a new coordinate. 
	void set_map_coords(const Coord &new_pos);

	// check if unit can traverse a given terrain
	virtual const bool CanTraverse(const Tile *terrain_tile) const = 0;

	// reset the has_moved_this_turn and has_attacked_this_turn flags to false and set the state to idle
	void EnableActions();
	// set the flags to true and state to asleep
	void DisableActions();

	// find out if this unit can be selected (if it has either moved or attacked already)
	const bool CanSelect() const;
	// select this unit
	void SelectUnit();
	// deselect this unit
	void DeselectUnit();

	// do unit death
	void Kill();

	// function return true if unit can move to any tiles
	const bool CanMove() const;
	// returns true if the unit can legally reach the target tile in one movement
	const bool CanReach(const Tile* target_tile);
	// function return true if unit can attack (units in range and not attacked yet)
	const bool CanAttack() const;
	// check if a target unit is attackable by this unit
	const bool CanAttackTarget(const Unit *target) const;

	// attack another unit
	void Attack(Unit *target);
	// apply damage and healing
	void AttackedBy(const Unit *target);

	// sets highlighted status for tiles/units that are attackable
	void HighlightAttackableTiles(const bool &highlight) const;

	// sets highlighted status for the tiles that the unit can reach
	void Unit::HighlightReachableTiles(const bool &highlight) const;

	// Move to a target tile (will not work for an invalid tile or when there is no legal movement to the tile)
	void MoveTo(Tile* target_tile);

	// renders the unit in the console window
	void Render() const;
};
