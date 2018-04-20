#include "game_manager.h"
#include "window.h"
#include "game_instance.h"
#include "map.h"
#include "tile.h"
#include "unit_warrior.h"
#include "unit_rogue.h"
#include "unit_archer.h"
#include "ascii_art.h"
#include "help_text.h"

// ---------- ctors and dtors ---------- //

// ctor with no window to output to
GameManager::GameManager() {
	// start in title window
	state_ = STATE_TITLE_SCREEN;
	// start with map#1, no unit being placed and team 1 placing units
	current_map_num = 1;
	placing_unit_ = std::unique_ptr<Unit>(nullptr);
	team_placing_ = 1;
}
// ctor with window to output to
GameManager::GameManager(Window &display) : display_(&display){
	// start in title window
	state_ = STATE_TITLE_SCREEN;
	// start with map#1, no unit being placed and team 1 placing units
	current_map_num = 1;
	placing_unit_ = std::unique_ptr<Unit>(nullptr);
	team_placing_ = 1;
}

// dtor
GameManager::~GameManager(){}

// initiate the static GameManager object
GameManager GameManager::game_;

// ---------- accessors ---------- //

// get a reference to the current game instance
GameInstance& GameManager::get_instance() { return instance_; }

// get a reference to the display window
const Window& GameManager::get_display() const { return *display_; }

// set the window to the display
void GameManager::set_display(Window &display) { display_ = &display; }

// ---------- internal (protected) functions ---------- //

// clear all buttons from the vector of menus (SHOULD I CLEAR THE SCREEN IN THIS FUNCTION AS WELL?)
void GameManager::ClearButtons() {
	//clears the buttons vector
	buttons_.clear();
}

// render all buttons
void GameManager::RenderButtons() const {
	// individually render the menus within the menus vector
	for (auto button_iter = buttons_.begin(); button_iter != buttons_.end(); button_iter++) {
		button_iter->Render();
	}
}

// moves the game into the title window state (shows ascii art and updates menus)
void GameManager::ShowTitleScreen() {
	// set state to title window
	state_ = STATE_TITLE_SCREEN;

	// remove all menus and clear the console
	ClearButtons();
	display_->Clear();

	// ----------show title art----------

	// save current colour scheme to set back after
	ColourScheme original_colour_scheme = display_->get_colour_scheme();
	// set colour scheme to yellow on black
	display_->set_colour_scheme(ColourScheme(BLACK, YELLOW));
	// determine top left coord to output ascii art from (art is centred horizontally and placed 1/3 down from top of window)
	int start_col = display_->Width() / 2 - (int)stratagem[0].length() / 2;
	int start_row = display_->Height() / 3 - (int)stratagem.size() / 2;
	// if end up with negative of either coordinate then set it to 0
	if (start_col < 0) {
		start_col = 0;
	}
	if (start_row < 0) {
		start_row = 0;
	}
	// go through the ascii art and print out each row (art is stored as vector of row strings)
	for (int row = 0; row < (int)stratagem.size(); row++) {
		display_->GoTo(Coord{ start_col, start_row + row });
		std::string row_str = stratagem[row];
		std::cout << row_str;
	}
	// revert back to original colour scheme
	display_->set_colour_scheme(original_colour_scheme);

	// ---------- add buttons (menus) ----------

	// play button (player moves on to map selection)
	std::string button_text(" Play ");
	Button start_button(button_text, [this]() {StartMapSelection(); });
	// button centred horizontally
	start_button.set_location(Coord{ display_->Width() / 2 - (int)std::string(button_text).length() / 2 , display_->Height() / 2 + 4 });
	buttons_.push_back(start_button);

	// help button (player moves to help screen)
	button_text = " Help ";
	Button help_button(button_text, [this]() {ShowHelpScreen(); });
	// button located 1/3rd along the window
	help_button.set_location(Coord{ display_->Width() / 3 - (int)std::string(button_text).length() / 2, display_->Height() / 2 + 4 });
	buttons_.push_back(help_button);

	// quit button (exits the game)
	button_text = " Quit ";
	Button quit_button(button_text, [this]() {state_ = STATE_QUITTING; });
	// button 2/3rd along the window
	quit_button.set_location(Coord{ display_->Width() * 2 / 3 - (int)std::string(button_text).length() / 2 , display_->Height() / 2 + 4 });
	buttons_.push_back(quit_button);

	// render all the menus
	RenderButtons();
}

