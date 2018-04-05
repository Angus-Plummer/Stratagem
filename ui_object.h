#pragma once
#include "stdafx.h"

class UIObject{
protected:
	Coord location_; // location of the UI object (top left console cell)
	int width_;
	int height_;
	int colour_scheme_;
public:
	UIObject(Coord location);
	virtual ~UIObject();

	// accessors and mutators
	void set_location(const Coord &coord) { location_ = coord; }
	Coord get_location() const { return location_; }
	void set_colour_scheme(const int &colour_scheme) { colour_scheme_ = colour_scheme; }
	int get_width() const { return width_; }
	int get_height() const { return height_; }

	// returns true if the input coordinate is within the area covered by the UI object, defined in cpp file
	bool Contains(const Coord &coord) const;

	// Renders the object on the screen
	virtual void Render() const = 0;
};

