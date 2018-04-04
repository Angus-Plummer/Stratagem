#pragma once
#include "stdafx.h"

class UIObject{
protected:
	Coord location_; // location of the UI object (top right console cell). may be relative to screen or a parent object
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

	virtual void Render() const = 0;
	virtual void Interact() = 0;
};

