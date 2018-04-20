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
	const std::string& get_type() const;
	const Coord& get_map_coords() const;
	virtual const ColourScheme& get_colour_scheme() const = 0;// will return the colour scheme correct for the current state of the object

	// render the object on the console
	void virtual Render() const = 0;

};