// moves the game into the help screen state
void GameManager::ShowHelpScreen() {
	// set state to help window
	state_ = STATE_HELP_SCREEN;

	// remove all menus and clear the console
	ClearButtons();
	display_->Clear();

	// set temporary buffer size (if changing screen size or just help text in general will need to change this manually or write something to count the lines required)
	display_->SetBufferSize(85);

	// write the help text starting from coordinate 0,0
	display_->GoTo(Coord());
	for (int line = 0; line < (int)help_text.size(); line++) {
		std::cout << WrapString(help_text[line]) << std::endl;
	}
	// record what row the help text ends at
	int last_text_row = display_->CursorPosition().y;

	// add back button ( to reset buffer size and take back to title screen) at top of help screen
	std::string button_text(" Back ");
	Button back_button(button_text, [this]() {display_->ResetBufferSize(); ShowTitleScreen(); });
	// button at top right if window (with 2 cell wide, 1 cell high margin)
	back_button.set_location(Coord{ display_->Width() - (int)std::string(button_text).length() - 2, 1 });
	buttons_.push_back(back_button);
	// add another back button at the end of the help text
	back_button.set_location(Coord{ display_->Width() - (int)std::string(button_text).length() - 2, last_text_row + 1 });
	buttons_.push_back(back_button);

	// render thr buttons
	RenderButtons();

	// set console view to first line at top of console
	display_->GoTo(Coord(0, 0));
}
// helper function to wrap help text in window
std::string GameManager::WrapString(std::string& str) const {
	// number of chars in line is the width of the display window in console cells
	int line_width = display_->Width();
	// if the string has length less than the line length then do nothing
	if ((int)str.length() < line_width) { return str; }
	// last space variable records the char of the last space in the text before the new line
	int last_space = line_width - 1;
	// find the last space in the first line length chars
	for (int i = 0; i < line_width; ++i) {
		if (str.at(i) == ' ') {
			last_space = i;
		}
	}
	// first part of new string is the chars up to the last space + new line char
	std::string new_str = str.substr(0, last_space);
	new_str += '\n';
	// perform WrapString on the remainder of the string and concatenate them together
	new_str += WrapString(str.substr(last_space + 1, std::string::npos));

	// return the wrapped string 
	return new_str;
}

// moved the game into the map selection state
void GameManager::StartMapSelection() {
	// set the state
	state_ = STATE_MAP_SELECTION;
	// clear the console and the buttons
	display_->Clear();
	ClearButtons();
	// load a map into the game instance and render
	instance_.LoadMap(current_map_num);
	instance_.RenderMap();

	// make buttons for either list of maps or next/previous map (the buttons make the map load a new map from raw_maps.h and render it)

	// add next map button ( cycle through to next map)
	std::string button_text(" Next Map ");
	Button next_button(button_text, [this]() {
		current_map_num += 1;
		if (current_map_num > 5) {
			current_map_num = 1;
		}
		instance_.LoadMap(current_map_num);
		instance_.RenderMap();
	});
	// button at top right of map

	next_button.set_location(Coord{ instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1 , 1 });
	buttons_.push_back(next_button);

	// add previous map button ( cycle back to previous map)
	button_text = " Previous Map ";
	Button previous_button(button_text, [this]() {
		current_map_num -= 1;
		if (current_map_num < 1) {
			current_map_num = 5;
		}
		instance_.LoadMap(current_map_num);
		instance_.RenderMap();
	});
	// button at top right of map (below the next map button)
	previous_button.set_location(Coord{ instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, 3 });
	buttons_.push_back(previous_button);

	// add confirm map button ( move on to unit placement with current map)
	button_text = " Confirm Map ";
	Button confirm_button(button_text, [this]() {StartTeamSizeSelection(); });

	// button at bottom right of window
	confirm_button.set_location(Coord{ instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, 5 });
	buttons_.push_back(confirm_button);

	// back button
	button_text = " Back ";
	Button back_button(button_text, [this]() { ShowTitleScreen(); });
	back_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1,
		instance_.get_map().get_map_y_offset() + instance_.get_map().get_tile_height() * instance_.get_map().get_map_height() - 1));
	buttons_.push_back(back_button);

	// render the buttons
	RenderButtons();
}

