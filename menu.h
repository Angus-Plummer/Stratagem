#pragma once
#include "stdafx.h"
#include "ui_object.h"
#include "button.h"

// class for menu of buttons
class Menu : public UIObject {
protected:
	std::vector<Button> options_; // buttons in the menu
	int border_thickness_; // thickness of the menu border

public:
	// ctor and dtor
	Menu();
	~Menu();

	// accessor for border thickness
	const int& get_border_thickness() const { return border_thickness_; }

	// set the location of the top left of the menu (in console cell coordinates)
	void set_location(const Coord &position);

	// render the menu and its buttons in the game window
	void Render() const;

	// add a button to the menu
	void AddButton(Button &button);

	// clears all the buttons from the menu
	void Clear();

	// handle a click at the consonle coordinate "coord", will trigger any button which contains the coordinate
	void HandleLeftMouseButtonDown(const Coord &coord);
};

