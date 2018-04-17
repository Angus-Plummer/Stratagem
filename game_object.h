#pragma once
#include "stdafx.h"

// class for game objects (i.e objects that go on the map)
class GameObject{
protected:
	std::string type_; // string of name of object type
	Coord map_coords_; // coordinates of object on the game map
	ColourScheme default_colour_scheme_; // default colour scheme of terminal output
	ColourScheme highlighted_colour_scheme_; // colour scheme when highlighted
	
public:
	// ctor and dtor
	GameObject();
	virtual ~GameObject();

	// accessors
	const std::string& get_type() const { return type_; }
	const Coord& get_map_coords() const { return map_coords_; }
	const ColourScheme& get_colour_scheme() const { return default_colour_scheme_; }

	// render the object on the console
	void virtual Render() const = 0;

};