void GameManager::StartTeamSizeSelection() {
	// set the state
	state_ = STATE_TEAM_SIZE_SELECT;
	// clear the console and the buttons
	display_->Clear();
	ClearButtons();
	// ask user how many units they want to play with
	std::string question("How many units do you want on each team?");
	display_->GoTo(Coord(display_->Width() / 2 - question.length() / 2, display_->Height() / 2 - 3));
	std::cout << question;
	int min_team_size = 2; // minimum size of teams
	int max_team_size = 5; // maximum size of teams
	int num_options = max_team_size - min_team_size + 1;
	// show buttons for 1v1, 2v2, 3v3, 4v4, 5v5...
	for (int n = 0; n < num_options; n++) {
		// set string text to nvn (1v1, 2v2, ..)
		std::string button_text = " " + std::to_string(n + min_team_size) + "v" + std::to_string(n + min_team_size) + " ";
		// clicking on the button will set the size of the team to n an move on to unit placement for player 1
		Button team_size_button(button_text, [this, n, min_team_size]() {team_size_ = min_team_size + n; team_placing_ = 1;  StartUnitPlacement(); });

		// m choices: 1/(2m), 3/(2m), 5/(2m), ..., (2m-1)/(2m). e.g. for 5 choices : first button centred at 1/10 along window, second at 3 / 10, third at 5/10, ...
		team_size_button.set_location(Coord((int)(display_->Width() / (2.*num_options) * (n * 2 + 1) - team_size_button.get_width() / 2.), display_->Height() / 2)); // do calc as doubles then cast to int at the end so that position on window is more accurate
																																									 // add to the buttons vector
		buttons_.push_back(team_size_button);
	}
	// back button
	std::string button_text = " Back ";
	Button back_button(button_text, [this]() { StartMapSelection(); });
	back_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1,
		instance_.get_map().get_map_y_offset() + instance_.get_map().get_tile_height() * instance_.get_map().get_map_height() - 1));
	buttons_.push_back(back_button);
	// render all the buttons
	RenderButtons();
}

// starts the actual game running
void GameManager::PlayGame() {
	// change state
	state_ = STATE_GAME_RUNNING;
	// clear the console
	display_->Clear();
	// run the game
	instance_.Run();
	// make a new instance
	instance_ = GameInstance();
	// on game exit go to the title menu
	ShowTitleScreen();
}

// handles a mouse down event (i.e. the user clicking somewhere on the window)
void GameManager::HandleLeftMouseButtonDown(const Coord &window_location) {
	// mouse event location must
	assert(window_location != Coord(-1, -1));

	// go through the vector of menus and check if any contain the mouse event location. if so then handle the mouse event
	for (auto button_iter = buttons_.begin(); button_iter != buttons_.end(); button_iter++) {
		if (button_iter->Contains(window_location)) {
			button_iter->Trigger();
			return; // return here as cant iterate through the rest of the buttons and dont want to continue through rest of this function if a button was clicked
		}
	}
	// if in the state of unit selection and placement
	if (state_ == STATE_UNIT_PLACEMENT) {
		// get info at location of click..
		// get tile of click location (will be nullptr if not on tile)
		Tile* tile = instance_.get_map().GetTileFromConsoleCoord(window_location);
		Unit* unit = nullptr;
		// if there is a unit on the tile then get that too (will be nullptr otherwise)
		if (tile) {
			// iterator locaiton of unit at the tile coord, will be units_placed_.end() if no unit found at that coordinate
			auto found_unit = std::find_if(units_placed_.begin(), units_placed_.end(), [this, tile](const std::unique_ptr<Unit> &unit) {return unit->get_map_coords() == tile->get_map_coords(); });
			// if the found unit iterator is not the units_placed_.end() then it will point to a unit at the tile coord so set unit to the dereferenced iterator
			if (found_unit != units_placed_.end()) {
				unit = found_unit->get();
			}
		}

		// and the user clicked on a tile that hasnt got a unit on it and is currently placing a unit
		if (tile && !unit && placing_unit_ && CountUnits() < team_size_) {
			// and if can place on that tile
			if (tile->get_highlighted()) {
				// unhighlight the tiles
				ResetTiles();
				// set the units position to be that position on the map
				placing_unit_->set_map_coords(tile->get_map_coords());
				// render the unit at its placed location
				placing_unit_->Render();
				// add it to the vector of units
				units_placed_.push_back(move(placing_unit_));

				// set placing_unit to nullptr
				placing_unit_.reset();
				// update the placing unit and unit counter texts
				ShowPlacingUnit();
				ShowUnitCounter();
			}
		}
		// if the user has clicked on a tile that has a unit on it and the unit belongs to the team currently placing units..
		else if (unit) {
			if (unit->get_team() == team_placing_) {
				// remove the current placing unit
				RemovePlacingUnit();
				// move unique pointer of unit in the units_placed_ vector to the placing unit
				std::find_if(units_placed_.begin(), units_placed_.end(), [&unit](const std::unique_ptr<Unit> &vec_unit) {return vec_unit.get() == unit; })->swap(placing_unit_);
				// remove the clicked on unit from the vector of units (is now nullptr after swapping in above line)
				units_placed_.erase(std::remove_if(units_placed_.begin(), units_placed_.end(), [&unit](const std::unique_ptr<Unit> &vec_unit) {return vec_unit.get() == nullptr; }), units_placed_.end());
				// highlight the placable tiles
				HighlightPlaceableTiles();
				// update the unit counter and placing unit text
				ShowUnitCounter();
				ShowPlacingUnit();
				// note that SetPlacingUnit is not used here as need to move the unique pointer without deleting the unit object that is being pointed to.
			}
		}
	}
}


