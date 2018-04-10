#include "game_main_menu.h"
#include "screen.h"
#include "game_instance.h"
#include "map.h"
#include "ascii_art.h"


GameMainMenu::GameMainMenu(Screen &display) : display_(&display){
	state_ = STATE_TITLE_SCREEN;
	instance_ = new GameInstance(display);
	current_map_ = 1;
}

GameMainMenu::~GameMainMenu(){
}

// clear all buttons from the vector of menus (SHOULD I CLEAR THE SCREEN IN THIS FUNCTION AS WELL?)
void GameMainMenu::ClearButtons() {
	//clears the buttons vector
	buttons_.clear();
}

// render all menus
void GameMainMenu::RenderButtons() {
	// individually render the menus within the menus vector
	for (auto button_iter = buttons_.begin(); button_iter != buttons_.end(); button_iter++) {
		button_iter->Render();
	}
}

// moves the game into the title screen state (shows ascii art and updates menus)
void GameMainMenu::ShowTitleScreen() {
	// set state to title screen
	state_ = STATE_TITLE_SCREEN;

	// remove all menus and clear the screen
	ClearButtons();
	display_->Clear();

	// ----------show title art----------

	// save current colour scheme to set back after
	ColourScheme original_colour_scheme = display_->get_colour_scheme();
	// set colour scheme to yellow on black
	display_->set_colour_scheme(ColourScheme(BLACK, YELLOW));
	// determine top left coord to output ascii art from (art is centred horizontally and placed 1/3 down from top of screen)s
	int start_col = display_->Width() / 2 - (int)stratagem1[0].length() / 2;
	int start_row = display_->Height() / 3 - (int)stratagem1.size() / 2;
	// if end up with negative of either coordinate then set it to 0
	if (start_col < 0) {
		start_col = 0;
	}
	if (start_row < 0) {
		start_row = 0;
	}
	// go through the ascii art and print out each row (art is stored as vector of row strings)
	for (int row = 0; row < (int)stratagem1.size(); row++) {
		display_->GoTo(Coord{ start_col, start_row + row });
		std::string row_str = stratagem1[row];
		std::cout << row_str;
	}
	// revert back to original colour scheme
	display_->set_colour_scheme(original_colour_scheme);

	// ---------- add buttons (menus) ----------

	// play button (player moves on to map selection)
	std::string button_text(" Play ");
	Button start_button(button_text, [this]() {StartMapSelection(); });
	start_button.set_colour_scheme(ColourScheme(WHITE,BLACK));
	// button centred horizontally
	start_button.set_location(Coord{display_->Width() / 2 - (int)std::string(button_text).length() / 2 , display_->Height() / 2 + 4});
	buttons_.push_back(start_button);

	// help button (player moves to help screen)
	button_text = " Help ";
	Button help_button(button_text, [this]() {ShowHelpScreen(); });
	help_button.set_colour_scheme(ColourScheme(WHITE,BLACK));
	// button located 1/3rd along the screen
	help_button.set_location(Coord{ display_->Width() / 3 - (int)std::string(button_text).length() / 2, display_->Height() / 2 + 4});
	buttons_.push_back(help_button);

	// quit button (exits the game)
	button_text = " Quit ";
	Button quit_button(button_text, [this]() {state_ = STATE_QUITTING; });
	quit_button.set_colour_scheme(ColourScheme(WHITE,BLACK));
	// button 2/3rd along the screen
	quit_button.set_location(Coord{ display_->Width() * 2 / 3 - (int)std::string(button_text).length() / 2 , display_->Height() / 2 + 4});
	buttons_.push_back(quit_button);

	// render all the menus
	RenderButtons();
}

// moves the game into the help screen state
void GameMainMenu::ShowHelpScreen() {
	// set state to help screen
	state_ = STATE_HELP_SCREEN;

	// remove all menus and clear the screen
	ClearButtons();
	display_->Clear();

	// write the help text

	// add back button ( to take back to title screen)
	std::string button_text(" Back ");
	Button back_button(button_text, [this]() {ShowTitleScreen(); });
	back_button.set_colour_scheme(ColourScheme(WHITE, BLACK));
	// button at top right if screen (with 2 cell wide, 1 cell high margin)
	back_button.set_location(Coord{ display_->Width() - (int)std::string(button_text).length() - 2, 1 });
	buttons_.push_back(back_button);

	RenderButtons();
}

