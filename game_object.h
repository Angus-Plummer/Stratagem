#pragma once
#include "stdafx.h"

// forward declarations
class GameInstance;
class Map;
class Screen;

// class for game object (i.e object that go on the map)
class GameObject{
protected:
	std::string type_; // string of name of tile type
	GameInstance* the_game_; // pointer to the current game instance
	Map* parent_map_; // pointer to the tilemap that this object is on
	COORD map_coords_; // coordinates of object on the game map
	char marker_; // representation of tile type as char for when printing map
	int default_colour_scheme_; // default colour scheme of terminal output
	
public:
	GameObject(GameInstance* game, Map* map);
	virtual ~GameObject();

	// accessors
	std::string get_type() const { return type_; }
	COORD get_map_coords() const { return map_coords_; }
	char get_marker() const { return marker_; }
	int get_colour_scheme() { return default_colour_scheme_; }

	// render the object on a screen
	void virtual Render(Screen const &display) const = 0;

};