// ---------- internal (protected) functions for unit placement ---------- //

// moves the game into the unit placement state
void GameManager::StartUnitPlacement() {
	// set the state
	state_ = STATE_UNIT_PLACEMENT;
	// clear the console and the buttons
	display_->Clear();
	ClearButtons();
	// clear the console
	display_->Clear();
	// tell players which player is placing units
	std::string inform_text("Player " + std::to_string(team_placing_) + ", select and place your units");
	display_->GoTo(Coord(display_->Width() / 2 - inform_text.length() / 2, display_->Height() / 2));
	std::cout << inform_text;
	// wait for mouse click then clear the console
	display_->WaitForMouse();
	display_->Clear();
	// render the map
	instance_.RenderMap();

	// show the number of units selected out of the required number and the unit being placed
	ShowUnitCounter();
	ShowPlacingUnit();

	// button to add warrior
	std::string button_text = " Add Warrior ";
	Button add_warrior_button(button_text, [this]() {
		// if the max number of units hasnt been placed yet then start placing a new warrior
		if (CountUnits() < team_size_) {
			SetPlacingUnit(new Warrior(team_placing_)); // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS
		}
		else {
			// if the max number of units has been placed already then flash the unit counter to inform the user
			FlashUnitCounter();
		}
	});
	// place button underneath the unit counter and show placing unit text
	add_warrior_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 4));
	buttons_.push_back(add_warrior_button);

	// button to add rogue
	button_text = " Add Rogue ";
	Button add_rogue_button(button_text, [this]() {
		// if the max number of units hasnt been placed yet then start placing a new rogue
		if (CountUnits() < team_size_) {
			SetPlacingUnit(new Rogue(team_placing_)); // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS
		}
		else {
			// if the max number of units has been placed already then flash the unit counter to inform the user
			FlashUnitCounter();
		}
	});
	// place button under the add warrior button
	add_rogue_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 6));
	buttons_.push_back(add_rogue_button);

	// button to add archer
	button_text = " Add Archer ";
	Button add_archer_button(button_text, [this]() {
		// if the max number of units hasnt been placed yet then start placing a new archer
		if (CountUnits() < team_size_) {
			SetPlacingUnit(new Archer(team_placing_)); // MAKE SURE THIS DOESNT CAUSE MEMORY LEAKS
		}
		else {
			// if the max number of units has been placed already then flash the unit counter to inform the user
			FlashUnitCounter();
		}
	});
	// place button under the add rogue button
	add_archer_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 8));
	buttons_.push_back(add_archer_button);

	// button to reset unit placement
	button_text = " Reset ";
	Button reset_button(button_text, [this]() {
		// remove the unit currently being placed and clear the units of the team currently placing units
		RemovePlacingUnit();
		ClearUnits();
	});
	// button goes below add unit buttons
	reset_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 12));
	buttons_.push_back(reset_button);

	// button to confirm unit placement
	button_text = " Confirm ";
	// buttons runs the confirm unit placement function which let the next player place units or move onto the game
	Button confirm_button(button_text, [this]() {ConfirmUnitPlacement(); });
	// location is below the reset button
	confirm_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 14));
	buttons_.push_back(confirm_button);

	// back button
	button_text = " Back ";
	Button back_button(button_text, [this]() {
		// clear the place units for both teams
		team_placing_ = 2;
		ClearUnits();
		team_placing_ = 1;
		ClearUnits();
		// go back to team size selection
		StartTeamSizeSelection();
	});
	// place back button at bottom right, next to map
	back_button.set_location(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1,
		instance_.get_map().get_map_y_offset() + instance_.get_map().get_tile_height() * instance_.get_map().get_map_height() - 1));
	buttons_.push_back(back_button);

	// render all the buttons
	RenderButtons();
}

