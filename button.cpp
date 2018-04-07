#include "button.h"
#include "game_instance.h"
#include "menu.h"
#include "screen.h"


// ctor for button
Button::Button(std::string text, std::function<void()> function): 
	text_(text), result_(function), 
	parent_menu_(nullptr), enabled_(true), disabled_colour_scheme_(8) // this colour scheme is grey on black
{
	width_ = text_.length(); // width is just the number of chars in the text
	height_ = 1;
	colour_scheme_ = 15; // white on black
}


Button::~Button()
{
}

// renders the button on the screen
void Button::Render() const {
	// get the currently used display
	Screen display = GameInstance::instance().get_display();
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
void Button::Trigger() {
	// if the button is enabled then trigger its function
	if (enabled_) { 
		result_(); 
	}
}

void Button::UpdateWidth() {
	if (parent_menu_) {
		width_ = parent_menu_->get_width() - 2 * parent_menu_->get_border_thickness();
	}
	else {
		width_ = text_.length();
	}
}
