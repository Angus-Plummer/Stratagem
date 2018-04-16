#include "game_instance.h"
#include "game_manager.h"
#include "window.h"
#include "map.h"
#include "tile.h"
#include "unit.h"
#include "menu.h"
#include "button.h"
#include "ascii_art.h"

GameInstance::GameInstance() {
	display_ = new Window(610, 560); // default window size 610x560 ( just fits everything on)
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	context_menu_ = Menu(); // context menu not showing
	end_turn_button_ = Button(); // start with end turn button not showing
	surrender_button_ = Button(); // start with no surrender button showing
	player_turn_ = 1; // start with player 1's turn
	running_ = false; // game initiates not running
	state_ = STATE_SETUP; // set initial state is setup 
}

GameInstance::GameInstance(Window &display): display_(&display){
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	context_menu_ = Menu(); // context menu not showing
	end_turn_button_ = Button(); // start with end turn button not 
	surrender_button_ = Button(); // start with no surrender button showing
	player_turn_ = 1; // start with player 1's turn
	running_ = false; // game initiates not running
	state_ = STATE_SETUP; // set initial state is setup 
}

// dtor : NEED TO SET THIS UP PROPERLY
GameInstance::~GameInstance(){
	delete game_map_;
}

// select the target unit
void GameInstance::SelectUnit(Unit *unit) {
	// deselect any currently selected unit
	DeselectUnit();
	// update selected unit, state, and show the context menu
	selected_unit_ = unit;
	state_ = STATE_UNIT_SELECTING_ACTION;
	ShowContextMenu();
}

// deselect the currently selected unit
void GameInstance::DeselectUnit() {
	// deselect any currently selected unit
	if (selected_unit_) {
		selected_unit_->DeselectUnit();
	}
	// set selected unit as nullptr, update state, and remove the context menu
	selected_unit_ = nullptr;
	state_ = STATE_SELECTING_UNIT;
	RemoveContextMenu();	
}

// load in a map from the 5 premade maps
void GameInstance::LoadMap(const int &map_id) const {
	assert(map_id >= 1 && map_id <=5);
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
}
// update the map
void GameInstance::RenderMap() const {
	game_map_->Render();
}

// add a unit to the game
void GameInstance::AddUnit(Unit *new_unit, const Coord &pos) {
	game_map_->AddUnit(new_unit, pos);
	if (new_unit->get_team() == 1) {
		p1_units_.push_back(new_unit);
	}
	else if (new_unit->get_team() == 2) {
		p2_units_.push_back(new_unit);
	}
}
// remove a unit from the game and map
void GameInstance::RemoveUnit(Unit *unit) {
	// use the Erase-Remove idiom to delete the unit from whichever array it is in
	p1_units_.erase(std::remove(p1_units_.begin(), p1_units_.end(), unit), p1_units_.end());
	p2_units_.erase(std::remove(p2_units_.begin(), p2_units_.end(), unit), p2_units_.end());
	game_map_->RemoveUnit(unit);

	// if after removing the unit there are not units left on the team from then the other team wins (if the game is not currently in setup where this map happen during unit placement)
	if (state_ != STATE_SETUP && state_ != STATE_VICTORY) {
		if (p1_units_.size() == 0) {
			Victory(2);
		}
		else if (p2_units_.size() == 0) {
			Victory(1);
		}
	}
}

// handles entering the UNIT_SELECTING_ATTACK state
void GameInstance::ChooseAttack() {
	RemoveContextMenu();
	state_ = STATE_UNIT_SELECTING_ATTACK;
	selected_unit_->HighlightAttackableUnits(true);
}
// handles leaving the UNIT_SELECTING_ATTACK state
void GameInstance::UnChooseAttack() {
	state_ = STATE_UNIT_SELECTING_ACTION;
	selected_unit_->HighlightAttackableUnits(false);
	ShowContextMenu();
}

// handles entering the UNIT_SELECTING_MOVEMENT state
void GameInstance::ChooseMovement() {
	RemoveContextMenu();
	state_ = STATE_UNIT_SELECTING_MOVEMENT;
	selected_unit_->HighlightReachableTiles(true);
}
// handles leaving the UNIT_SELECTING_MOVEMENT state
void GameInstance::UnChooseMovement() {
	state_ = STATE_UNIT_SELECTING_ACTION;
	selected_unit_->HighlightReachableTiles(false);
	ShowContextMenu();
}

// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
void GameInstance::ShowContextMenu() {
	// clear the current buttons from the menu
	context_menu_.Clear();

	// Buttons

	// movement button (player can then choose a move target)
	Button move_button("Move", []() {GameManager::game().get_instance().ChooseMovement(); });
	if (!selected_unit_->CanMove()) { move_button.set_enabled(false); }
	context_menu_.AddButton(move_button);
	
	// attack button (player can then choose attack target)
	Button attack_button("Attack", []() {GameManager::game().get_instance().ChooseAttack(); });
	if (!selected_unit_->CanAttack()) { attack_button.set_enabled(false); }
	context_menu_.AddButton(attack_button);
	
	// pass / cancel button
	std::string end_cancel_button_name; // name to written on button
	// if the unit has yet to attack or move then the button is named cancel
	if (!selected_unit_->has_attacked_this_turn() && !selected_unit_->has_moved_this_turn()) { end_cancel_button_name = "Cancel"; }
	// if the unit has either moved or attacked already then the button is named pass
	else { end_cancel_button_name = "End"; }
	Button end_cancel_button(end_cancel_button_name, []() {GameManager::game().get_instance().DeselectUnit(); });
	context_menu_.AddButton(end_cancel_button);

	// if selected unit is in any but the last two map columns then set menu position as to right of unit
	Coord top_left_of_cell_to_right = Coord(display_->get_map_x_offset() + (selected_unit_->get_map_coords().x + 1) * display_->get_tile_width(),
		display_->get_map_y_offset() + selected_unit_->get_map_coords().y * display_->get_tile_height());
	if (selected_unit_->get_map_coords().x < game_map_->get_map_width() - 2) {
		context_menu_.set_location(top_left_of_cell_to_right);
	}
	else {
		context_menu_.set_location(top_left_of_cell_to_right - Coord{ display_->get_tile_width() + context_menu_.get_width(), 0}); // go left 1 tile width + the width of the menu
	}

	// render the menu
	context_menu_.Render();
}
// removes the context menu and re renders any tiles and units it was covering
void GameInstance::RemoveContextMenu() {
	// fill the are occupied by the context menu with blank cells (this will mean any area which never otherwise has anything ouput on it will get reset back to being empty)
	for (int i = 0; i < context_menu_.get_width(); i++) {
		for (int j = 0; j < context_menu_.get_height(); j++) {
			display_->GoTo(context_menu_.get_location() + Coord{ i,j });
			std::cout << ' ';
		}
	}

	// if the context menu is covering the game map at all then re-render the tiles and units that it covers (renders each tile once instead of rendering a tile for each cell of the context menu)
	if (game_map_->GetTileFromConsoleCoord(context_menu_.get_location())) { // "window_location" of menu is the top left cell and it expands right and down
		// get top-left-most tile covered by menu
		Coord top_left = game_map_->GetTileFromConsoleCoord(context_menu_.get_location())->get_map_coords();
		// for all tiles that are covered by any amount by the context menu...
		for (int i = 0; i < context_menu_.get_width() / display_->get_tile_width() + 1; i++) { // the width of the menu / width of the tile gives number of tiles that the menu covers horizontally (rounded down, hence add 1)
			for (int j = 0; j < context_menu_.get_height() / display_->get_tile_height() + 1; j++) { // the height of the menu / height of the tile gives number of tiles that the menu covers vertically (rounded down, hence add 1)
				// if there is a tile at this coordinate then render the tile, if not then there is nothing to render
				if (game_map_->GetTile(top_left + Coord{ i,j })) {
					// render the tile (will also render any unit on the tile
					game_map_->Render(top_left + Coord{ i,j });
				}
			}
		}
	}
	// clear the context menu
	context_menu_.Clear();
}

