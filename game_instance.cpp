#include "game_instance.h"
#include "game_manager.h"
#include "window.h"
#include "map.h"
#include "tile.h"
#include "unit.h"
#include "menu.h"
#include "button.h"
#include "ascii_art.h"

// ---------- ctors, dtors, assignment overloading ---------- //

// default ctor
GameInstance::GameInstance() {
	game_map_ = std::unique_ptr<Map>(new Map()); // create empty map
	selected_unit_ = nullptr; // no unit selected
	context_menu_ = Menu(); // context menu not showing
	end_turn_button_ = Button(); // start with end turn button not showing
	surrender_button_ = Button(); // start with no surrender button showing
	player_turn_ = 1; // start with player 1's turn
	running_ = false; // game initiates not running
	state_ = STATE_SETUP; // set initial state is setup 
}

// copy ctor
GameInstance::GameInstance(const GameInstance &instance) {
	selected_unit_ = instance.selected_unit_;
	context_menu_ = instance.context_menu_;
	end_turn_button_ = instance.end_turn_button_;
	surrender_button_ = instance.surrender_button_;
	player_turn_ = instance.player_turn_;
	running_ = instance.running_;
	state_ = instance.state_;

	// make unique pointer to a new map object that is copy of the input instance's map
	game_map_ = std::unique_ptr<Map>(new Map(*instance.game_map_));

	// make copies of the units and add them to the unit vectors
	for (auto unit_iter = instance.p1_units_.begin(); unit_iter != instance.p1_units_.end(); unit_iter++) {
		p1_units_.push_back((*unit_iter)->Clone());
	}
	for (auto unit_iter = instance.p2_units_.begin(); unit_iter != instance.p2_units_.end(); unit_iter++) {
		p2_units_.push_back((*unit_iter)->Clone());
	}
} 

// move ctor
GameInstance::GameInstance(GameInstance &&instance) : GameInstance() {
	std::swap(game_map_, instance.game_map_);
	std::swap(p1_units_, instance.p1_units_);
	std::swap(p2_units_, instance.p2_units_);
	std::swap(selected_unit_, instance.selected_unit_);
	std::swap(context_menu_, instance.context_menu_);
	std::swap(end_turn_button_, instance.end_turn_button_);
	std::swap(surrender_button_, instance.surrender_button_);
	std::swap(player_turn_, instance.player_turn_);
	std::swap(running_, instance.running_);
	std::swap(state_, instance.state_);
} 
// dtor
GameInstance::~GameInstance() {} 

// copy assignment
GameInstance& GameInstance::operator=(const GameInstance &instance) {
	game_map_ = std::unique_ptr<Map>(new Map(*instance.game_map_));
	selected_unit_ = nullptr;
	context_menu_ = Menu();
	end_turn_button_ = Button();
	surrender_button_ = Button();
	player_turn_ = instance.player_turn_;
	running_ = false;
	state_ = STATE_SETUP;
	// make copies of the units and add them to the unit vectors
	for (auto unit_iter = instance.p1_units_.begin(); unit_iter != instance.p1_units_.end(); unit_iter++) {
		p1_units_.push_back((*unit_iter)->Clone());
	}
	for (auto unit_iter = instance.p2_units_.begin(); unit_iter != instance.p2_units_.end(); unit_iter++) {
		p2_units_.push_back((*unit_iter)->Clone());
	}
	return *this;
}
// move assignment
GameInstance& GameInstance::operator=(GameInstance &&instance) {
	std::swap(game_map_, instance.game_map_);
	std::swap(p1_units_, instance.p1_units_);
	std::swap(p2_units_, instance.p2_units_);
	std::swap(selected_unit_, instance.selected_unit_);
	std::swap(context_menu_, instance.context_menu_);
	std::swap(end_turn_button_, instance.end_turn_button_);
	std::swap(surrender_button_, instance.surrender_button_);
	std::swap(player_turn_, instance.player_turn_);
	std::swap(running_, instance.running_);
	std::swap(state_, instance.state_);

	return *this;
}

// ---------- accessors and mutators ---------- //

// map
Map& GameInstance::get_map() const { return *game_map_; }
Unit& GameInstance::get_selected_unit() const { return *selected_unit_; }

// player turn
const int& GameInstance::get_player_turn() const { return player_turn_; }

// ---------- internal (protected) functions ----------//

