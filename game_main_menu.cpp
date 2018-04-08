#include "game_main_menu.h"
#include "screen.h"
#include "ascii_art.h"



GameMainMenu::GameMainMenu(Screen &display) : display_(&display){
	state_ = STATE_TITLE_SCREEN;
	instance_ = nullptr;
}


GameMainMenu::~GameMainMenu()
{
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
	int original_colour_scheme = display_->get_colour_scheme();
	// set colour scheme to yellow on black
	display_->set_colour_scheme(14);
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
	std::string button_text("Play");
	Button start_button(button_text, [this]() {StartMapSelection(); });
	// button centred horizontally
	start_button.set_location(Coord{display_->Width() / 2 - (int)std::string(button_text).length() / 2 , display_->Height() / 2 + 4});
	buttons_.push_back(start_button);

	// help button (player moves to help screen)
	button_text = "Help";
	Button help_button(button_text, [this]() {ShowHelpScreen(); });
	// button located 1/3rd along the screen
	help_button.set_location(Coord{ display_->Width() / 3 - (int)std::string(button_text).length() / 2, display_->Height() / 2 + 4});
	buttons_.push_back(help_button);

	// quit button (exits the game)
	button_text = "Quit";
	Button quit_button(button_text, [this]() {state_ = STATE_QUITTING; });
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
	std::string button_text("Back");
	Button back_button(button_text, [this]() {ShowTitleScreen(); });
	// button at top right if screen (with 2 cell wide, 1 cell high margin)
	back_button.set_location(Coord{ display_->Width() - (int)std::string(button_text).length() - 2, 1 });
	buttons_.push_back(back_button);

	RenderButtons();

}

// moved the game into the map selection state
void GameMainMenu::StartMapSelection() {
	// set the state
	// clear the screen
	// create a map (may not survive when this function exits so may need a gameinstance already for it to live in)
	// make buttons for either list of maps or next/previous map (the buttons make the map load a new map from raw_maps.h and render it)
	// need button for confirm the map which will take the player to the start unit placement screen
	// render the buttons
}

// moves the game into the unit placement state
void GameMainMenu::StartUnitPlacement() {}

// remove all units of a team from the game map (allows them to try placing again. also run for both teams when backing out of unit selection)
void GameMainMenu::ResetUnitPlacement(const int &team) {}

// handles a mouse down event (i.e. the user clicking somewhere on the screen)
void GameMainMenu::HandleLeftMouseButtonDown(const Coord &location){
	// double check mouse event was on the screen
	// go through the vector of menus and check if any contain the mouse event location. if so then handle the mouse event
}

// starts the actual game running
void GameMainMenu::PlayGame() {
	// change state
	// clear the screen
	// run the game
	// on game exit go to the title menu
}

// runs the main game loop until the user quits the game
void GameMainMenu::Run() {
	while (state_ != STATE_QUITTING) {
		// if there was a mouse event on the screen..
			// then handle mouse event
	}
}
