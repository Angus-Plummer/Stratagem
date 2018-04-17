#include "menu.h"
#include "game_manager.h"
#include "game_instance.h"
#include "window.h"
#include "button.h"

// ctor
Menu::Menu() {
	location_ = Coord{ 0,0 }; // default position is top left of window. must set to something
	border_thickness_ = 1; // thickness of the menu's border
	width_ = 2 * border_thickness_;
	height_ = 2 * border_thickness_;
	colour_scheme_ = ColourScheme(BLACK, WHITE); // white on black
}

// dtor
Menu::~Menu(){
}

// set the location of the top left of the menu (in console cell coordinates)
void Menu::set_location(const Coord &position) {
	location_ = position;
	// iterate through the vector of buttons and update the location of each one
	int lines_down = 0; // keep track of how many lines have been taken up by buttons moved so far
	for (auto iter = options_.begin(); iter != options_.end(); iter++) {
		// location of button is location of menu + shifted right by border thickness, shifted down by border thickness + how many lines are occupied by buttons already moved
		iter->set_location(location_ + Coord(border_thickness_, border_thickness_ + lines_down));
		// update lines_down to include the most recently added button
		lines_down += iter->get_height(); 
	}
}

// render the menu and its buttons in the game window
void Menu::Render() const {
	// get the currently used window
	Window display = GameManager::game().get_display();
	
	// get original colour scheme to revert back at end
	ColourScheme original_colour_scheme = display.get_colour_scheme();
	display.set_colour_scheme(colour_scheme_);

	// ----- menu background -----
	for (int j = 0; j < height_; j++) {
		display.GoTo(location_ + Coord(0, j));
		for (int i = 0; i < width_; i++) {
			std::cout << ' ';
		}
	}
	
	// ----- menu borders -----
	
	// top border
	display.GoTo(location_); // cursor in top left of menu
	std::cout << char(218); // top left corner (single: 218, double: 201, left double: 214, top double: 213)
	for (int i = 1; i < width_ - 1; i++) {
		std::cout << char(196); // horizontal (single: 196, double: 205)
	}
	std::cout << char(191); // top right corner (single: 191, double: 187, right double: 183, top double: 184)
	
	// side borders
	for (int i = 1; i < height_ - 1; i++) {
		// left side
		display.GoTo(location_ + Coord{ 0, i });
		std::cout << char(179); // vertical line (single: 179 double: 186)
		// right side
		display.GoTo(location_ + Coord{ width_ - 1, i });
		std::cout << char(179); // vertical line (single: 179 double: 186)
	}

	// bottom border
	display.GoTo(location_ + Coord{ 0, height_ - 1 });
	std::cout << char(192); // bottom left corner (single: 192, double: 200, left double: 211, bottom double: 212)
	for (int i = 1; i < width_ - 1; i++) {
		std::cout << char(196); // horizontal (single: 196, double: 205)
	}
	std::cout << char(217); // bottom right corner (single: 217, double: 188, right double: 189, bottom double: 190)
	
	// ----- render the buttons -----
	// iterate through the vector of buttons and render each one
	for (auto iter = options_.begin(); iter != options_.end(); iter++) {
		iter->Render();
	}
	// revert back to the original colour scheme
	display.set_colour_scheme(original_colour_scheme);
}

// add a button to the menu
void Menu::AddButton(Button &button){
	// set the buttons location (bottom of menu)
	button.set_location(location_ + Coord(border_thickness_, height_ -  border_thickness_));
	// set the buttons parent menu as this menu
	button.set_menu(this);
	button.set_colour_scheme(ColourScheme(BLACK,WHITE));
	// add the button to this menu's button vector and increse the height of the menu by the buttons height
	options_.push_back(button);
	// increase the height of the menu to accomodate the button
	height_ += 1; // buttons always have a height of 1
	// if the button is wider than the menu then increase the width of the menu and update all buttons in the menu accordingly
	if (width_ < button.get_width() + 2 * border_thickness_) {
		width_ = button.get_width() + 2 * border_thickness_;
		for (auto iter = options_.begin(); iter != options_.end(); iter++) {
			iter->UpdateWidth();
		}
	}
}

// clears all the buttons from the menu
void Menu::Clear() {
	options_.clear();
	width_ = 2 * border_thickness_;
	height_ = 2 * border_thickness_;
}

// handle a click at the consonle coordinate "coord", will trigger any button which contains the coordinate
void Menu::HandleLeftMouseButtonDown(const Coord &coord) {
	// check if the coordinate is contained in the menu 
	if (Contains(coord)) {
		// check which button contains the coordinate (if any)
		for (auto iter = options_.begin(); iter != options_.end(); iter++) {
			if (iter->Contains(coord)) {
				// trigger the button and set in_border flag to false
				iter->Trigger();
				break; // no need to to continue iterating through the options when we've found it
			}
		}
	}
}