// carries out the beginning of a turn
void GameInstance::StartTurn() {
	// clear the console and render the game map again, show the end turn button and end turn label
	display_->Clear();
	game_map_->Render();
	ShowTurnLabel();
	ShowEndTurnButton();
	ShowSurrenderButton();

	// enable the units of the player whose turn it is
	if (player_turn_ == 1) {
		EnableUnits(p1_units_);
	}
	else if (player_turn_ == 2) {
		EnableUnits(p2_units_);
	}
	// set game state to selecting a unit
	state_ = STATE_SELECTING_UNIT;
}

// carries out the end of turn and begins a new turn
void GameInstance::EndTurn() {
	// deselect any currently selected unit (shouldnt be one but just in case)
	DeselectUnit();
	// set state state to being between turns
	state_ = STATE_BETWEEN_TURNS;
	// change the player turn variable
	if (player_turn_ == 1) {
		DisableUnits(p1_units_);
		player_turn_ = 2;
	}
	else if (player_turn_ == 2) {
		DisableUnits(p2_units_);
		player_turn_ = 1;
	}
	// run an animation to show turn change (as easy to miss the current turn label)
	ShowTurnChangeScreen();
	// wait for mouse click and then start the next turn
	display_->WaitForMouse();
	StartTurn();
}

// checks if all the units on the current team have acted and ends turn if they have
void GameInstance::AutoEndTurn() {
	// only end turn if the current state is that of selecting a unit (i.e no unit is currently selected)
	if (state_ == STATE_SELECTING_UNIT) {
		// get vector of units of team whos turn it is
		std::vector<Unit*> current_team;
		if (player_turn_ == 1) { current_team = p1_units_; }
		else if (player_turn_ == 2) { current_team = p2_units_; }
		// iterate through the units on that team and see if any can be selected or are the currently selected unit
		bool all_acted = true;
		for (auto unit_iter = current_team.begin(); unit_iter != current_team.end(); unit_iter++) {
			if ((*unit_iter)->CanSelect()) { all_acted = false; }
		}
		// if no units can be selected then end the turn
		if (all_acted) { EndTurn(); }
	}
}

// activates a vector of units (resets their has moved or attacked this turn booleans)
void GameInstance::EnableUnits(std::vector<Unit*> units) const {
	// iterate through the units and enable their actions
	for (auto unit_iter = units.begin(); unit_iter != units.end(); unit_iter++) {
		(*unit_iter)->EnableActions();
	}
}

// deactivates a vector of units (sets their has moved or attacked this turn booleans)
void GameInstance::DisableUnits(std::vector<Unit*> units) const {
	// iterate through the units and enable their actions
	for (auto unit_iter = units.begin(); unit_iter != units.end(); unit_iter++) {
		(*unit_iter)->DisableActions();
	}
}

// show / update the label for who's turn it currently is
void GameInstance::ShowTurnLabel() {
	// save the current colour scheme to revert back after
	ColourScheme original_colour_scheme = display_->get_colour_scheme();
	// go to top right next to map
	display_->GoTo(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * game_map_->get_map_width() + 1,1 });
	// if player 1s turn set colour scheme to blue, player 2s turn then red
	if (player_turn_ == 1) {
		display_->set_colour_scheme(ColourScheme(DARK_BLUE, WHITE));
	}
	else if (player_turn_ == 2) {
		display_->set_colour_scheme(ColourScheme(DARK_RED, WHITE));
	}
	// print out message saying whos turn it is
	std::cout << " Player " << player_turn_ << "'s turn ";
	// revert back to original colour scheme
	display_->set_colour_scheme(original_colour_scheme);
}

// add / redo end turn button
void GameInstance::ShowEndTurnButton() {
	// add button
	end_turn_button_ =  Button(" End turn ", []() {GameManager::game().get_instance().EndTurn(); });

	// menu position is top right, next to map but below the current players turn label
	end_turn_button_.set_location(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * game_map_->get_map_width() + 1, 3 });

	// render the menu
	end_turn_button_.Render();
}

// show quit game button
void GameInstance::ShowSurrenderButton() {

	// add button
	surrender_button_ = Button(" Surrender ", []() { 
		// lambda function runs the victory function for the other player
		if (GameManager::game().get_instance().get_player_turn() == 1) { GameManager::game().get_instance().Victory(2);}
		else { GameManager::game().get_instance().Victory(1); }
	});
	
	// menu position is bottom right, next to map, far from other buttons
	surrender_button_.set_location(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * game_map_->get_map_width() + 1,
		display_->get_map_y_offset() + display_->get_tile_height() * game_map_->get_map_height() - surrender_button_.get_height()});

	// render the menu
	surrender_button_.Render();
}

