#include "unit.h"
#include "map.h"
#include "screen.h"
#include "tile.h"



Unit::Unit(Map* map, bool team): GameObject(map), team_(team), alive_(true), can_move_(true){
	if (team_) {
		default_colour_scheme_ = 31;
		selected_colour_scheme_ = 191;
		//highlighted_colour_scheme_ = 
	}
	else {
		default_colour_scheme_ = 79;
		selected_colour_scheme_ = 207;
	}
}


Unit::~Unit()
{
}

// set new current hp value
void Unit::set_current_hp(int const hp) {  // will set to 0 if < 0 and max_hp if > max_hp
	if (hp > max_hp_) {
		current_hp_ = max_hp_;
	}
	else if (hp <= 0) {
		current_hp_ = 0;
		alive_ = false;
	}
	else {
		current_hp_ = hp;
	}
}
// apply a damage value, this is modified linearly by the terrain and armour of the unit
void Unit::ApplyPhysicalDamage(int const dmg) {
	int terrain_modifier = parent_map_->GetTile(map_coords_)->get_def_modifier();
	int received_damage = dmg - armour_ - terrain_modifier; //
	set_current_hp(current_hp_ - received_damage);
	
}
void Unit::ApplyHeal(int const heal) {
	set_current_hp(current_hp_ + heal);
}

// get tile unit is currently on
Tile* Unit::GetTile() const {
	return parent_map_->GetTile(map_coords_);
}

// check if target unit is adjacent to this unit
bool Unit::IsAdjacent(Unit* const target) const {
	// shorter names so easier to read
	int unit_x = map_coords_.X;
	int unit_y = map_coords_.Y;
	int target_x = target->get_map_coords().X;
	int target_y = target->get_map_coords().Y;
	// if target is either in the tile above, below, left, or right then return true
	if ((target_x == unit_x && (target_y == unit_y + 1 || target_y == unit_y - 1))
		|| (target_y == unit_y && (target_x == unit_x + 1 || target_x == unit_x - 1)))
	{
		return true;
	}
	else {
		return false;
	}
}

// move unit to a new coordinate
void Unit::set_map_coords(COORD const new_pos) {
	// need to check if the new coordinate is valid ( on the game map )
	map_coords_ = new_pos;
}

void Unit::Render(Screen &display) const {
	int original_colour_scheme = display.get_colour_scheme(); // save original colour scheme to set back late
	// set colour scheme of unit
	display.set_colour_scheme(get_colour_scheme());
	// set console cursor position to given tile component
	display.GoTo(display.get_map_x_offset() + map_coords_.X * display.get_tile_width() + 2, display.get_map_y_offset() + map_coords_.Y * display.get_tile_height() + 1);
	// output the tile marker
	std::cout << marker_;
	// revert to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}
