#pragma once
#include "stdafx.h"

// class for output to handling console output and mouse input
class Window {
protected:
	HWND console_window_; // handle for the console window
	HANDLE standard_in_handle_; // handle for standard input
	HANDLE standard_out_handle_; // handle for standard output
	DWORD fdwMode_; // holds info on the input/output mode of the console
	//CONSOLE_SCREEN_BUFFER_INFO csbi_; // holds information on the console buffer (pointer to). having just one caused issues so now just declare new one in each function

	// height and width of game map tiles
	int tile_width_;
	int tile_height_;
	// offset of tiles from edge of window (to fit in row and columns labels)
	int map_x_offset_;
	int map_y_offset_;

public:
	// ctor takes an input width and height (in pixel units). It gets the handles and sets the correct input/output mode
	Window(const int &width, const int &height);
	~Window();

	// clears the window
	void Clear();

	// get console window width and height in terms of rows and columns of console cells
	int Width() const;
	int Height() const;
	// get width and height of tiles in terms of console cells
	int get_tile_width() const;
	int get_tile_height() const;
	// get x and y offset of map
	int get_map_x_offset() const;
	int get_map_y_offset() const;
	// get and set the colour scheme of the console
	void set_colour_scheme(const ColourScheme &colour_scheme) const;
	ColourScheme get_colour_scheme() const;

	// get and set the position of the console cursor
	void GoTo(const Coord &coord) const;
	Coord CursorPosition() const;

	// gets the position (console cell location) of the mouse cursor if LMB is pressed clicked down. (acts like detecting a mouse up event)
	Coord MouseDownPosition() const;

	// wait for mouse input (function will keep looping until a click is detected)
	void WaitForMouse();
};
