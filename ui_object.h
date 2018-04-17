#pragma once
#include "stdafx.h"

// abstract base class from which all UI objects derive (menus and buttons)
class UIObject{
protected:
	Coord location_; // location of the UI object (top left console cell)
	int width_; // width of UI object in console cells
	int height_; // height of UI object in console cells
	ColourScheme colour_scheme_; // default colour scheme of the UI object
public:
	// constructor and destructor
	UIObject();
	virtual ~UIObject();

	// accessors and mutators
	void set_location(const Coord &coord) { location_ = coord; }
	const Coord& get_location() const { return location_; }
	void set_colour_scheme(const ColourScheme &colour_scheme) { colour_scheme_ = colour_scheme; }
	const int& get_width() const { return width_; }
	const int& get_height() const { return height_; }

	// returns true if the input coordinate is within the area covered by the UI object, defined in cpp file
	const bool& Contains(const Coord &coord) const;

	// Renders the object on the console
	virtual void Render() const = 0;
};

