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
	end_turn_button_menu_ = nullptr; // start with end turn button not showing
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
	running_ = false; // game initiates not running
}

Game::Game(Screen &display): display_(&display){
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	context_menu_ = nullptr; // context menu not showing
	end_turn_button_menu_ = nullptr; // start with end turn button not 
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
	running_ = false; // game initiates not running
}

// dtor : NEED TO SET THIS UP PROPERLY
Game::~Game(){
}

// static game instance, globally accessible
Game Game::instance_;

// select the target unit
void Game::SelectUnit(Unit *unit) {
	// deselect any currently selected unit
	DeselectUnit();
	// update selected unit and show its context menu
	selected_unit_ = unit;
	ShowContextMenu();
}

// deselect the currently selected unit
void Game::DeselectUnit() {
	// get copy of pointer to selected unit
	Unit* selected_unit_save = selected_unit_;
	// set selected unit as nullptr and remove the context menu
	selected_unit_ = nullptr;
	RemoveContextMenu();
	// if there was a unit selected then run its deselection procedure ( do this after unsetting as the selected unit so it renders with this knowledge)
	if (selected_unit_save) {
		selected_unit_save->DeselectUnit();
	}
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

// add a unit to the game
void Game::AddUnit(Unit *new_unit, const Coord &pos) {
	game_map_->AddUnit(new_unit, pos);
	if (new_unit->get_team() == 1) {
		p1_units_.push_back(new_unit);
	}
	else if (new_unit->get_team() == 2) {
		p2_units_.push_back(new_unit);
	}
}
// remove a unit from the game and map
void Game::RemoveUnit(Unit *unit) {
	// use the Erase-Remove idiom to delete the unit from whichever array it is in
	p1_units_.erase(std::remove(p1_units_.begin(), p1_units_.end(), unit), p1_units_.end());
	p2_units_.erase(std::remove(p2_units_.begin(), p2_units_.end(), unit), p2_units_.end());
	game_map_->RemoveUnit(unit);
}

// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
void Game::ShowContextMenu() {
	// if there is already a context menu then first remove that
	if (context_menu_) {
		RemoveContextMenu();
	}
	// make a new context_menu_
	context_menu_ = new Menu();

	// Buttons

	// movement button (player can then choose a move target)
	Button move_button("Move", []() {Game::instance().get_selected_unit().ChooseMovement(); });
	if (!selected_unit_->CanMove()) { move_button.set_enabled(false); }
	context_menu_->AddButton(move_button);
	
	// attack button (player can then choose attack target)
	Button attack_button("Attack", []() {Game::instance().get_selected_unit().ChooseAttack(); });
	if (!selected_unit_->CanAttack()) { attack_button.set_enabled(false); }
	context_menu_->AddButton(attack_button);
	
	// pass / cancel button
	std::string end_cancel_button_name; // name to written on button
	// if the unit has yet to attack or move then the button is named cancel
	if (!selected_unit_->has_attacked_this_turn() && !selected_unit_->has_moved_this_turn()) { end_cancel_button_name = "Cancel"; }
	// if the unit has either moved or attacked already then the button is named pass
	else { end_cancel_button_name = "End"; }
	Button end_cancel_button(end_cancel_button_name, []() {Game::instance().DeselectUnit(); });
	context_menu_->AddButton(end_cancel_button);

	// if selected unit is in any but the last two map columns then set menu position as to right of unit
	Coord top_left_of_cell_to_right = Coord(display_->get_map_x_offset() + (selected_unit_->get_map_coords().x + 1) * display_->get_tile_width(),
		display_->get_map_y_offset() + selected_unit_->get_map_coords().y * display_->get_tile_height());
	if (selected_unit_->get_map_coords().x < game_map_->get_map_width() - 2) {
		context_menu_->set_location(top_left_of_cell_to_right);
	}
	else {
		context_menu_->set_location(top_left_of_cell_to_right - Coord{ display_->get_tile_width() + context_menu_->get_width(), 0}); // go left 1 tile width + the width of the menu
	}

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
		// if the context menu extended off to the right of the map then repaint the player turn indicator and the end turn button in case they were covered
		if (context_menu_->get_location().x + context_menu_->get_width()  // rightmost extent of the menu
					> display_->get_map_x_offset() + display_->get_tile_width() * game_map_->get_map_width()) { // rightmost extent of the map
			ShowTurnLabel();
			ShowEndTurnButton();
		}

		// if the context menu is covering the game map at all then re-render the tiles and units that it covers at all
		if (game_map_->GetTileFromConsoleCoord(context_menu_->get_location())) { // "screen_location" of menu is the top left cell and it expands right and down
			// get top-left-most tile covered by menu
			Coord top_left = game_map_->GetTileFromConsoleCoord(context_menu_->get_location())->get_map_coords();
			// for all tiles that are covered by any amount by the context menu...
			for (int i = 0; i < context_menu_->get_width() / display_->get_tile_width() + 1; i++) { // the width of the menu / width of the tile gives number of tiles that the menu covers horizontally (rounded down, hence add 1)
				for (int j = 0; j <= context_menu_->get_height() / display_->get_tile_height() + 1; j++) { // the height of the menu / height of the tile gives number of tiles that the menu covers vertically (rounded down, hence add 1)
					// render the tile and if there is a unit present on the tile, render that too
					game_map_->Render(top_left + Coord{ i,j });
				}
			}
		}
		// remove the context menu
		delete context_menu_; // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS BY LEAVING OLD CONTEXT MENU BUTTONS IN MEMORY!
		context_menu_ = nullptr; 
	}
}