// confirms the current unit placement (if the correct number of units have been placed)
void GameManager::ConfirmUnitPlacement() {
	// if the number of units placed by the team is equal to the required amount on each team..
	if (CountUnits() == team_size_) {
		// if current player is team 1 then swap to team 2 and begin their unit placement
		if (team_placing_ == 1) {
			team_placing_ = 2;
			StartUnitPlacement();
		}
		// if current player is team 2 then move the placed units to the game and play the game
		else if (team_placing_ == 2) {
			for (auto unit_iter = units_placed_.begin(); unit_iter != units_placed_.end(); unit_iter++) {
				instance_.AddUnit(std::move(*unit_iter), (*unit_iter)->get_map_coords());
			}
			// clear the vector of placed units (as they have now moved to the game instance)
			units_placed_.clear();
			// play the game
			PlayGame();
		}
	}
	// if the number of placed units is not equal to the required amount by each player then flash the unit counter text red
	else {
		FlashUnitCounter();
	}

}

// remove all units of a team from the game map (allows them to try placing again. also run for both teams when backing out of unit selection)
void GameManager::ClearUnits() {
	// iterate through units and delete any that are on the team that is currently selecting units, then replace the pointer with a nullptr
	for (auto unit_iter = units_placed_.begin(); unit_iter != units_placed_.end(); unit_iter++) {
		if ((*unit_iter)->get_team() == team_placing_) {
			// render the tile under the unit (so the unit is no longer rendered)
			instance_.get_map().Render((*unit_iter)->get_map_coords());
			unit_iter->reset();
		}
	}
	// find and remove the nullptrs from the vector
	units_placed_.erase(std::remove_if(units_placed_.begin(), units_placed_.end(), [](const std::unique_ptr<Unit> &unit) {return unit.get() == nullptr; }), units_placed_.end());
	// update the unit counter
	ShowUnitCounter();
}

// returns a vector of the tiles that placing_unit_ can be placed on
std::vector<Tile*> GameManager::PlaceableTiles() const {
	std::vector<Tile*> valid_tiles;
	// iterate through the tiles that are within the set up wdith of the sides of the map
	for (int width = 0; width < instance_.get_map().get_set_up_width(); width++) {
		for (int row = 0; row < instance_.get_map().get_map_height(); row++) {
			int column = 0;
			// for player 1 the setup columns are on the left of map
			if (team_placing_ == 1) {
				column = width;
			}
			// for player 2 the setup columns are on the right of the map
			else if (team_placing_ == 2) {
				column = instance_.get_map().get_map_width() - 1 - width; // need -1 as the numbering starts from zero but get_map_width will start from 1
			}
			// if unit can be placed on the tile then add it to the vector of valid tiles
			// tile valid if: the placed unit can traverse the tile and no unit in the units vector is already on that tile
			if (placing_unit_->CanTraverse(instance_.get_map().GetTile(Coord(column, row))) &&
				std::find_if(units_placed_.begin(), units_placed_.end(), [this, column, row](const std::unique_ptr<Unit> &unit) {return unit->get_map_coords() == Coord(column, row); }) == units_placed_.end()) {
				valid_tiles.push_back(instance_.get_map().GetTile(Coord(column, row)));
			}
		}
	}
	return valid_tiles;
}

// highlight placable tiles (sets placable tiles to highlighted and renders them)
void GameManager::HighlightPlaceableTiles() {
	std::vector<Tile*> placeable_tiles = PlaceableTiles();
	for (auto tile_iter = placeable_tiles.begin(); tile_iter != placeable_tiles.end(); tile_iter++) {
		(*tile_iter)->set_highlighted(true);
		(*tile_iter)->Render();
	}
}