// carries out the beginning of a turn
void GameInstance::StartTurn() {
	assert(state_ == STATE_SETUP || state_ == STATE_BETWEEN_TURNS);
	// clear the console and render the game map again, show the end turn button and end turn label
	GameManager::Game().get_display().Clear();
	game_map_->Render();
	ShowTurnLabel();
	ShowEndTurnButton();
	ShowSurrenderButton();

	// enable the units of the player whose turn it is
	if (player_turn_ == 1) {
		for (auto unit_iter = p1_units_.begin(); unit_iter != p1_units_.end(); unit_iter++) {
			(*unit_iter)->EnableActions();
		}
	}
	else if (player_turn_ == 2) {
		for (auto unit_iter = p2_units_.begin(); unit_iter != p2_units_.end(); unit_iter++) {
			(*unit_iter)->EnableActions();
		}
	}
	// set game state to selecting a unit
	state_ = STATE_SELECTING_UNIT;
}

// carries out the end of turn and begins a new turn
void GameInstance::EndTurn() {
	assert(state_ == STATE_SELECTING_UNIT || state_ == STATE_UNIT_SELECTING_ACTION);
	// deselect any currently selected unit (shouldnt be one but just in case)
	DeselectUnit(); // state -> STATE_SELECTING_UNIT
	// set state state to being between turns
	state_ = STATE_BETWEEN_TURNS;
	// change the player turn variable
	if (player_turn_ == 1) {
		// disable all units on p1 team
		for (auto unit_iter = p1_units_.begin(); unit_iter != p1_units_.end(); unit_iter++) {
			(*unit_iter)->DisableActions();
		}
		player_turn_ = 2;
	}
	else if (player_turn_ == 2) {
		// disable all units on p2 team
		for (auto unit_iter = p2_units_.begin(); unit_iter != p2_units_.end(); unit_iter++) {
			(*unit_iter)->DisableActions();
		}
		player_turn_ = 1;
	}
	// run an animation to show turn change (as easy to miss the current turn label)
	ShowTurnChangeScreen();
	// wait for mouse click and then start the next turn
	GameManager::Game().get_display().WaitForMouse();
	StartTurn();
}

// checks if all the units on the current team have acted and ends turn if they have
void GameInstance::AutoEndTurn() {
	// only end turn if the current state is that of selecting a unit (i.e no unit is currently selected)
	if (state_ == STATE_SELECTING_UNIT) {
		// get vector of units of team whos turn it is
		std::vector<Unit*> current_team;
		if (player_turn_ == 1) {
			for (auto unit_iter = p1_units_.begin(); unit_iter != p1_units_.end(); unit_iter++) {
				current_team.push_back(unit_iter->get());
			}
		}
		else if (player_turn_ == 2) {
			for (auto unit_iter = p2_units_.begin(); unit_iter != p2_units_.end(); unit_iter++) {
				current_team.push_back(unit_iter->get());
			}
		}
		// iterate through the units on that team and see if any can be selected or are the currently selected unit
		bool all_acted = true;
		for (auto unit_iter = current_team.begin(); unit_iter != current_team.end(); unit_iter++) {
			if ((*unit_iter)->CanSelect()) { all_acted = false; }
		}
		// if no units can be selected then end the turn
		if (all_acted) { EndTurn(); }
	}
}

// handles game victory
void GameInstance::Victory(const int &team) {
	assert(state_ == STATE_SELECTING_UNIT);
	// get currently used window
	Window display = GameManager::Game().get_display();

	state_ = STATE_VICTORY; // set state
	// iterate through the units and remove any remaining from the game
	for (auto unit_iter = p1_units_.begin(); unit_iter != p1_units_.end(); unit_iter++) {
		game_map_->RemoveUnit(unit_iter->get());
	}
	p1_units_.clear();
	for (auto unit_iter = p2_units_.begin(); unit_iter != p2_units_.end(); unit_iter++) {
		game_map_->RemoveUnit(unit_iter->get());
	}
	p1_units_.clear();

	// show victory message / screen
	GameManager::Game().get_display().Clear();
	// determine top left coord to output ascii art from (art is centred horizontally and placed 1/3 down from top of window)
	int start_col = display.Width() / 2 - fireworks[0].length() / 2;
	int start_row = display.Height() / 3 - (int)fireworks.size() / 2;
	// if end up with negative of either coordinate then set it to 0
	if (start_col < 0) {
		start_col = 0;
	}
	if (start_row < 0) {
		start_row = 0;
	}
	// go through the ascii art and print out each row (art is stored as vector of row strings)
	for (int row = 0; row < (int)fireworks.size(); row++) {
		display.GoTo(Coord{ start_col, start_row + row });
		std::string row_str = fireworks[row];
		std::cout << row_str;
	}
	// print victory message
	display.GoTo(Coord{ display.Width() / 2 - 25 / 2, start_row + (int)fireworks.size() + 2 }); // 25 is length of congratulations string below
	std::cout << "CONGRATULATIONS PLAYER " << team << "!";
	display.GoTo(Coord{ display.Width() / 2 - 8 / 2, start_row + (int)fireworks.size() + 3 }); // 4 is length of you won string below
	std::cout << "YOU WON!";
	// wait for mouse click then set running to false so game instance ends
	display.WaitForMouse();
	running_ = false;
}

