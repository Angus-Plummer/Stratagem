#include "stdafx.h"
#include "game_object.h"

GameObject::GameObject(GameInstance* game, Map* map) : the_game_(game), parent_map_(map) {

}

GameObject::~GameObject(){}


