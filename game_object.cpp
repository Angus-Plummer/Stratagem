#include "stdafx.h"
#include "game_object.h"
#include "screen.h"
#include "map.h"


GameObject::GameObject(Map* map) : parent_map_(map), selected_(false), highlighted_(false){

}

GameObject::~GameObject(){}

int GameObject::get_colour_scheme() const {
	// order of importance for colour scheme: selected > highlighted > default
	if (selected_) {
		return selected_colour_scheme_;
	}
	if (highlighted_) {
		return highlighted_colour_scheme_;
	}
	// if neither then return default colour scheme
	return default_colour_scheme_;
}

// selected and deselect
void GameObject::set_selected(bool selected) {
	selected_ = selected;
}
// highlight and unhighlight
void GameObject::set_highlighted(bool highlighted) {
	highlighted_ = highlighted;
}

