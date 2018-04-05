#include "game.h"
#include "screen.h"
#include "map.h"
#include "tile.h"
#include "unit.h"
#include "menu.h"
#include "button.h"

Game::Game() {
	display_ = new Screen(700, 700); // default screen size 700x700
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	context_menu_ = nullptr; // context menu not showing
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
}

Game::Game(Screen &display): display_(&display){
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	context_menu_ = nullptr; // context menu not showing
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
}

// dtor : NEED TO SET THIS UP PROPERLY
Game::~Game(){
}

// static game instance, globally accessible
Game Game::instance_;

// select the target unit
void Game::SelectUnit(Unit *unit) {
	DeselectUnit();
	selected_unit_ = unit;
	ShowContextMenu();
}

// deselect the currently selected unit
void Game::DeselectUnit() {
	if (selected_unit_) {
		selected_unit_->DeselectUnit();
	}
	selected_unit_ = nullptr;
}

// load in a map from the 5 premade maps
void Game::LoadMap(const int &map_id) const {
	// maps identified by an integer id
	if (map_id == 1) {
		game_map_->LoadMap(raw_map1);
	}
	else if (map_id == 2) {
		game_map_->LoadMap(raw_map2);
	}
	else if (map_id == 3) {
		game_map_->LoadMap(raw_map3);
	}
	else if (map_id == 4) {
		game_map_->LoadMap(raw_map4);
	}
	else if (map_id == 5) {
		game_map_->LoadMap(raw_map5);
	}
	else {
		exit(1);
		//invalid map id, throw error...
	}
}
// update the map
void Game::RenderMap() const {
	game_map_->Render();
}

// add a unit to the map
void Game::AddUnit(Unit *unit, const Coord &pos) {
	game_map_->AddUnit(unit, pos);
	if (unit->get_team() == 1) {
		p1_units_.push_back(unit);
	}
	else if (unit->get_team() == 2) {
		p2_units_.push_back(unit);
	}
}

// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
void Game::ShowContextMenu() {
	// if there is already a context menu then first remove that
	if (context_menu_) {
		RemoveContextMenu();
	}
	// menu position is the top left of tile adjacent to the selected unit
	Coord position = Coord(display_->get_map_x_offset() + (selected_unit_->get_map_coords().x + 1) * display_->get_tile_width(),
		display_->get_map_y_offset() + selected_unit_->get_map_coords().y * display_->get_tile_height());
	// make a new context_menu_
	context_menu_ = new Menu(position);

	// Buttons

	// movement button (player can then choose a move target)
	Button move_button(Coord{ 0,0 }, "Move", []() {Game::instance().RemoveContextMenu(); Game::instance().get_selected_unit().ChooseMovement(); });
	if (!selected_unit_->CanMove()) { move_button.set_enabled(false); }
	context_menu_->AddButton(move_button);
	
	// attack button (player can then choose attack target)
	Button attack_button(Coord{ 0,0 }, "Attack", []() {Game::instance().RemoveContextMenu(); Game::instance().get_selected_unit().ChooseAttack(); });
	if (!selected_unit_->CanAttack()) { attack_button.set_enabled(false); }
	context_menu_->AddButton(attack_button);
	
	// pass / cancel button
	std::string end_cancel_button_name; // name to written on button
	// if the unit has yet to attack or move then the button is named cancel
	if (!selected_unit_->has_attacked_this_turn() && !selected_unit_->has_moved_this_turn()) { end_cancel_button_name = "Cancel"; }
	// if the unit has either moved or attacked already then the button is named pass
	else { end_cancel_button_name = "End"; }
	Button end_cancel_button(Coord{ 0,0 }, end_cancel_button_name, []() {Game::instance().RemoveContextMenu(); Game::instance().DeselectUnit(); });
	context_menu_->AddButton(end_cancel_button);

	// render the menu
	context_menu_->Render();
}
// removes the context menu and re renders any tiles and units it was covering
void Game::RemoveContextMenu() {
	// only try and remove if there is currently a context menu
	if (context_menu_) {
		// fill the are occupied by the context menu with blank cells (this will mean any area which never otherwise has anything ouput on it will get reset back to being empty)
		for (int i = 0; i < context_menu_->get_width(); i++) {
			for (int j = 0; j < context_menu_->get_height(); j++) {
				display_->GoTo(context_menu_->get_location() + Coord{ i,j });
				std::cout << ' ';
			}
		}
		// if the context menu is covering the game map at all then re-render the tiles and units that it covers at all
		if (game_map_->GetTileFromConsoleCoord(context_menu_->get_location())) { // "location" of menu is the top left cell and it expands right and down
			// get top-left-most tile covered by menu
			Coord top_left = game_map_->GetTileFromConsoleCoord(context_menu_->get_location())->get_map_coords();
			// for all tiles that are covered by any amount by the context menu...
			for (int i = 0; i < context_menu_->get_width() / display_->get_tile_width() + 1; i++) { // the width of the menu / width of the tile gives number of tiles that the menu covers horizontally (rounded down, hence add 1)
				for (int j = 0; j <= context_menu_->get_height() / display_->get_tile_height() + 1; j++) { // the height of the menu / height of the tile gives number of tiles that the menu covers vertically (rounded down, hence add 1)
					// render the tile and if there is a unit present on the tile, render that too
					game_map_->GetTile(top_left + Coord{ i,j })->Render();
					if (game_map_->UnitPresent(top_left + Coord{ i,j })) {
						game_map_->GetUnit(top_left + Coord{ i,j })->Render();
					}
				}
			}
		}
		// remove the context menu
		delete context_menu_; // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS BY LEAVING OLD CONTEXT MENU BUTTONS IN MEMORY!
		context_menu_ = nullptr; 
	}
}

