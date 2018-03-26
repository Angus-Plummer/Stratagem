#pragma once
#include "tile_map.h"
#include "maps.h"

#include<vector>
#include<iostream>
#include<Windows.h>
#include<WindowsX.h>

class Screen{
protected:
	HWND console_window_; // handle for the console window
	HANDLE hStdin_; // handle for standard input
	HANDLE hStdout_; // handle for standard output
	DWORD fdwMode_; // holds info on the input/output mode of the console
	//CONSOLE_SCREEN_BUFFER_INFO csbi_; // holds information on the console buffer (pointer to). having just one caused issues so now just declare new one in each function

	std::shared_ptr<TileMap> game_map_;
	// std::vector<Unit> team1;
	// std::vector<Unit> team2;
	std::vector<std::shared_ptr<Selectable>> console_cells;
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

	void PrintMap() const;

	void SetMap(std::shared_ptr<TileMap> const game_map);
};