// handles entering the UNIT_SELECTING_ATTACK state
void GameInstance::ChooseAttack() {
	assert(state_ == STATE_UNIT_SELECTING_ACTION);
	RemoveContextMenu();
	state_ = STATE_UNIT_SELECTING_ATTACK;
	selected_unit_->HighlightAttackableTiles(true);
}
// handles leaving the UNIT_SELECTING_ATTACK state
void GameInstance::UnChooseAttack() {
	assert(state_ == STATE_UNIT_SELECTING_ATTACK);
	state_ = STATE_UNIT_SELECTING_ACTION;
	selected_unit_->HighlightAttackableTiles(false);
	ShowContextMenu();
}

// handles entering the UNIT_SELECTING_MOVEMENT state
void GameInstance::ChooseMovement() {
	assert(state_ == STATE_UNIT_SELECTING_ACTION);
	RemoveContextMenu();
	state_ = STATE_UNIT_SELECTING_MOVEMENT;
	selected_unit_->HighlightReachableTiles(true);
}
// handles leaving the UNIT_SELECTING_MOVEMENT state
void GameInstance::UnChooseMovement() {
	assert(state_ == STATE_UNIT_SELECTING_MOVEMENT);
	state_ = STATE_UNIT_SELECTING_ACTION;
	selected_unit_->HighlightReachableTiles(false);
	ShowContextMenu();
}