// carries out the end of turn and begins a new turn
void Game::EndTurn() {
	// deselect any currently selected unit
	DeselectUnit();
	// change the player turn variable
	if (player_turn_ == 1) {
		player_turn_ = 2;
	}
	else if (player_turn_ == 2) {
		player_turn_ = 1;
	}
	// reset the moved and attacked flags for all units so that their colour schemes go back to default
	EnableUnits(p1_units_);
	EnableUnits(p2_units_);
	// update the label indicating whos turn it is
	ShowTurnLabel();
}

// checks if all the units on the current team have acted and ends turn if they have
void Game::AutoEndTurn() {
	// get vector of units of team whos turn it is
	std::vector<Unit*> current_team;
	if (player_turn_ == 1) { current_team = p1_units_; }
	else if (player_turn_ == 2) { current_team = p2_units_; }
	// iterate through the units on that team and see if any can be selected or are the currently selected unit
	bool all_acted = true;
	for (auto unit_iter = current_team.begin(); unit_iter != current_team.end(); unit_iter++) {
		if ((*unit_iter)->CanSelect() || selected_unit_ == *unit_iter) { all_acted = false; }
	}
	// if no units can be selected then end the turn
	if (all_acted) { EndTurn(); }
}

// activates a vector of units (resets their has moved or attacked this turn booleans)
void Game::EnableUnits(std::vector<Unit*> units) const {
	// iterate through the units and enable their actions
	for (auto unit_iter = units.begin(); unit_iter != units.end(); unit_iter++) {
		(*unit_iter)->EnableActions();
	}
}

// show / update the label for who's turn it currently is
void Game::ShowTurnLabel() {
	// save the current colour scheme to revert back after
	int original_colour_scheme = display_->get_colour_scheme();
	// go to top right next to map
	display_->GoTo(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * game_map_->get_map_width() + 1,1 });
	// if player 1s turn set colour scheme to blue, player 2s turn then red
	if (player_turn_ == 1) {
		display_->set_colour_scheme(159);
	}
	else if (player_turn_ == 2) {
		display_->set_colour_scheme(79);
	}
	// print out message saying whos turn it is
	std::cout << "Player " << player_turn_ << "'s turn";
	// revert back to original colour scheme
	display_->set_colour_scheme(original_colour_scheme);
}

