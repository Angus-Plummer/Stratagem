#include "unit.h"



Unit::Unit()
{
}


Unit::~Unit()
{
}

// set new current hp value
void Unit::set_current_hp(int const hp) {  // will set to 0 if < 0 and max_hp if > max_hp
	if (hp > max_hp_) {
		current_hp_ = max_hp_;
	}
	else if (hp < 0) {
		current_hp_ = 0;
	}
	else {
		current_hp_ = hp;
	}
}

// move unit to a new coordinate
void Unit::move(COORD const new_pos) const {
	// need some sort of check to see if the new coordinate is valid ( on the game map )
}