// reset tiles (sets all to unhighlighted and renders. also renders the units on the current players team)
void GameManager::ResetTiles() {
	// iterate over hight of map and setup width
	for (int width = 0; width < instance_.get_map().get_set_up_width(); width++) {
		for (int row = 0; row < instance_.get_map().get_map_height(); row++) {
			// for both sides of map: if tile is highlighted then unhighlight it and render it again
			// left side of map
			if (instance_.get_map().GetTile(Coord(width, row))->get_highlighted() == true) {
				instance_.get_map().GetTile(Coord(width, row))->set_highlighted(false);
				instance_.get_map().GetTile(Coord(width, row))->Render();
			}
			// right side of map
			if (instance_.get_map().GetTile(Coord(instance_.get_map().get_map_width() - 1 - width, row))->get_highlighted() == true) {
				instance_.get_map().GetTile(Coord(instance_.get_map().get_map_width() - 1 - width, row))->set_highlighted(false);
				instance_.get_map().GetTile(Coord(instance_.get_map().get_map_width() - 1 - width, row))->Render();
			}
		}
	}
}

// counts the number of units that have been placed by the current team
const int GameManager::CountUnits() const {
	return std::count_if(units_placed_.begin(), units_placed_.end(), [this](const std::unique_ptr<Unit> &unit) {return unit->get_team() == team_placing_; });

}

// shows the number of units the team currently placing units has placed
void GameManager::ShowUnitCounter() const {
	display_->GoTo(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset()));
	std::cout << "Units: " << CountUnits() << " / " << team_size_;
}

void GameManager::FlashUnitCounter() const {
	// save current colour scheme to revert after
	ColourScheme original_colour_scheme = display_->get_colour_scheme();
	int num_flashes = 3;
	int flash_duration = 300; // duration of one flash in ms
	for (int flashes = 0; flashes < num_flashes; flashes++) {
		// set to red on black
		display_->set_colour_scheme(ColourScheme(BLACK, RED));
		// show the units counter and wait for a short time
		ShowUnitCounter();
		Sleep(flash_duration / 2);
		// revert back to the old colour scheme
		display_->set_colour_scheme(original_colour_scheme);
		// show the unit counter again, with original colour scheme
		ShowUnitCounter();
		Sleep(flash_duration / 2);
	}
}

// shows the type of unit that is currently being placed
void GameManager::ShowPlacingUnit() const {
	// first write blank spaces over the previous text
	display_->GoTo(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 2));
	std::cout << "                ";
	// if there is a unit being placed then print its type, otherwies instruct user to select a unit
	display_->GoTo(Coord(instance_.get_map().get_map_x_offset() + instance_.get_map().get_tile_width() * instance_.get_map().get_map_width() + 1, instance_.get_map().get_map_y_offset() + 2));
	if (placing_unit_) {
		std::cout << "Placing: " << placing_unit_->get_type();
	}
	else {
		std::cout << "Select a unit";
	}
}

// select a new unit to be placed
void GameManager::SetPlacingUnit(Unit *placing_unit) {
	// if there is already a unit being placed then remove it
	RemovePlacingUnit();
	// update the placing_unit to the input arg
	placing_unit_.reset(placing_unit);
	// update the text informing the user of the unit currently being placed
	ShowPlacingUnit();
	// if the new unit being placed is not a nullptr then highlight the placable tiles
	if (placing_unit_) {
		HighlightPlaceableTiles();
	}
}

// removes the unit being placed and replaces with a nullptr, also resets the tiles
void GameManager::RemovePlacingUnit() {
	// if there is currently a unit being placed then delete it, reset the tiles
	if (placing_unit_) {
		placing_unit_.reset();;
		ResetTiles();
		ShowPlacingUnit();
	}
}

// ---------- public functions ---------- //

// runs the main game loop until the user quits the game
void GameManager::Run() {
	ShowTitleScreen();
	while (state_ != STATE_QUITTING) {
		// if there was a mouse event on the window (no click on window then coordinate is (-1,-1))
		Coord mouse_down_pos = display_->MouseDownPosition();
		if (mouse_down_pos != Coord{ -1,-1 }) {
			// handle the mouse down at that location
			HandleLeftMouseButtonDown(mouse_down_pos);
		}
	}
}