// add / redo end turn button
void Game::ShowEndTurnButton() {
	// if there is already a context menu then first remove that
	//if (end_turn_button_menu_) {
	//	delete end_turn_button_menu_;
	//}
	// make a new menu to hold the button
	end_turn_button_menu_ = new Menu();
	// meny position is top right, next to map but below the current players turn label
	end_turn_button_menu_->set_location(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * game_map_->get_map_width() + 1, 3 });

	// add button
	Button end_turn_button("End turn", []() {Game::instance().EndTurn(); });
	end_turn_button_menu_->AddButton(end_turn_button);

	// render the menu
	end_turn_button_menu_->Render();

}

// show quit game button
void Game::ShowQuitGameButton() {}


// handles a mouse down event (i.e. the user clicking somewhere on the screen)
void Game::HandleLeftMouseButtonDown(const Coord &screen_location) { // Coord screen_location is in terms of console cells over whole display
	// check for no mouse click returns a Coord of {-1, -1}
	if (screen_location.x == -1 && screen_location.y == -1) {
		// throw error as should not be able to get here
		exit(1);
	}
	// get pointers to the tile and unit at the click location. Will be nullptr if there isnt one at location (or clicked off of map)
	Tile* tile = game_map_->GetTileFromConsoleCoord(screen_location);
	Unit* unit = nullptr;
	if (tile) {
		unit = game_map_->GetUnit(tile->get_map_coords()); // returns nullptr if no unit present
	}

	// ---------- handle end turn and quit buttons ----------

	if (end_turn_button_menu_) {
		if (end_turn_button_menu_->Contains(screen_location)) {
			end_turn_button_menu_->HandleLeftMouseButtonDown(screen_location);
			return;
		}
	}

	// ---------- handle context menu ----------

	// if the context menu is up then only clicking on the menu will do something (other than remove the context menu)
	if (context_menu_) {
		if (context_menu_->Contains(screen_location)) {
			context_menu_->HandleLeftMouseButtonDown(screen_location);
		}
		// if they didnt click on the menu... but the selected unit has yet to move or attack (or use any ability) then just remove the menu and they can click on something else
		else{
			// if there is a tile at the screen_location of the click (i.e. they clicked on the map) and the currently selected unit has yet to attack or move
			if (tile && !selected_unit_->has_moved_this_turn() && !selected_unit_->has_attacked_this_turn() ) {
				// deselect the unit
				DeselectUnit();
				// if there is also a unit present on that tile then select that unit if it can be selected
				if (unit){
					if (unit->CanSelect()) {
						unit->SelectUnit();
					}
				}
			}
		}
		// if they didnt click on the menu but the selected unit either has already moved or attacked then they must select an option on the menu, so do nothing
	}

	// if the context menu is not up
	else {

	// ---------- handle selected unit actions ----------

		// if there is currently a unit selected but the context menu is not up...
		if (selected_unit_) {
			// ---------- handle movement selection ----------
			// if the unit is currently selecting a tile to move to...
			if (selected_unit_->is_selecting_movement()) {
				// .. and the screen_location of the click is a tile (i.e. on the map) ...
				if (tile) { //tile == nullptr if no tile at screen_location of click
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
			// ---------- handle attack selection ----------
			// if the currently selected unit is selecting an attack target ...
			else if (selected_unit_->is_selecting_attack()) {
				// ... and the screen_location of the click is a unit ...
				if (unit) {
					// ... and the currently selected unit can attack this target unit...
					if (selected_unit_->CanAttackTarget(unit)) {
						selected_unit_->Attack(unit);
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
			// ... and the screen_location of the click is a unit ...
			if (unit) {
				// select the unit if it can be selected
				if (unit->CanSelect()) {
					unit->SelectUnit();
				}
			} // if there is no unit on the tile do nothing
		}
	}
}

// run the main game loop until the user quits
void Game::Run() {
	running_ = true; // will set to false if user clicks quit button or somone wins
	while (running_) {
		// get location of mouse click
		Coord mouse_down_pos = Game::instance().get_display().MouseDownPosition();
		// if mouse click was on the screen then handle it
		if (mouse_down_pos.x != -1 && mouse_down_pos.y != -1) {
			Game::instance().HandleLeftMouseButtonDown(mouse_down_pos);
			// if all the units on the team whos go it is have now acted then end the turn
			AutoEndTurn();
		}
	}
}