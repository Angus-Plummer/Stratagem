#pragma once
#include "ui_object.h"
#include<functional>

class Menu;

class Button : public UIObject {
protected:
	// members
	std::string text_; // text on button
	std::function<void()> result_; // function that is invoked if the button is pressed
	Menu* parent_menu_; // not sure if necessary. Won't always have a parent menu
	bool enabled_; // true if the button can be pressed
	int disabled_colour_scheme_; // colour scheme for when shown but not enabled

public:
	// ctor for button without menu
	Button(std::string text, std::function<void()> function);
	~Button();

	// render the button
	void Render() const;
	// trigger the button's function
	void Trigger();

	// mutators
	void set_enabled(const bool &enabled) { enabled_ = enabled; }
	bool is_enabled() const { return enabled_; }
	void set_disabled_colour_scheme(const int &colour_scheme) { disabled_colour_scheme_ = colour_scheme; }
	void set_menu(Menu *menu) { parent_menu_ = menu; }
	
	// update the width of the button (used when the width of a parent menu changes)
	void UpdateWidth();
};