void GameInstance::ShowTurnChangeScreen() {
	// completely clear the console
	display_->Clear();
	
	// at centre of where the window print a statement indicating the change in turn
	Coord window_centre(display_->Width() / 2, display_->Height() / 2);
	display_->GoTo(window_centre + Coord{ -7, -1 }); // -7 as ~half the output immediately below so it is centred
	std::cout << "Player " << player_turn_ << "'s turn"; // length of this output is 15 chars
	display_->GoTo(window_centre + Coord{ -13, 1 }); // go left 13 chars (half out ouput immediately below)
	std::cout << "click anywhere to continue"; // length of 26 chars
}


// handles a mouse down event (i.e. the user clicking somewhere on the window)
void GameInstance::HandleLeftMouseButtonDown(const Coord &window_location) { // Coord window_location is in terms of console cells over whole display
	// no mouse click returns a Coord of {-1, -1} so should only be here if coordinate is not -1,-1
	assert(window_location != Coord(-1,-1));

	// get pointers to the tile and unit at the click location. Will be nullptr if there isnt one at location (or clicked off of map)
	Tile* tile = game_map_->GetTileFromConsoleCoord(window_location);
	Unit* unit = nullptr;
	if (tile) {
		unit = game_map_->GetUnit(tile->get_map_coords()); // returns nullptr if no unit present
	}

	switch (state_) {
	// if curently selecting a unit then select a unit if clicked a valid one, else handle the end turn and surrender buttons
	case STATE_SELECTING_UNIT:
		// if the location of the mouse click contains a unit then select the unit if it can be selected
		if (unit) {
			if (unit->CanSelect()) {
				unit->SelectUnit();
			}
		}
		// otherwise check if the click was on the end turn and surrender buttons and handle those cases
		else if (end_turn_button_.Contains(window_location)) {
			end_turn_button_.Trigger();
		}
		else if (surrender_button_.Contains(window_location)) {
			surrender_button_.Trigger();
		}
		break;
	// if selecting an action then handle click on the menu or deselect the unit (and select new unit if clicked on suitable unit)
	case STATE_UNIT_SELECTING_ACTION:
		// if they clicked on the context menu then handle
		if (context_menu_.Contains(window_location)) {
			context_menu_.HandleLeftMouseButtonDown(window_location);
		}
		// if they clicked on the end_turn button then handle
		else if (end_turn_button_.Contains(window_location)) {
			RemoveContextMenu();
			end_turn_button_.Trigger();
		}
		// if they clicked on the surrender button then handle
		else if (surrender_button_.Contains(window_location)) {
			RemoveContextMenu();
			surrender_button_.Trigger();
		}
		// if they didnt click on the menu or the surrender or end turn buttons...
		else {
			// if there is a tile at the window_location of the click (i.e. they clicked on the map) 
			if (tile){
				// deselect the currently selected unit if it has yet to act
				if (!selected_unit_->has_moved_this_turn() && !selected_unit_->has_attacked_this_turn()) {
					// deselect the currently selected unit
					DeselectUnit();
				}
				// if there is also a unit present on that tile then select that unit if it can be selected (even if the currently selected unit has done some action)
				if (unit) {
					if (unit->CanSelect()) {
						unit->SelectUnit();
					}
				}
			}
		}
		break;
	// if selecting movement then handle movement to valid tile or go back to selecting action
	case STATE_UNIT_SELECTING_MOVEMENT:
		// .. and the window_location of the click is a tile (i.e. on the map) ...
		if (tile) { //tile == nullptr if no tile at window_location of click
			//  ... and the unit can reach the tile
			if (selected_unit_->CanReach(tile)) {
				// unhighlight the reachable tiles
				selected_unit_->HighlightReachableTiles(false);
				// move the currently selected unit to the tile
				selected_unit_->MoveTo(tile);
				// if the selected unit has now attacked and moved this turn then deselect it
				if (selected_unit_->has_moved_this_turn() && selected_unit_->has_attacked_this_turn()) {
					DeselectUnit();
				}
				// if it can still act then reselect it
				else{
					selected_unit_->SelectUnit();
				}
			}
			// if it cant reach the tile
			else {
				// run un-choose-movement function ( takes back to selecting action)
				UnChooseMovement();
			} // if click not on the map then nothing happens
		}
		break;
	// if selecting attack then either handle attack on valid unit or go back to selecting action
	case STATE_UNIT_SELECTING_ATTACK:
		// ... and the window_location of the click is a unit ...
		if (unit) {
			// ... and the currently selected unit can attack this target unit...
			if (selected_unit_->CanAttackTarget(unit)) {
				// unhighlight the attackable tiles
				selected_unit_->HighlightAttackableUnits(false);
				selected_unit_->Attack(unit);
				// if this attack didnt result in victory for either player then continue..
				if (state_ != STATE_VICTORY){
					// if the selected unit has now attacked and moved this turn then deselect it
					if (selected_unit_->has_moved_this_turn() && selected_unit_->has_attacked_this_turn()) {
						DeselectUnit();
					}
					// if it can still act and the game has been won in that last attack then reselect it
					else {
						selected_unit_->SelectUnit();
					}
				}
				
			}
			// if it cant attack the target
			else {
				// set to not selecting attack and bring up the options menu again
				UnChooseAttack();
			}
		}
		// if there is no unit on the tile
		else {
			// set to not selecting attack and bring up the options menu again
			UnChooseAttack();
		}
		break;
	// if between turns then mouse click just triggers the start of the next turn (should automatically do this anyway)
	case STATE_BETWEEN_TURNS:
		StartTurn();
		break;
	// if on victory window then mouse click stops the running game instance
	case STATE_VICTORY:
		running_ = false;
		break;
	}
}

