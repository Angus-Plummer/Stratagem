#pragma once
#include "game_object.h"
class Unit : public GameObject {
protected:
	// max and current hp
	int max_hp_;
	int current_hp_;
	// armor value
	int armour_;
	// maximum move distance
	int move_range_;
public:
	Unit();
	virtual ~Unit();

	// accessor functions
	int get_max_hp() const { return max_hp_; }
	int get_current_hp() const { return current_hp_; }
	void set_current_hp(int const hp); // will set to 0 if < 0 and max_hp if > max_hp
	int get_armour() const { return armour_; }
	int get_move_distance() const { return move_range_; }

	// move unit to a new coordinate
	void move(COORD const new_pos) const;
};