// handles a mouse down event (i.e. the user clicking somewhere on the screen)
void Game::HandleLeftMouseButtonDown(const Coord &location) { // Coord location is in terms of console cells over whole display
	// check for no mouse click returns a Coord of {-1, -1}
	if (location.x == -1 && location.y == -1) {
		// throw error as should not be able to get here
		exit(1);
	}
	// if the context menu is up then only clicking on the menu will do something (other than remove the context menu)
	if (context_menu_) {
		if (context_menu_->Contains(location)) {
			context_menu_->HandleLeftMouseButtonDown(location);
		}
		// if they didnt click on the menu but the selected unit has yet to move or attack (or use any ability) then just remove the menu and they can click on something else
		else if (!context_menu_->Contains(location) && !selected_unit_->has_moved_this_turn() && !selected_unit_->has_attacked_this_turn()) {
			RemoveContextMenu();
			DeselectUnit();
		}
		// if they didnt click on the menu but the selected unit either has already moved or attacked then they must select an option on the menu, so do nothing
	}
	// if the context menu is not up
	else {
		// get tile at the location of the click (will be nullptr if not clicked on the map)
		Tile* tile = game_map_->GetTileFromConsoleCoord(location);
		// if there is currently a unit selected but the context menu is not up...
		if (selected_unit_) {
			// if the unit is currently selecting a tile to move to...
			if (selected_unit_->is_selecting_movement()) {
				// .. and the location of the click is a tile (i.e. on the map) ...
				if (tile) { //tile == nullptr if no tile at location of click
					//  ... and the unit can reach the tile
					if (selected_unit_->CanReach(tile)) {
						// move the currently selected unit to the tile
						selected_unit_->MoveTo(tile);
					}
					// if it cant reach the tile
					else {
						// set to not selecting movement and bring up the menu again
						selected_unit_->UnChooseMovement();
						ShowContextMenu();
					}
				} // if click not on the map then nothing happens
			}
			// if the currently selected unit is selecting an attack target ...
			else if (selected_unit_->is_selecting_attack()) {
				// ... and the location of the click is a tile (i.e. on the map) ...
				if (tile) {
					// ... and there is a unit on the selected tile ...
					if (game_map_->UnitPresent(tile->get_map_coords())) {
						Unit* target_unit = game_map_->GetUnit(tile->get_map_coords()); // unit on the tile
						// ... and the currently selected unit can attack this target unit...
						if (selected_unit_->CanAttackTarget(target_unit)) {
							selected_unit_->Attack(target_unit);
						}
						// if it cant attack the target
						else {
							// set to not selecting attack and bring up the options menu again
							selected_unit_->UnChooseAttack();
							ShowContextMenu();
						}
					}
					// if there is no unit on the tile
					else {
						// set to not selecting attack and bring up the options menu again
						selected_unit_->UnChooseAttack();
						ShowContextMenu();
					}
				} // if click not on the map then nothing happens
			}
			// if the unit has either attacked or moved but not both (XOR) then show the menu again
			if (selected_unit_->has_attacked_this_turn() != selected_unit_->has_moved_this_turn()) {
				ShowContextMenu();
			}
			// if the unit has now both attacked and moved then deselect it
			else if (selected_unit_->has_attacked_this_turn() && selected_unit_->has_moved_this_turn()) {
				DeselectUnit();
			}
		}
		// if there is no currently selected unit...
		else {
			// ... and the location of the click is a tile (i.e. on the map) ...
			if (tile) {
				// ... and the tile contains a unit ...
				if (Game::instance().get_map().UnitPresent(tile->get_map_coords())) {
					// ... get the unit and select it
					Unit * target_unit = Game::instance().get_map().GetUnit(tile->get_map_coords());
					// select the unit if it has yet to act on this turn
					if (!target_unit->has_moved_this_turn() && !target_unit->has_attacked_this_turn()) {
						target_unit->SelectUnit();
					}
				} // if there is no unit on the tile do nothing
			} // if the click occurs off the map then do nothing
		}
	}
}
	/*
	// get the tile at the location (if any), will handle unit later
	Tile* tile = game_map_->GetTileFromConsoleCoord(location);
	// if a tile was selected... (i.e. the click was on the map)
	if (tile) {
		// get unit on tile if any ( will be nullptr if no unit)
		Unit* unit = game_map_->GetUnit(tile->get_map_coords());
		// ... and there is a unit on the tile...
		if (unit){
			// ... and there is no unit currently selected...
			if (!selected_unit_) {
				unit->SelectUnit();
			}
			else {
				// ...and the currently selected unit is trying to attack..
				// if (selected_unit->currently_attacking) {
					// and can attack that unit..
					//if ( selected_unit_->has_attacked_this_turn(unit) ) {
						// then handle the attack and deselect the unit as its go is now over
					//}
					// ... but cannot attack that unit
					//else{
						// either do nothing or remove the attacking status of the currently selected unit
				// .. and the currently selected unit is trying to move
				// if (selected_unit->currently_moving){
					// then deselect the currently selected unit
					selected_unit_->DeselectUnit();
					unit->SelectUnit();
			}
		}
		// ...and there is no unit on the tile...
		else{
			// ... and there is a unit currently selected..
			if (selected_unit_) {
				// ...and the currently selected unit can reach the tile and has yet to move..
				if (selected_unit_->CanReach(tile) && selected_unit_->is_selecting_movement() && !unit) {
					// move the currently selected unit to the tile
					selected_unit_->MoveTo(tile);
					// if the unit has yet to attack then show the context menu after moving
					if (selected_unit_->has_attacked_this_turn()) {
						//ShowContextMenu();
					}
				}
				// ... and the currently selected unit either cant move or cant reach the tile
				else {
					// deselect the currently selected unit
					selected_unit_->DeselectUnit();
				}
			}
		}
		
	}
	// if the mouse click did not occur on the map...
	else {

	}
}
*/
