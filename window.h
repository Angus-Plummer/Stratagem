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

public:
	// ctor takes an input width and height (in pixel units). It gets the handles and sets the correct input/output mode
	Window(const int &width, const int &height);
	~Window();

	// get console window width and height in terms of rows and columns of console cells
	const int Width() const;
	const int Height() const;

	// get and set the colour scheme of the console
	void set_colour_scheme(const ColourScheme &colour_scheme) const;
	const ColourScheme get_colour_scheme() const;

	// get and set the position of the console cursor
	const Coord CursorPosition() const;
	void GoTo(const Coord &coord) const;

	// set the buffer size to input number of lines (will do nothing if lines < window height)
	void SetBufferSize(const int &lines) const ;
	// set the buffer size to fit the window size exactly
	void ResetBufferSize() const;

	// clears the window
	void Clear() const;

	// gets the position (console cell location) of the mouse cursor if LMB is pressed clicked down. (acts like detecting a mouse up event)
	const Coord MouseDownPosition() const;

	// wait for mouse input (function will keep looping until a click is detected)
	void WaitForMouse() const;
};

