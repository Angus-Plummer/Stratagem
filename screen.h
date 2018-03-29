#pragma once
#include "tile_map.h"
#include "raw_maps.h"

#include<vector>
#include<iostream>
#include<Windows.h>
#include<WindowsX.h>

class Screen{
protected:
	HWND console_window_; // handle for the console window
	HANDLE standard_in_handle_; // handle for standard input
	HANDLE standard_out_handle_; // handle for standard output
	DWORD fdwMode_; // holds info on the input/output mode of the console
	//CONSOLE_SCREEN_BUFFER_INFO csbi_; // holds information on the console buffer (pointer to). having just one caused issues so now just declare new one in each function

	// shared pointer to the TileMap currently being used
	std::shared_ptr<TileMap> game_map_;
	// vector of shared pointers to player units
	// std::vector<std::shared_ptr<Unit>> team1, team2;
	// vector of shared pointers to the UI objects on the screen which are selectable
	//std::vector<std::shared_ptr<UIObject>> interface_objects_;

	// height and width of game map tiles
	int tile_width_;
	int tile_height_;
	// offset of tiles from edge of window (to fit in row and columns labels)
	int map_x_offset_;
	int map_y_offset_;

public:
	// ctor takes an input width and height (in pixel units). It gets the handles and sets the correct input/output mode
	Screen(int const width, int const height);
	~Screen();
	
	// clears the screen
	void Clear();

	// get console screen width and height in terms of rows and columns of console cells
	int Width() const;
	int Height() const;

	// get and set the position of the console cursor
	void GoTo(int const x, int const y) const;
	COORD CursorPosition() const;

	// gets the position of the mouse cursor (pixel location, relative to window)
	POINT MousePosition() const;
	// gets the position of the mouse cursor if LMB is pressed down. (console cell location)
	COORD MouseDownPosition() const;

	// get and set the colour scheme
	void set_colour_scheme(int const attribute) const;
	int get_colour_scheme() const;
	
	// prints out the current map
	void PrintMap() const;
	// change the current map
	void SetMap(std::shared_ptr<TileMap> const game_map);
	// updates the rendering of argument objects
	void Screen::Render(std::shared_ptr<GameObject> const tile) const;
	// select an object on the screen
	std::shared_ptr<GameObject> Select() const;
};
