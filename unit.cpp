#include "unit.h"
#include "game_manager.h"
#include "game_instance.h"
#include "window.h"
#include "map.h"
#include "tile.h"
#include "move_sequence.h"

// ---------- ctor and dtor ---------- //

// ctor
Unit::Unit(const int &team): GameObject(), team_(team), state_(STATE_ASLEEP){ 
	if (team_ == 1) {
		default_colour_scheme_ = ColourScheme(DARK_BLUE, WHITE);
		highlighted_colour_scheme_ = ColourScheme(BLUE, WHITE);
		acted_colour_scheme_ = ColourScheme(DARK_BLUE, DARK_GREY);
		dead_colour_scheme_ = ColourScheme(BLACK, DARK_GREY);
	}
	if (team_ == 2){
		default_colour_scheme_ = ColourScheme(DARK_RED, WHITE);
		highlighted_colour_scheme_ = ColourScheme(RED, WHITE);
		acted_colour_scheme_ = ColourScheme(DARK_RED, DARK_GREY);
		dead_colour_scheme_ = ColourScheme(BLACK, DARK_GREY);
	}
}

// dtor
Unit::~Unit(){}

// ---------- accessors ---------- //

bool Unit::has_moved_this_turn() const { return moved_this_turn_; }
bool Unit::has_attacked_this_turn() const { return attacked_this_turn_; }
const int Unit::get_team() const { return team_; }

// ---------- internal (protected) functions ---------- //

// finds distance to target unit (manhattan distance)
const int Unit::DistanceTo(const Unit *target) const {
	return abs(map_coords_.x - target->map_coords_.x) + abs(map_coords_.y - target->map_coords_.y);
}

// returns a vector of all units that are currently attackable by this unit
const std::vector<Unit*> Unit::AttackableUnits() const {
	std::vector<Unit*> attackable_units; // vector to hold units this unit can currently attack
										 // get all the units on the map
	std::vector<Unit*> units = GameManager::game().get_instance().get_map().get_units();
	// iterate through the units and check if it can be attacked by this unit
	for (auto iter = units.begin(); iter != units.end(); iter++) {
		if (CanAttackTarget(*iter)) {
			attackable_units.push_back(*iter);
		}
	}
	return attackable_units;
}

// get tile unit is currently on
Tile* Unit::GetTile() const {
	return GameManager::game().get_instance().get_map().GetTile(map_coords_);
}

