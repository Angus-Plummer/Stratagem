#include "menu.h"
#include "game_instance.h"
#include "screen.h"
#include "button.h"


Menu::Menu() {
	location_ = Coord{ 0,0 }; // default position is top left of screen. must set to something
	border_thickness_ = 1; // thickness of the menu's border
	width_ = 2 * border_thickness_;
	height_ = 2 * border_thickness_;
	colour_scheme_ = 15; // white on black
}


Menu::~Menu(){ // CHECK IF NEED TO DELETE THE BUTTONS IN THE OPTIONS VECTOR DIRECTLY
}

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


void Menu::Render() const {
	// get the currently used screen
	Screen display = GameInstance::instance().get_display();
	
	// get original colour scheme to revert back at end
	int original_colour_scheme = display.get_colour_scheme();
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
	for (int i = border_thickness_; i < width_ - border_thickness_; i++) {
		std::cout << char(196); // horizontal (single: 196, double: 205)
	}
	std::cout << char(191); // top right corner (single: 191, double: 187, right double: 183, top double: 184)
	
	// side borders
	for (int i = border_thickness_; i < height_ - border_thickness_; i++) {
		// left side
		display.GoTo(location_ + Coord{ 0, i });
		std::cout << char(179); // vertical line (single: 179 double: 186)
		// right side
		display.GoTo(location_ + Coord{ width_ - border_thickness_, i });
		std::cout << char(179); // vertical line (single: 179 double: 186)
	}

	// bottom border
	display.GoTo(location_ + Coord{ 0, height_ - border_thickness_ });
	std::cout << char(192); // bottom left corner (single: 192, double: 200, left double: 211, bottom double: 212)
	for (int i = border_thickness_; i < width_ - border_thickness_; i++) {
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

void Menu::HandleLeftMouseButtonDown(const Coord &coord) {
	// check if the coordinate is contained in the menu 
	if (Contains(coord)) {
		bool in_border = true; // flag will remain true if none of the buttons contain the coord
		// check which button contains the coordinate (if any)
		for (auto iter = options_.begin(); iter != options_.end(); iter++) {
			if (iter->Contains(coord)) {
				// trigger the button and set in_border flag to false
				iter->Trigger();
				in_border = false;
				break; // no need to to continue iterating through the options when we've found it
			}
		}
		// if the event location in the menu but not any of the buttons then it was on the menu border
		if (in_border) {
			// do what happens for left / right click in border
		}
	}
}