// handles game victory
void GameInstance::Victory(const int &team) {
	state_ = STATE_VICTORY; // set state
	// iterate through the units and remove any remaining from the game
	for (auto unit_iter = p1_units_.begin(); unit_iter != p1_units_.end(); unit_iter++) {
		game_map_->RemoveUnit(*unit_iter);
	}
	p1_units_.clear();
	for (auto unit_iter = p2_units_.begin(); unit_iter != p2_units_.end(); unit_iter++) {
		game_map_->RemoveUnit(*unit_iter);
	}
	p1_units_.clear();

	// show victory message / screen
	display_->Clear();
	// determine top left coord to output ascii art from (art is centred horizontally and placed 1/3 down from top of window)
	int start_col = display_->Width() / 2 - fireworks[0].length() / 2;
	int start_row = display_->Height() / 3 - (int)fireworks.size() / 2;
	// if end up with negative of either coordinate then set it to 0
	if (start_col < 0) {
		start_col = 0;
	}
	if (start_row < 0) {
		start_row = 0;
	}
	// go through the ascii art and print out each row (art is stored as vector of row strings)
	for (int row = 0; row < (int)fireworks.size(); row++) {
		display_->GoTo(Coord{ start_col, start_row + row });
		std::string row_str = fireworks[row];
		std::cout << row_str;
	}
	// print victory message
	display_->GoTo(Coord{ display_->Width() / 2 - 25 / 2, start_row + (int)fireworks.size() + 2 }); // 25 is length of congratulations string below
	std::cout << "CONGRATULATIONS PLAYER " << team << "!";
	display_->GoTo(Coord{ display_->Width() / 2 - 8 / 2, start_row + (int)fireworks.size() + 3 }); // 4 is length of you won string below
	std::cout << "YOU WON!";
}

// run the main game loop until the user quits
void GameInstance::Run() {
	// set up the start of the first turn
	StartTurn();
	running_ = true; // will set to false if user clicks quit button or somone wins
	while (running_) {
		// get location of mouse click
		Coord mouse_down_pos = display_->MouseDownPosition();
		// if mouse click was on the window then handle it
		if (mouse_down_pos != Coord{ -1,-1 }) {
			HandleLeftMouseButtonDown(mouse_down_pos);
			// if the game isnt over then auto end the turn if appropriate
			if (state_ != STATE_VICTORY) {
				// if all the units on the team whos go it is have now acted then end the turn
				AutoEndTurn();
			}
		}
	}
}