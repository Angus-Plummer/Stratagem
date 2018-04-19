#pragma once
#include "ui_object.h"
#include<functional> // for function as member variable

// forward declarations
class Menu;

// button class: allows users to click on a location on the screen and trigger a function to run. The function is declared at button creation
class Button : public UIObject {
protected:
	// members
	std::string text_; // text on button
	std::function<void()> button_function_; // function that is invoked if the button is pressed
	Menu* parent_menu_; // pointer to the parent menu, nullptr if none
	bool enabled_; // true if the button can be pressed
	ColourScheme disabled_colour_scheme_; // colour scheme for when shown but not enabled

public:
	// ctor for disabled empty button
	Button();
	// ctor for button with text and function
	Button(std::string text, std::function<void()> function);
	// dtor
	~Button();

	// mutators
	void set_enabled(const bool &enabled) { enabled_ = enabled; }
	void set_disabled_colour_scheme(const ColourScheme &colour_scheme) { disabled_colour_scheme_ = colour_scheme; }
	void set_menu(Menu *menu) { parent_menu_ = menu; }

	// render the button
	void Render() const;

	// trigger the button's function
	void Trigger();
	
	// update the width of the button (used when the width of a parent menu changes)
	void UpdateWidth();
};

