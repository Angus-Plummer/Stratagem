#pragma once
#include<string>
// class for selectable objects : tiles, units, screen elements (menu buttons etc.)
class Selectable{
protected:
	std::string type_; // string of name of tile type
	char marker_; // representation of tile type as char for when printing map
	int default_colour_scheme_; // default colour scheme of terminal output
	int selected_colour_scheme_; // colour scheme when selected
	int highlighted_colour_scheme_; // colour scheme when highlighted
	bool selected_; // if object is currently selected then this is true, otherwise false
	bool highlighted_; // if object is currently highlighted then this is true, otherwise false
public:
	Selectable();
	virtual ~Selectable();

	// accessors
	std::string get_type() const { return type_; }
	char get_marker() const { return marker_; }
	int get_colour_scheme() const; // implementation in cpp file. (returns default/selected/highlighted colour scheme depending on whether tile is selected or not)
};

