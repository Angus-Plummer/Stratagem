#include "button.h"
#include "game_manager.h"
#include "game_instance.h"
#include "menu.h"
#include "window.h"

// --------- ctor and dtor --------- //

// ctor for empty button
Button::Button() :
	text_(""), button_function_([]() {}), // no button text and null function
	parent_menu_(nullptr), enabled_(false), // no parent menu and button disabled
	disabled_colour_scheme_(ColourScheme(BLACK, DARK_GREY)) // grey on black
{
	// 0x0 dimensions
	width_ = 0;
	height_ = 0;
	colour_scheme_ = ColourScheme(BLACK, WHITE); // white on black
}

// ctor for button with text and funtion
Button::Button(std::string text, std::function<void()> function): 
	text_(text), button_function_(function), 
	parent_menu_(nullptr), enabled_(true), disabled_colour_scheme_(ColourScheme(BLACK, DARK_GREY)) // this colour scheme is grey on black
{
	width_ = text_.length(); // width is just the number of chars in the text
	height_ = 1;
	colour_scheme_ = ColourScheme(WHITE, BLACK); // white on black
}

// dtor
Button::~Button() {}

// --------- accesssors and mutators ---------- //

// enable or disable button
void Button::set_enabled(const bool &enabled) { enabled_ = enabled; }

// set the colour scheme of the button when disables
void Button::set_disabled_colour_scheme(const ColourScheme &colour_scheme) { disabled_colour_scheme_ = colour_scheme; }

// set the parent menu of the button
void Button::set_menu(Menu *menu) { parent_menu_ = menu; }

// --------- other public functions ---------- //

// renders the button on the console
void Button::Render() const {
	// get the currently used display
	Window display = GameManager::Game().get_display();
	// save the current colour scheme to rest bacck after rendering
	ColourScheme original_colour_scheme = display.get_colour_scheme();
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
		button_function_(); 
	}
}

// update the width of the button (used when the width of a parent menu changes)
void Button::UpdateWidth() {
	if (parent_menu_) {
		width_ = parent_menu_->get_width() - 2 * parent_menu_->get_border_thickness();
	}
	else {
		width_ = text_.length();
	}
}
