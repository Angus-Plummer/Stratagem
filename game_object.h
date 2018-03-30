#pragma once
#include<string>
#include<Windows.h>
#include<memory>

class Screen; //forward declaration for screen class
class Map; // forward declaration of the TileMap class

// class for game object (i.e object that go on the map)
class GameObject{
protected:
	std::string type_; // string of name of tile type
	Map* parent_map_; // pointer to the tilemap that this object is on
	COORD map_coords_; // coordinates of object on the game map
	char marker_; // representation of tile type as char for when printing map
	int default_colour_scheme_; // default colour scheme of terminal output
	int selected_colour_scheme_; // colour scheme when selected
	int highlighted_colour_scheme_; // colour scheme when highlighted
	bool selected_; // if object is currently selected then this is true, otherwise false
	bool highlighted_; // if object is currently highlighted then this is true, otherwise false
public:
	GameObject(Map* map);
	virtual ~GameObject();

	// accessors
	std::string get_type() const { return type_; }
	COORD get_map_coords() const { return map_coords_; }
	char get_marker() const { return marker_; }
	int get_colour_scheme() const; // implementation in cpp file. (returns default/selected/highlighted colour scheme depending on whether tile is selected or not)
	void set_selected(bool selected);
	bool get_selected() { return selected_; }
	void set_highlighted(bool highlighted);
	bool get_highlighted() { return highlighted_; }

	// render the object on a screen
	void virtual Render(Screen &display) const = 0;

};

