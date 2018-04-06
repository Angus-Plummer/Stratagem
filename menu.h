#pragma once
#include "stdafx.h"
#include "ui_object.h"
#include "button.h"

class Menu : public UIObject {
protected:
	std::vector<Button> options_; // vector of buttons
	int border_thickness_;
public:
	Menu();
	~Menu();

	void set_location(const Coord &position);

	void Render() const;

	void AddButton(Button &button);

	int NumberOfOptions() const { return options_.size(); }

	int get_border_thickness() const { return border_thickness_; }

	void HandleLeftMouseButtonDown(const Coord &coord);
};

