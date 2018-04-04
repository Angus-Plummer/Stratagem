#include "button.h"
#include "game.h"
#include "menu.h"
#include "screen.h"


// ctor for button without menu
Button::Button(Coord location, std::string text, std::function<void()> function): 
	UIObject(location), text_(text), result_(function), 
	parent_menu_(nullptr), enabled_(true), disabled_colour_scheme_(8) // this colour scheme is grey on black
{
	width_ = text_.length(); // width is just the number of chars in the text
	height_ = 1;
	colour_scheme_ = 15; // white on black
}
// ctor for button with menu
Button::Button(Coord rel_location, std::string text, std::function<void()> function, Menu* menu) : 
	UIObject(menu->get_location() + rel_location), text_(text), result_(function), parent_menu_(menu),
	enabled_(true), disabled_colour_scheme_(8) // this colour scheme is grey on black
{
	width_ = parent_menu_->get_width() - 2; // width is that of parent menu - the 1 cell width border of the menu
	height_ = 1;
	colour_scheme_ = 15; // white on black
}


Button::~Button()
{
}

// renders the button on the screen
void Button::Render() const {
	// get the currently used display
	Screen display = Game::instance().get_display();
	// save the current colour scheme to rest bacck after rendering
	int original_colour_scheme = display.get_colour_scheme();
	// if the button is enabled then set the colour scheme to the default colour scheme
	if (enabled_) {
		display.set_colour_scheme(colour_scheme_);
	}
	// if disabled then use the disabled colour scheme
	else if (!enabled_){
		display.set_colour_scheme(disabled_colour_scheme_);
	}
	// go to the location of the button and output the text description
	display.GoTo(location_);
	std::cout << text_;
	// revert the colour scheme back to what it was at start
	display.set_colour_scheme(original_colour_scheme);
}
// on button interaction invoke the buttons function
void Button::Interact() {
	result_();
}
