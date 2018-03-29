#include "stdafx.h"
#include "game_object.h"


GameObject::GameObject() : selected_(false), highlighted_(false){
}

GameObject::~GameObject(){}

int GameObject::get_colour_scheme() const {
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

// selected and deselect
void GameObject::set_selected(bool selected) {
	selected_ = selected;
}
// highlight and unhighlight
void GameObject::set_highlighted(bool highlighted) {
	highlighted_ = highlighted;
}
