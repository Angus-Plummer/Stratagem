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
	Button(Coord location, std::string text, std::function<void()> function);
	// ctor for button with menu
	Button(Coord rel_location, std::string text, std::function<void()> function, Menu* menu);
	~Button();

	void Render() const;
	void Interact();

	void set_enabled(const bool &enabled) { enabled_ = enabled; }
	void set_disabled_colour_scheme(const int &colour_scheme) { disabled_colour_scheme_ = colour_scheme; }

};