// make and remove the context menu (context menu shows what options the currently selected unit has available to them)
void GameInstance::ShowContextMenu() {
	assert(state_ == STATE_UNIT_SELECTING_ACTION);
	// clear the current buttons from the menu
	context_menu_.Clear();

	// Buttons

	// movement button (player can then choose a move target)
	Button move_button("Move", []() {GameManager::Game().get_instance().ChooseMovement(); });
	if (!selected_unit_->CanMove()) { move_button.set_enabled(false); }
	context_menu_.AddButton(move_button);

	// attack button (player can then choose attack target)
	Button attack_button("Attack", []() {GameManager::Game().get_instance().ChooseAttack(); });
	if (!selected_unit_->CanAttack()) { attack_button.set_enabled(false); }
	context_menu_.AddButton(attack_button);

	// pass / cancel button
	std::string end_cancel_button_name; // name to written on button
	// if the unit has yet to attack or move then the button is named cancel
	if (!selected_unit_->has_attacked_this_turn() && !selected_unit_->has_moved_this_turn()) { end_cancel_button_name = "Cancel"; }
	// if the unit has either moved or attacked already then the button is named pass
	else { end_cancel_button_name = "End"; }
	Button end_cancel_button(end_cancel_button_name, []() {GameManager::Game().get_instance().DeselectUnit(); });
	context_menu_.AddButton(end_cancel_button);

	// default menu position is starting at top left of tile to the right of the unit's tile
	Coord top_left_of_menu = Coord(game_map_->get_map_x_offset() + (selected_unit_->get_map_coords().x + 1) * game_map_->get_tile_width(),
		game_map_->get_map_y_offset() + selected_unit_->get_map_coords().y * game_map_->get_tile_height());
	
	// if the menu would go off the right edge of the map, then move the menu to the left of the unit
	if (!game_map_->GetTileFromConsoleCoord(top_left_of_menu + Coord{ context_menu_.get_width(), 0 })) {
		top_left_of_menu.x -= game_map_->get_tile_width() + context_menu_.get_width(); // go left 1 tile width + the width of the menu
	}
	// if the menu would go off the bottom edge of the map, then move the menu up until it is not off the map
	if (!game_map_->GetTileFromConsoleCoord(top_left_of_menu + Coord{ 0, context_menu_.get_height() })) {
		int bottom_row_of_map = game_map_->get_map_y_offset() + game_map_->get_map_height() * game_map_->get_tile_height();
		top_left_of_menu.y = bottom_row_of_map - context_menu_.get_height();
	}
	context_menu_.set_location(top_left_of_menu);

	// render the menu
	context_menu_.Render();
}
// removes the context menu and re renders any tiles and units it was covering
void GameInstance::RemoveContextMenu() {
	// fill the are occupied by the context menu with blank cells (this will mean any area which never otherwise has anything ouput on it will get reset back to being empty)
	for (int i = 0; i < context_menu_.get_width(); i++) {
		for (int j = 0; j < context_menu_.get_height(); j++) {
			GameManager::Game().get_display().GoTo(context_menu_.get_location() + Coord{ i,j });
			std::cout << ' ';
		}
	}

	// if the context menu is covering the game map at all then re-render the tiles and units that it covers (renders each tile once instead of rendering a tile for each cell of the context menu)
	if (game_map_->GetTileFromConsoleCoord(context_menu_.get_location())) { // "window_location" of menu is the top left cell and it expands right and down
																			// get top-left-most tile covered by menu
		Coord top_left = game_map_->GetTileFromConsoleCoord(context_menu_.get_location())->get_map_coords();
		// for all tiles that are covered by any amount by the context menu...
		for (int i = 0; i < context_menu_.get_width() / game_map_->get_tile_width() + 1; i++) { // the width of the menu / width of the tile gives number of tiles that the menu covers horizontally (rounded down, hence add 1)
			for (int j = 0; j < context_menu_.get_height() / game_map_->get_tile_height() + 1; j++) { // the height of the menu / height of the tile gives number of tiles that the menu covers vertically (rounded down, hence add 1)
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

// show / update the label for who's turn it currently is
void GameInstance::ShowTurnLabel() const {
	Window display = GameManager::Game().get_display();
	// save the current colour scheme to revert back after
	ColourScheme original_colour_scheme = display.get_colour_scheme();
	// go to top right next to map
	display.GoTo(Coord{ game_map_->get_map_x_offset() + game_map_->get_tile_width() * game_map_->get_map_width() + 1,1 });
	// if player 1s turn set colour scheme to blue, player 2s turn then red
	if (player_turn_ == 1) {
		display.set_colour_scheme(ColourScheme(DARK_BLUE, WHITE));
	}
	else if (player_turn_ == 2) {
		display.set_colour_scheme(ColourScheme(DARK_RED, WHITE));
	}
	// print out message saying whos turn it is
	std::cout << " Player " << player_turn_ << "'s turn ";
	// revert back to original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}

// add / redo end turn button
void GameInstance::ShowEndTurnButton() {
	// add button
	end_turn_button_ = Button(" End turn ", []() {GameManager::Game().get_instance().EndTurn(); });

	// menu position is top right, next to map but below the current players turn label
	end_turn_button_.set_location(Coord{ game_map_->get_map_x_offset() + game_map_->get_tile_width() * game_map_->get_map_width() + 1, 3 });

	// render the menu
	end_turn_button_.Render();
}

// show quit game button
void GameInstance::ShowSurrenderButton() {

	// add button
	surrender_button_ = Button(" Surrender ", []() {
		// lambda function runs the victory function for the other player
		if (GameManager::Game().get_instance().get_player_turn() == 1) { GameManager::Game().get_instance().Victory(2); }
		else { GameManager::Game().get_instance().Victory(1); }
	});

	// menu position is bottom right, next to map, far from other buttons
	surrender_button_.set_location(Coord{ game_map_->get_map_x_offset() + game_map_->get_tile_width() * game_map_->get_map_width() + 1,
		game_map_->get_map_y_offset() + game_map_->get_tile_height() * game_map_->get_map_height() - surrender_button_.get_height() });

	// render the menu
	surrender_button_.Render();
}

void GameInstance::ShowTurnChangeScreen() {
	assert(state_ == STATE_BETWEEN_TURNS);
	Window display = GameManager::Game().get_display();
	// completely clear the console
	display.Clear();

	// at centre of where the window print a statement indicating the change in turn
	Coord window_centre(display.Width() / 2, display.Height() / 2);
	display.GoTo(window_centre + Coord{ -7, -1 }); // -7 as ~half the output immediately below so it is centred
	std::cout << "Player " << player_turn_ << "'s turn"; // length of this output is 15 chars
	display.GoTo(window_centre + Coord{ -13, 1 }); // go left 13 chars (half out ouput immediately below)
	std::cout << "click anywhere to continue"; // length of 26 chars
}


// handles a mouse down event (i.e. the user clicking somewhere on the window)
void GameInstance::HandleMouseClick(const Coord &window_location) { // Coord window_location is in terms of console cells over whole display
	// no mouse click returns a Coord of {-1, -1} so should only be here if coordinate is not -1,-1
	assert(window_location != Coord(-1, -1));

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
				SelectUnit(unit);
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
			context_menu_.HandleMouseClick(window_location);
		}
		// if they clicked on the end_turn button then handle
		else if (end_turn_button_.Contains(window_location)) {
			DeselectUnit();
			end_turn_button_.Trigger();
		}
		// if they clicked on the surrender button then handle
		else if (surrender_button_.Contains(window_location)) {
			DeselectUnit();
			surrender_button_.Trigger();
		}
		// if they didnt click on the menu or the surrender or end turn buttons...
		else {
			// if there is a tile at the window_location of the click (i.e. they clicked on the map) 
			if (tile) {
				// deselect the currently selected unit if it has yet to act
				if (!selected_unit_->has_moved_this_turn() && !selected_unit_->has_attacked_this_turn()) {
					// deselect the currently selected unit
					DeselectUnit();
				}
				// if there is also a unit present on that tile then select that unit if it can be selected (even if the currently selected unit has done some action)
				if (unit) {
					if (unit->CanSelect()) {
						SelectUnit(unit);
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
				// if it can still act then select the unit again
				else {
					SelectUnit(selected_unit_);
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
				selected_unit_->HighlightAttackableTiles(false);
				selected_unit_->Attack(unit);
				// if this attack didnt result in victory for either player then continue..
				if (state_ != STATE_VICTORY) {
					// if the selected unit has now attacked and moved this turn then deselect it
					if (selected_unit_->has_moved_this_turn() && selected_unit_->has_attacked_this_turn()) {
						DeselectUnit();
					}
					// if it can still act and the game has been won in that last attack then reselect it
					else {
						SelectUnit(selected_unit_);
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
		// if on victory window then mouse click stops the running game instance (should automatically do this anyway)
	case STATE_VICTORY:
		running_ = false;
		break;
	}
}

// ---------- public functions ---------- //

// load in a map from the 5 premade maps
void GameInstance::LoadMap(const int &map_id) const {
	assert(map_id >= 1 && map_id <= 5 && state_ == STATE_SETUP);
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
void GameInstance::AddUnit(std::unique_ptr<Unit> new_unit, const Coord &pos) {
	assert(state_ == STATE_SETUP);
	game_map_->AddUnit(new_unit.get(), pos);
	if (new_unit->get_team() == 1) {
		p1_units_.push_back(std::move(new_unit));
	}
	else if (new_unit->get_team() == 2) {
		p2_units_.push_back(std::move(new_unit));
	}
}
// remove a unit from the game and map
void GameInstance::RemoveUnit(Unit *unit) {
	// use the Erase-Remove idiom to delete the unit from whichever array it is in
	p1_units_.erase(std::remove_if(p1_units_.begin(), p1_units_.end(), [&unit](const std::unique_ptr<Unit> &vec_unit) {return vec_unit.get() == unit; }), p1_units_.end());
	p2_units_.erase(std::remove_if(p2_units_.begin(), p2_units_.end(), [&unit](const std::unique_ptr<Unit> &vec_unit) {return vec_unit.get() == unit; }), p2_units_.end());
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

// select the target unit
void GameInstance::SelectUnit(Unit *unit) {
	// deselect any currently selected unit
	DeselectUnit();
	// update the unit
	unit->SelectUnit();
	// update selected_unit_, state, and show the context menu
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

// run the main game loop until the user quits
void GameInstance::Run() {
	assert(state_ == STATE_SETUP);
	// set up the start of the first turn
	StartTurn();
	running_ = true; // will set to false if user clicks quit button or somone wins
	while (running_) {
		// get location of mouse click
		Coord mouse_click_pos = GameManager::Game().get_display().MouseClickPosition();
		// if mouse click was on the window then handle it
		if (mouse_click_pos != Coord{ -1,-1 }) {
			HandleMouseClick(mouse_click_pos);
			// if the game isnt over then auto end the turn if appropriate
			if (state_ != STATE_VICTORY) {
				// if all the units on the team whos go it is have now acted then end the turn
				AutoEndTurn();
			}
		}
	}
}