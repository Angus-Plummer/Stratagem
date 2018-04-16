#include "ui_object.h"



UIObject::UIObject() {
}


UIObject::~UIObject()
{
}

// returns true if the input coordinate is within the area covered by the UI object
const bool& UIObject::Contains(const Coord &coord) const {
	return coord.x >= location_.x && coord.x < location_.x + width_ && coord.y >= location_.y && coord.y < location_.y + height_;
}
