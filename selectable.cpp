#include "stdafx.h"
#include "selectable.h"


Selectable::Selectable() : selected_(false), highlighted_(false){
}

Selectable::~Selectable(){}

int Selectable::get_colour_scheme() const {
	// order of importance for colour scheme: selected > highlighted > default
	if (selected_) {
		return selected_colour_scheme_;
	}
	else if (highlighted_) {
		return highlighted_colour_scheme_;
	}
	else {
		return default_colour_scheme_;
	}
}