// moved the game into the map selection state
void GameMainMenu::StartMapSelection() {
	// set the state
	state_ = STATE_MAP_SELECTION;
	// clear the screen and the buttons
	display_->Clear();
	ClearButtons();
	// load a map into the game instance and render
	instance_->LoadMap(current_map_);
	instance_->RenderMap();

	// make buttons for either list of maps or next/previous map (the buttons make the map load a new map from raw_maps.h and render it)
	
	// add next map button ( cycle through to next map)
	std::string button_text(" Next Map ");
	Button next_button(button_text, [this]() {
		current_map_ += 1;
		if (current_map_ > 5) {
			current_map_ = 1;
		}
		instance_->LoadMap(current_map_);
		instance_->RenderMap();
	});
	next_button.set_colour_scheme(ColourScheme(WHITE, BLACK));
	// button at top right of map
	next_button.set_location(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * instance_->get_map().get_map_width() + 1 , 1 });
	buttons_.push_back(next_button);

	// add previous map button ( cycle back to previous map)
	button_text =" Previous Map ";
	Button previous_button(button_text, [this]() {
		current_map_ -= 1;
		if (current_map_ < 1) {
			current_map_ = 5;
		}
		instance_->LoadMap(current_map_);
		instance_->RenderMap();
	});
	previous_button.set_colour_scheme(ColourScheme(WHITE, BLACK));
	// button at top right of map (below the next map button)
	previous_button.set_location(Coord{display_->get_map_x_offset() + display_->get_tile_width() * instance_->get_map().get_map_width() + 1, 3});
	buttons_.push_back(previous_button);

	// add confirm map button ( move on to unit placement with current map)
	button_text = " Confirm Map ";
	Button confirm_button(button_text, [this]() {StartUnitPlacement();});
	confirm_button.set_colour_scheme(ColourScheme(WHITE, BLACK));

	// button at bottom right of screen
	confirm_button.set_location(Coord{ display_->get_map_x_offset() + display_->get_tile_width() * instance_->get_map().get_map_width() + 1, 5 });
	buttons_.push_back(confirm_button);
	
	// render the buttons
	RenderButtons();
}

// moves the game into the unit placement state
void GameMainMenu::StartUnitPlacement() {
	// set the state
	state_ = STATE_UNIT_PLACEMENT;
	// clear the screen and the buttons
	display_->Clear();
	ClearButtons();
	// ask user how many units they want to play with
	// show buttons for 1v1, 2v2, 3v3, 4v4, 5v5
	// button they click on will set the required number of units
	// clear the screen and the buttons
	// tell players which player is placing units ( like change turn screen: black with white text, click to continue)
	// clear the screen again
	// load the map into the game instance and render (should already be loaded)
	instance_->LoadMap(current_map_);
	instance_->RenderMap();
	// hide other players units if there are any placed
	// show player list of unit types as buttons to right of map (Add Archer, Add Warrior, etc)
	// while pxunits.size() < max_num_units...
	// player selects one of their units (becomes game instance's selected unit) (can be from unplaced units list or unit already on map, if unit on map then remove it from the map, and create another of that unit type to place on the map)
	// show tiles that unit can be placed on for current player (edge <-> placement width tiles that are traversible and have no unit on already are highlighted)
	// player clicks on valid tile -> place unit of that type on that tile, deselect the unit
	// player clicks on invalid tile -> do nothing
	// player clicks on another unit -> deselect current unit and select the clicked on unit for placement
	// when all units have been placed show a confirm unit placement button, there is also a reset placement button that unsets all the units (this button is always present)
	// perform unit selection for next player
}

// remove all units of a team from the game map (allows them to try placing again. also run for both teams when backing out of unit selection)
void GameMainMenu::ResetUnitPlacement(const int &team) {}

// handles a mouse down event (i.e. the user clicking somewhere on the screen)
void GameMainMenu::HandleLeftMouseButtonDown(const Coord &screen_location){
	// double check mouse event was on the screen
	if (screen_location.x == -1 && screen_location.y == -1) {
		// throw error as should not be able to get here
		exit(1);
	}
	// go through the vector of menus and check if any contain the mouse event location. if so then handle the mouse event
	for (auto button_iter = buttons_.begin(); button_iter != buttons_.end(); button_iter++) {
		if (button_iter->Contains(screen_location)) {
			button_iter->Trigger();
			break; // break here as cant iterate through the rest of the buttons (the button vector may well change when the button is triggered making it no longer valid)
		}
	}
}

// starts the actual game running
void GameMainMenu::PlayGame() {
	// change state
	state_ = STATE_GAME_RUNNING;
	// clear the screen
	display_->Clear();
	// run the game
	instance_->Run();
	// delete the old game instance
	delete instance_;
	// on game exit go to the title menu
	ShowTitleScreen();
}

// runs the main game loop until the user quits the game
void GameMainMenu::Run() {
	while (state_ != STATE_QUITTING) {
		// if there was a mouse event on the screen (no click on screen then coordinate is (-1,-1))
		Coord mouse_down_pos = display_->MouseDownPosition();
		if (mouse_down_pos != Coord{ -1,-1 }) {
			// handle the mouse down at that location
			HandleLeftMouseButtonDown(mouse_down_pos);
		}
	}
}
