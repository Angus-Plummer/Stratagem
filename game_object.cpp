#include "stdafx.h"
#include "game_object.h"

// --------- ctors and dtors --------- //

// ctor
GameObject::GameObject() {}

// dtor
GameObject::~GameObject() {}

// --------- accessors -------- //

// object type string
const std::string& GameObject::get_type() const { return type_; }

// map coordinates of game object
const Coord& GameObject::get_map_coords() const { return map_coords_; }
