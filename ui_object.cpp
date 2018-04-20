#include "ui_object.h"

// ---------- ctor and dtor ---------- //

// ctor
UIObject::UIObject() {}

// dtor
UIObject::~UIObject(){}

// ---------- accessors and mutators ---------- //

// location in window
void UIObject::set_location(const Coord &coord) { location_ = coord; }
const Coord& UIObject::get_location() const { return location_; }

// colours scheme
void UIObject::set_colour_scheme(const ColourScheme &colour_scheme) { colour_scheme_ = colour_scheme; }

// dimensions (in console cells)
const int& UIObject::get_width() const { return width_; }
const int& UIObject::get_height() const { return height_; }

// ---------- other public functions ---------- //

// returns true if the input coordinate is within the area covered by the UI object
const bool UIObject::Contains(const Coord &coord) const {
	return coord.x >= location_.x && coord.x < location_.x + width_ && coord.y >= location_.y && coord.y < location_.y + height_;
}