// returns a vector of pointers to the tiles that are reachable by a given unit (ignores the tile it is currently on)
std::vector<Tile*> Unit::ReachableTiles() const {
	std::vector<Tile*> can_reach; // tiles that the unit can reach
	std::list<MoveSequence> closed; // tiles that have already been inspected
	std::list<MoveSequence> open; // tiles that have yet to be inspected

								  // start with the tile the unit is currently on, add to the vector of open tiles
	MoveSequence initial_tile(GetTile()); // maybe need to make a copy assignment operator to use this
	open.push_back(initial_tile);

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
		adjacent_tiles = GameManager::game().get_instance().get_map().AdjacentTo(current->get_tile());
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
							if (GameManager::game().get_instance().get_map().UnitPresent(adjacent.get_tile()->get_map_coords())) {
								// friendly unit -> can move through but not end on. non-friendly then neither
								if (team_ == GameManager::game().get_instance().get_map().GetUnit(adjacent.get_tile()->get_map_coords())->get_team()) {
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

// animates the unit moving to an adjacent tile (does not move unit to the tile, just animates it)
void Unit::AnimateMovement(const Tile* start_tile, const Tile* target_tile) const {
	Window display = GameManager::game().get_display();
	Map map = GameManager::game().get_instance().get_map();
	// save the colour scheme to reset at end of function
	ColourScheme initial_colour_scheme = display.get_colour_scheme();
	// if target tile is in adjacent to tile unit is on then animate movement
	assert(start_tile->AdjacencyTest(target_tile));
	// get coordinate change of x and y 
	Coord delta = target_tile->get_map_coords() - start_tile->get_map_coords();
	int num_step;
	if (delta.x != 0) {
		num_step = map.get_tile_width();
	}
	else {
		num_step = map.get_tile_height();
	}
	// get initial console cursor location of the unit
	Coord initial_pos(map.get_map_x_offset() + start_tile->get_map_coords().x * map.get_tile_width() + (map.get_tile_width() / 2) - 1, // -1 as we take leftmost of the central tiles  
		map.get_map_y_offset() + start_tile->get_map_coords().y * map.get_tile_height() + (map.get_tile_height() / 2));
	Coord current_pos = initial_pos;
	for (int step = 0; step <= num_step; step++) {
		// got to the current position of the unit marker and replace the console cell with the tile marker using the tile colour scheme
		display.GoTo(current_pos); // got to current position
								   // render the tile at the current console cell positions (unit marker is two cells)
		for (int i = 0; i < 2; i++) {
			// get tile occupied by one of the unit marker chars
			Tile* tile = GameManager::game().get_instance().get_map().GetTileFromConsoleCoord(current_pos + Coord{ i,0 });
			// render it
			tile->Render();
			// if the tile contains a unit
			if (GameManager::game().get_instance().get_map().UnitPresent(tile->get_map_coords())) {
				// and the unit is not this unit
				if (GameManager::game().get_instance().get_map().GetUnit(tile->get_map_coords()) != this) {
					GameManager::game().get_instance().get_map().GetUnit(tile->get_map_coords())->Render();
				}
			}
		}
		// update the current console cell position
		current_pos = initial_pos + delta * step;
		// set cursor to new position, set colour scheme to unit colour scheme and output the units marker
		display.GoTo(current_pos);
		display.set_colour_scheme(get_colour_scheme());
		std::cout << unit_marker_;
		// sleep for a short time so can actually see the movement
		if (delta.x != 0) {
			Sleep(50);
		}
		else if (delta.y != 0) { // if moving in y direction sleep for twice as long because console cells are twice as high as they are wide
			Sleep(100);
		}
	}
	// set colour scheme back to what it was at start of function
	display.set_colour_scheme(initial_colour_scheme);
}

// ---------- public functions --------- //

// get a unique pointer to a copy of this object
std::unique_ptr<Unit> Unit::clone() const { return std::unique_ptr<Unit>(clone_impl()); }

// set hp (will set to 0 if < 0 and max_hp if > max_hp)
void Unit::set_current_hp(const int &hp) {  // will set to 0 if < 0 and max_hp if > max_hp
	// if setting hp to higher than max then just set to max
	if (hp > max_hp_) {
		current_hp_ = max_hp_;
	}
	// if setting hp below 0 then just set to 0 and kill the unit
	else if (hp <= 0) {
		current_hp_ = 0;
		Kill();
	}
	// otherwise just set the unit's hp to the input hp
	else {
		current_hp_ = hp;
	}
}

// get the appropriate colour scheme for the unit
const ColourScheme& Unit::get_colour_scheme() const {
	// if the unit is the currently seleceted unit then return the selected colour scheme
	switch (state_) {
		// if the unit is selcted return the selected colour scheme
		case STATE_SELECTED :
			return highlighted_colour_scheme_;
		// if the unit is idle then return the default
		case STATE_IDLE :
			return default_colour_scheme_;
		// if the unit is not selectable (asleep) then use acted colour scheme. For units on team whos not taking their go then return default
		case STATE_ASLEEP :
			if (GameManager::game().get_instance().get_player_turn() == team_) {
				return acted_colour_scheme_;
			}
			else {
				return default_colour_scheme_;
			}
		// if the unit is dead return the dead colour scheme
		case STATE_DEAD :
			return dead_colour_scheme_;
	}
	// if its state is invalid just return default colour scheme
	return default_colour_scheme_;
}

// move unit to a new coordinate
void Unit::set_map_coords(const Coord &new_pos) {
	// need to check if the new coordinate is valid ( on the game map )
	map_coords_ = new_pos;
}


// set the unit to be able to act (state -> idle)
void Unit::EnableActions() {
	moved_this_turn_ = false;
	attacked_this_turn_ = false;
	state_ = STATE_IDLE;
	Render();
}

void Unit::DisableActions() {
	moved_this_turn_ = true;
	attacked_this_turn_ = true;
	state_ = STATE_ASLEEP;
	Render();
}


// find out if this unit can be selected (if it has either moved or attacked already and if it is its team's turn)
const bool Unit::CanSelect() const {
	return state_ == STATE_IDLE;
}

// select this unit
void Unit::SelectUnit() {
	// update state
	state_ = STATE_SELECTED;
	// set as currently selected unit in game
	GameManager::game().get_instance().SelectUnit(this);
	// render to update colour scheme
	Render();
}

// deselect this unit
void Unit::DeselectUnit() {
	// if the unit has attacked or moved this turn then set its state to asleep
	if (moved_this_turn_ || attacked_this_turn_) {
		state_ = STATE_ASLEEP;
	}
	// otherwise set the state back to idle
	else {
		state_ = STATE_IDLE;
	}
	// render to update colour scheme
	Render();
}


// kill the unit
void Unit::Kill() {
	state_ = STATE_DEAD;
	// run a short animation to make death feel more weighty
	unit_marker_ = "XX";
	Render();
	Sleep(150);
	unit_marker_ = "xx";
	Render();
	Sleep(150);
	unit_marker_ = "..";
	Render();
	Sleep(100);
	unit_marker_ = "  ";
	Render();
	Sleep(50);
	// get tile under the unit to render after removing unit
	Tile* tile_on = GetTile();
	// now remove the unit from the game
	GameManager::game().get_instance().RemoveUnit(this);
	// render the tile that was under the unit
	tile_on->Render();
}


// function return true if unit can move
const bool Unit::CanMove() const {
	return ReachableTiles().size() > 0 && !moved_this_turn_;// && (state_ == STATE_IDLE || state_ == STATE_SELECTED);
}

// returns true if the unit can legally reach the target tile in one movement
const bool Unit::CanReach(const Tile* target_tile) {
	std::vector<Tile*> reachable_tiles = ReachableTiles(); // vector of tiles reachable by this unit
														   // iterates through the tiles, if still have not found after all in vector then this will return false
	return std::find(reachable_tiles.begin(), reachable_tiles.end(), target_tile) != reachable_tiles.end();
}

// function return true if unit can attack
const bool Unit::CanAttack() const {
	return AttackableUnits().size() > 0 && !attacked_this_turn_;// && (state_ == STATE_IDLE || state_ == STATE_SELECTED);
}

// check if a target unit is attackable by this unit
const bool Unit::CanAttackTarget(const Unit *target) const {
	// if distance to the target equal to or less than this unit's attack range and it is not on the same team then it can attack
	return DistanceTo(target) <= attack_range_ && target->get_team() != team_;
}


// function to attack a target
void Unit::Attack(Unit *target) {
	// if can attack then apply damage after applying terrain modifier
	if (CanAttackTarget(target)) {
		// set unit to has attacked
		attacked_this_turn_ = true;
		// target is attacked by this unit
		target->AttackedBy(this);
	}
}

// apply damage and healing
void Unit::AttackedBy(const Unit *attacker) {
	// get raw damage and terrain modifiers
	int raw_damage = attacker->attack_damage_;
	int defense_modifier = GameManager::game().get_instance().get_map().GetTile(map_coords_)->get_def_modifier(); // this unit's terrain defense modifiers
	int attack_modifier = GameManager::game().get_instance().get_map().GetTile(attacker->get_map_coords())->get_atk_modifier(); // attacking unit's terrain attack modifiers
																																// get this units effective armour:
	int effective_armour = armour_ - attacker->armour_pierce_;
	// if it is below 0 then set to 0
	if (effective_armour < 0) {
		effective_armour = 0;
	}
	// apply modifiers and armour to the raw damage
	int received_damage = raw_damage + attack_modifier - defense_modifier - effective_armour;
	// update this units hp
	set_current_hp(current_hp_ - received_damage);
	// if the unit is still alive then render the tile its on and its updated hp again
	// (if it dies the rendering is handled elsewhere anyway so this prevents unnecessary rendering
	if (state_ != STATE_DEAD) {
		GameManager::game().get_instance().get_map().Render(map_coords_);
	}
}


// highlights tiles/units that are attackable
void Unit::HighlightAttackableUnits(const bool &highlight) const {
	// get all the attackable units on the map
	std::vector<Unit*> units = AttackableUnits();
	// iterate through the units
	for (auto iter = units.begin(); iter != units.end(); iter++) {
		// highlight the tile the unit is on, render the tile and then render the unit on top of it
		(*iter)->GetTile()->set_highlighted(highlight);
		GameManager::game().get_instance().get_map().Render((*iter)->get_map_coords());
	}
}

// highlight the tiles that the unit can reach
void Unit::HighlightReachableTiles(const bool &highlight) const {
	std::vector<Tile*> reachable_tiles = ReachableTiles(); // vector of reachable tiles
	for (auto iter = reachable_tiles.begin(); iter != reachable_tiles.end(); iter++) { // iterate through
																					   // set to higlighted and render
		(*iter)->set_highlighted(highlight);
		GameManager::game().get_instance().get_map().Render((*iter)->get_map_coords()); // renders the tile and then the unit on it if there is one
	}
}


// Move to a target tile (will not work for an invalid tile or when there is no legal movement to the tile)
void Unit::MoveTo(Tile* target_tile) {
	// asser that the target tile is reachable by the unit
	assert(CanReach(target_tile));

	// mark this unit as having moved this turn
	moved_this_turn_ = true;

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
		// set pointer to the target MoveSequence object in the closed set. The set doesnt change when looping through neighbours so parent pointers will stay valid
		MoveSequence* current = &(*std::find(closed.begin(), closed.end(), *current_iter));
		// remove the currently inspected tile from the open set as it will now be analysed
		open.erase(current_iter);
		// clear the vector of adjacent tiles and fill for the current tile
		adjacent_tiles.clear();
		adjacent_tiles = GameManager::game().get_instance().get_map().AdjacentTo(current->get_tile());
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
						if (GameManager::game().get_instance().get_map().UnitPresent(adjacent.get_tile()->get_map_coords())) {
							// friendly unit -> can move through, non-friendly then cant
							if (team_ == GameManager::game().get_instance().get_map().GetUnit(adjacent.get_tile()->get_map_coords())->get_team()) {
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
	// we now have a move sequence which takes the unit from its current tile to the target tile.
	// make vector of tiles that is the order of moves to reach the final tile using the MoveSequence corresponding to the fnial tile
	std::vector<Tile*> tile_sequence;
	MoveSequence current = end_tile;
	while (current.get_parent()) { // will keep adding until we reach then initial tile which is not added
		tile_sequence.push_back(current.get_tile());
		current = *current.get_parent();
	}
	std::reverse(tile_sequence.begin(), tile_sequence.end()); // vector of tiles from intitial to target ( does not include the initial tile)

	// Animate the sequence of tiles as separate tile movements
	Tile* from_tile = GetTile(); // current tile moving from
	for (auto tile_iter = tile_sequence.begin(); tile_iter != tile_sequence.end(); tile_iter++) {
		Tile* to_tile = *tile_iter; // current tile moving to
		// do the movement animation for moving to the tile
		AnimateMovement(from_tile, to_tile);
		from_tile = to_tile; // next tile to move from is the tile just moved to
	}
	// set the units position to the target tile
	set_map_coords(target_tile->get_map_coords());
	// render the unit again to give hp and other additional info not shown during movement
	Render();
}

// renders the unit in the console window
void Unit::Render() const {
	Window display = GameManager::game().get_display();
	Map map = GameManager::game().get_instance().get_map();
	ColourScheme original_colour_scheme = display.get_colour_scheme(); // save original colour scheme to set back late
																	   // set colour scheme of unit
	display.set_colour_scheme(get_colour_scheme());
	// set console cursor position to central tile component
	display.GoTo(Coord(map.get_map_x_offset() + map_coords_.x * map.get_tile_width() + (map.get_tile_width() / 2) - 1, // -1 as want left of the two centre cells
		map.get_map_y_offset() + map_coords_.y * map.get_tile_height() + (map.get_tile_height() / 2)));
	// output the tile marker
	std::cout << unit_marker_;
	// set console cursor to bottom right of tile
	display.GoTo(Coord(map.get_map_x_offset() + map_coords_.x * map.get_tile_width() + map.get_tile_width() - 1,
		map.get_map_y_offset() + map_coords_.y * map.get_tile_height() + map.get_tile_height() - 1));
	// if hp is >= 10 then need 2 cells to display so start printing from 1 tile to left
	if (current_hp_ >= 10) {
		display.GoTo(display.CursorPosition() + Coord{ -1, 0 });
	}
	// print hp
	std::cout << current_hp_;
	// revert to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}
