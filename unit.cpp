#include "unit.h"
#include "game.h"
#include "screen.h"
#include "map.h"
#include "tile.h"
#include "move_sequence.h"

#define action_movement 0
#define action_attack 1

// ctor
Unit::Unit(const int &team): GameObject(), team_(team), alive_(true), moved_this_turn_(false), 
								attacked_this_turn_(false), selecting_movement_(false), selecting_attack_(false){
	if (team_ == 1) {
		default_colour_scheme_ = 31;
		selected_colour_scheme_ = 191;
	}
	if (team_ == 2){
		default_colour_scheme_ = 79;
		selected_colour_scheme_ = 207;
	}
}

// dtor
Unit::~Unit()
{
}

// set new current hp value
void Unit::set_current_hp(const int &hp) {  // will set to 0 if < 0 and max_hp if > max_hp
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
void Unit::ApplyPhysicalDamage(const int &dmg) {
	int terrain_modifier = Game::instance().get_map().GetTile(map_coords_)->get_def_modifier();
	int received_damage = dmg - armour_ - terrain_modifier; //
	set_current_hp(current_hp_ - received_damage);
	
}

// apply heal value
void Unit::ApplyHeal(const int &heal) {
	set_current_hp(current_hp_ + heal);
}

// select this unit
void Unit::SelectUnit() {
	Game::instance().SelectUnit(this);
	if (!moved_this_turn_) {
		selecting_movement_ = true;
		ShowPossibleAction(action_movement);
	}
}

// deselect this unit
void Unit::DeselectUnit() {
	// set selection flags to false
	selecting_movement_ = false;
	selecting_attack_ = false;
	// reset the map tiles to remove highlighted movement tiles
	ResetReachableTiles();
}

void Unit::ShowPossibleAction(const int &action_type) {
	// if the action type is movement...
	if (action_type == action_movement) {
		HighlightReachableTiles();
	}
}

// function to attack a target
void Unit::Attack(Unit *target) const {
	// only attack if adjacent
	if (DistanceTo(target) <= attack_range_ && target->get_team() != team_) {
		// add modifier and apply damage
		int attack_modifier = GetTile()->get_atk_modifier();
		target->ApplyPhysicalDamage(attack_damage_ + attack_modifier);
	}
}

// get tile unit is currently on
Tile* Unit::GetTile() const {
	return Game::instance().get_map().GetTile(map_coords_);
}

// finds distance to target unit
int Unit::DistanceTo(const Unit *target) const {
	return (map_coords_.X - target->get_map_coords().X) + (map_coords_.Y - target->get_map_coords().Y);
}

// move unit to a new coordinate
void Unit::set_map_coords(const COORD &new_pos) {
	// need to check if the new coordinate is valid ( on the game map )
	map_coords_ = new_pos;
}

void Unit::Render() const {
	Screen display = Game::instance().get_display();
	int original_colour_scheme = display.get_colour_scheme(); // save original colour scheme to set back late
	// set colour scheme of unit
	display.set_colour_scheme(default_colour_scheme_);
	// set console cursor position to given tile component
	display.GoTo(display.get_map_x_offset() + map_coords_.X * display.get_tile_width() + (display.get_tile_width() / 2 ), 
				 display.get_map_y_offset() + map_coords_.Y * display.get_tile_height() + (display.get_tile_height() / 2) );
	// output the tile marker
	std::cout << marker_;
	// revert to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}

// returns true if the unit can legally reach the target tile in one movement
bool Unit::CanReach(const Tile* target_tile) {
	std::vector<Tile*> reachable_tiles = ReachableTiles(); // vector of tiles reachable by this unit
	// iterates through the tiles, if still have not found after all in vector then this will return false
	return std::find(reachable_tiles.begin(), reachable_tiles.end(), target_tile) != reachable_tiles.end();
}

// returns a vector of pointers to the tiles that are reachable by a given unit (ignores the tile it is currently on)
std::vector<Tile*> Unit::ReachableTiles() const {
	std::vector<Tile*> can_reach; // tiles that the unit can reach
	std::list<MoveSequence> closed; // tiles that have already been inspected
	std::list<MoveSequence> open; // tiles that have yet to be inspected

	// start with the tile the unit is currently on, add to the vector of open tiles
	MoveSequence initial_tile(GetTile()); // maybe need to make a copy assignment operator to use this
	open.push_back(initial_tile);
	can_reach.push_back(initial_tile.get_tile());

	std::vector<Tile*> adjacent_tiles;

	// while there are any tiles left to inspect
	while (open.size() > 0) {
		// poitner to the current tile to be analysed is the one which has the lowest cost to reach so far
		auto current_iter = std::min_element(open.begin(), open.end(),
			[](const MoveSequence &lhs, const MoveSequence &rhs) {return lhs.get_cost() < rhs.get_cost(); }); // lambda function to compare the cost to reach the tiles
		// add it to the closed vector
		closed.push_back(*current_iter);
		// set pointer to the target MoveSequence object in the closed set. The set doesnt change when looping through neighbours so will stay valid
		MoveSequence* current = &(*std::find(closed.begin(), closed.end(), *current_iter));
		// remove it from the open vector
		open.erase(current_iter);
		// clear the vector of adjacent tiles and fill for the current tile
		adjacent_tiles.clear();
		adjacent_tiles = Game::instance().get_map().AdjacentTo(current->get_tile());
		// go through this vector
		for (auto iter = adjacent_tiles.begin(); iter != adjacent_tiles.end(); iter++) {
			// make a MoveSequence object from this adjacent tile and make its parent the currently inspected
			MoveSequence adjacent(*iter);
			adjacent.set_parent(*current);
			// see if this tile is already in the open and closed sets
			bool in_open_set = std::find(open.begin(), open.end(), adjacent) != open.end();
			bool in_closed_set = std::find(closed.begin(), closed.end(), adjacent) != closed.end();
			// if this tile is not already in the closed set then proceed (if it is in closed set then it has already been analysed and shortest path to it found)
			if (!in_closed_set) {
				// compute score
				// if the tile can be traversed by the unit then proceed
				if (CanTraverse(adjacent.get_tile())) {
					if (!in_open_set) { // if not in open set
						// if cost to reach it is less than max possible add it to the reachable tiles and the open set
						if (adjacent.get_cost() <= move_range_) {
							// if there is a unit on the tile..
							if (Game::instance().get_map().UnitPresent(adjacent.get_tile()->get_map_coords())) {
								// friendly unit -> can move through but not end on. non-friendly then neither
								if (team_ == Game::instance().get_map().GetUnit( adjacent.get_tile()->get_map_coords() )->get_team()) {
									open.push_back(adjacent);
								}
							}
							// if no unit on the tile then add to reachable tiles and to open set
							else {
								open.push_back(adjacent);
								can_reach.push_back(adjacent.get_tile());
							}
						}
					}
					else { // (in open set already)
						// pointer to object already in open set
						MoveSequence* in_set = &*std::find(open.begin(), open.end(), adjacent);
						// if adjacent tile costs less to reach from parent as currently inspected tile then update the parent of tile already in set
						if (adjacent.get_cost() < in_set->get_cost()) {
							in_set->set_parent(*current);
						}
					}
				}
			}
		}
	}
	return can_reach;
}

// highlight the tiles that the unit can reach
void Unit::HighlightReachableTiles() const {
	std::vector<Tile*> reachable_tiles = ReachableTiles(); // vector of reachable tiles
	for (auto iter = reachable_tiles.begin(); iter != reachable_tiles.end(); iter++) { // iterate through
																					   // set to higlighted and render
		(*iter)->set_highlighted(true);
		(*iter)->Render();
		// if tile has a unit on it then redraw the unit
		if (Game::instance().get_map().UnitPresent( (*iter)->get_map_coords() ) ) {
			Game::instance().get_map().GetUnit((*iter)->get_map_coords())->Render();
		}
	}
}

// unhighlight the tiles that the unit can reach
void Unit::ResetReachableTiles() const {
	std::vector<Tile*> reachable_tiles = ReachableTiles(); // vector of reachable tiles
	for (auto iter = reachable_tiles.begin(); iter != reachable_tiles.end(); iter++) { // iterate through
																					   // set to not higlighted and render
		(*iter)->set_highlighted(false);
		(*iter)->Render();
		// if tile has a unit on it then redraw the unit
		if (Game::instance().get_map().UnitPresent((*iter)->get_map_coords())) {
			Game::instance().get_map().GetUnit((*iter)->get_map_coords())->Render();
		}
	}
}

// animates the unit moving to an adjacent tile (does not move unit to the tile, just animates it)
void Unit::AnimateMovement(const Tile* target_tile) const {
	Screen display = Game::instance().get_display();
	// save the colour scheme to reset at end of function
	int initial_colour_scheme = display.get_colour_scheme();
	// if target tile is in adjacent to tile unit is on then animate movement
	if (GetTile()->AdjacencyTest(target_tile)) {
		int x_delta = target_tile->get_map_coords().X - map_coords_.X;
		int y_delta = target_tile->get_map_coords().Y - map_coords_.Y;
		int num_step;
		if (x_delta != 0) {
			num_step = display.get_tile_width();
		}
		else {
			num_step = display.get_tile_height();
		}
		// get initial console cursor location of the unit
		COORD initial_pos = { (SHORT)(display.get_map_x_offset() + map_coords_.X * display.get_tile_width() + (display.get_tile_width() / 2)),  
							  (SHORT)(display.get_map_y_offset() + map_coords_.Y * display.get_tile_height() + (display.get_tile_height() / 2)) };
		COORD current_pos = initial_pos;
		for (int step = 0; step <= num_step ; step++) {
			// got to the current position of the unit marker and replace the console cell with the tile marker using the tile colour scheme
			display.GoTo(current_pos.X, current_pos.Y); // got to current position
			// render the tile at the current position
			Game::instance().get_map().GetTileFromConsoleCoord(current_pos)->Render();
			// update the current tile position
			current_pos.X = initial_pos.X + x_delta * step;
			current_pos.Y = initial_pos.Y + y_delta * step;
			// set cursor to new position, set colour scheme to unit colour scheme and output the units marker
			display.GoTo(current_pos.X, current_pos.Y);
			display.set_colour_scheme(get_colour_scheme());
			std::cout << marker_;
			// sleep for a short time so can actually see the movement
			if (x_delta != 0) {
				Sleep(50);
			}
			if (y_delta != 0) { // if moving in y direction sleep for twice as long because console cells are twice as high as they are wide
				Sleep(100);
			}
		}
	}
	// set colour scheme back to what it was at start of function
	display.set_colour_scheme(initial_colour_scheme);
	// if target tile is not adjacent to the unit then throw error and do nothing
}

// Move to a target tile (will not work for an invalid tile or when there is no legal movement to the tile)
void Unit::MoveTo(Tile* target_tile) {
	// if the target tile is not it the reachable tiles then return an error and exit, otherwise continue
	if (!CanReach(target_tile)) {
		exit(1);
	}
	// clear the highlighting of the tiles that the unit could reach as not necessary now
	std::vector<Tile*> reachable_tiles = ReachableTiles();
	for (auto iter = reachable_tiles.begin(); iter != reachable_tiles.end(); iter++) {
		(*iter)->set_highlighted(false);
		(*iter)->Render();
	}

	std::list<MoveSequence> open; // tiles that will potentially be analysed
	std::list<MoveSequence> closed; // tiles that have been analysed
	MoveSequence initial_tile(GetTile()); // the tile current occupied by the unit
	MoveSequence end_tile(target_tile);
	// add the initial tile to the open set
	initial_tile.set_heuristic_to(target_tile);
	open.push_back(initial_tile);
	// vector to hold tiles adjcent to inspected tile
	std::vector<Tile*> adjacent_tiles;
	// keep iterating over this loop until end_tile has a parent, at that point the end tile must have been reached!
	while (!end_tile.get_parent() ) {
		// pointer to the current tile to be analysed is the one which has the lowest total estimated score so far
		auto current_iter = std::min_element(open.begin(), open.end(),
			[](const MoveSequence &lhs, const MoveSequence &rhs) {return lhs.get_score() < rhs.get_score(); }); // lambda function for comparison of scores
																												// add the current tile to the closed set
		closed.push_back(*current_iter);
		// set pointer to the target MoveSequence object in the closed set. The set doesnt change when looping through neighbours so will stay valid
		MoveSequence* current = &(*std::find(closed.begin(), closed.end(), *current_iter));
		// remove the currently inspected tile from the open set as it will now be analysed
		open.erase(current_iter);
		// clear the vector of adjacent tiles and fill for the current tile
		adjacent_tiles.clear();
		adjacent_tiles = Game::instance().get_map().AdjacentTo(current->get_tile());
		// go through the tiles in the vector of adjacent tiles
		for (auto iter = adjacent_tiles.begin(); iter != adjacent_tiles.end(); iter++) {
			// make a MoveSequence object from this adjacent tile and make its parent the currently inspected tile
			MoveSequence adjacent(*iter);
			adjacent.set_parent(*current);
			adjacent.set_heuristic_to(target_tile);
			// check if this tile is the target tile
			if (adjacent.get_tile() == end_tile.get_tile()) {
				end_tile = adjacent;
			}
			// see if this tile is already in the open and closed sets
			bool in_open_set = std::find(open.begin(), open.end(), adjacent) != open.end();
			bool in_closed_set = std::find(closed.begin(), closed.end(), adjacent) != closed.end();
			// if this tile is not already in the closed set then proceed (if it is in closed set then it has already been analysed and shortest path to it found)
			if (!in_closed_set) {
				// if the tile can be traversed by the unit then proceed
				if (CanTraverse(adjacent.get_tile())) {
					if (!in_open_set) { // if not in open set
										// if there is a unit on the tile..
						if (Game::instance().get_map().UnitPresent(adjacent.get_tile()->get_map_coords())) {
							// friendly unit -> can move through, non-friendly then cant
							if (team_ == Game::instance().get_map().GetUnit(adjacent.get_tile()->get_map_coords())->get_team()) {
								open.push_back(adjacent);
							}
						}
						// if no unit on the tile then just add to reachable tiles and to open set
						else {
							open.push_back(adjacent);
						}
					}
					else { // (in open set already)
						   // pointer to object already in open set
						MoveSequence* in_set = &*std::find(open.begin(), open.end(), adjacent);
						// if adjacent tile costs less to reach from parent as currently inspected tile then update the parent of tile already in set
						if (adjacent.get_score() < in_set->get_score()) {
							in_set->set_parent(*current);
						}
					}
				}
			}
		}
	}
	// we now have a move sequence which takes the unit from its current tile to the target tile...
	// make vector of tiles that is the order of moves to reach the final tile using the MoveSequence corresponding to the fnial tile
	std::vector<Tile*> tile_sequence;
	MoveSequence current = end_tile;
	while (current.get_parent()) {
		tile_sequence.push_back(current.get_tile());
		current = *current.get_parent();
	}
	std::reverse(tile_sequence.begin(), tile_sequence.end());

	// Animate the sequence of tiles as separate tile movements
	for (auto iter = tile_sequence.begin(); iter != tile_sequence.end(); iter++) {
		// do the movement animation for moving to the tile
		AnimateMovement(*iter);
		// update the units map coordinates to the new position
		set_map_coords((*iter)->get_map_coords());
	}
}