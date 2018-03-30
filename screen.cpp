#include "stdafx.h"
#include "screen.h"


Screen::Screen(int const width, int const height) : console_window_(GetConsoleWindow()),
													standard_in_handle_(GetStdHandle(STD_INPUT_HANDLE)),
													standard_out_handle_(GetStdHandle(STD_OUTPUT_HANDLE)),
													tile_width_(6), tile_height_(3),
													map_x_offset_(3), map_y_offset_(1)
{
	RECT r; // rectangle to store window position and dimensions
	// disable quick edit mode so that mouse input works
	fdwMode_ = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(standard_in_handle_, fdwMode_)) { exit(1); }
	// enable mouse input events
	fdwMode_ = ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(standard_in_handle_, fdwMode_)) { exit(1); }

	//stores the console's current dimensions
	GetWindowRect(console_window_, &r);
	// resize window height and width
	MoveWindow(console_window_, r.left, r.top, width, height, TRUE);
	// make cursor invisible
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(standard_out_handle_, &cursor_info);
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(standard_out_handle_, &cursor_info);
}


Screen::~Screen(){
}

void Screen::Clear() {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	COORD top_left = { 0, 0 };

	// flush cout buffer so no text gets written after clearing screen
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(standard_out_handle_, &buffer)) {
		exit(1);
	}
	DWORD length = buffer.dwSize.X * buffer.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(standard_out_handle_, TEXT(' '), length, top_left, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(standard_out_handle_, buffer.wAttributes, length, top_left, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(standard_out_handle_, top_left);
}

// get console screen width and height in terms of rows and columns of console cells
int Screen::Width() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	if (!GetConsoleScreenBufferInfo(standard_out_handle_, &buffer)) {
		exit(1);
	}
	return buffer.srWindow.Right - buffer.srWindow.Left;

}
int Screen::Height() const{
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	if (!GetConsoleScreenBufferInfo(standard_in_handle_, &buffer)) {
		exit(1);
	}
	return buffer.srWindow.Bottom - buffer.srWindow.Top;
}

// tile width and height in  terms of console cells
int Screen::get_tile_width() const { return tile_width_; }
int Screen::get_tile_height() const { return tile_height_;}
// map x and y offset in terms of console cells
int Screen::get_map_x_offset() const { return map_x_offset_; }
int Screen::get_map_y_offset() const { return map_y_offset_; }


// get and set the position of the console cursor
void Screen::GoTo(int const x, int const y) const {
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(standard_out_handle_, coord);
}
COORD Screen::CursorPosition() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	// attempt to get buffer info
	if (GetConsoleScreenBufferInfo(standard_out_handle_, &buffer)) {
		return buffer.dwCursorPosition;
	}
	// if function fails then return -1, -1
	else {
		COORD fail = { -1, -1 };
		return fail;
	}
}

// gets the position of the mouse cursor (pixel location, relative to window)
POINT Screen::MousePosition() const {
	POINT position; // POINT var to hold output
	// try to get cursor position ( relative to top left of screen)
	if (GetCursorPos(&position)) {
		// try to convert to position relative to console window
		if (ScreenToClient(GetConsoleWindow(), &position)) {
			return position;
		}
		// if could not convert just return 0,0
		else {
			position = { 0,0 };
			return position;
		}
	}
	// if could not get position just return 0,0
	else {
		position = { 0,0 };
		return position;
	}
}
// gets the position of the mouse cursor if LMB is pressed down. (console cell location)
COORD Screen::MouseDownPosition() const {
	COORD position = { -1,-1 }; // defualt position, will be returned if no LMB down detected
	DWORD cNumRead, i; //var to hold number of inputs detected and an interator value
	INPUT_RECORD irInBuf[128]; // record of the inputs in the buffer
							   // perform read of input and print error message if error occurs in function
	if (!ReadConsoleInput(standard_in_handle_, irInBuf, 128, &cNumRead)) { std::cout << "Error reading console input\n"; }
	// iterate through the record of the input buffer
	for (i = 0; i < cNumRead; i++) {
		// if there is a LMB mouse down event then update the position COORD with its location
		if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			position = irInBuf[i].Event.MouseEvent.dwMousePosition;
		}
	}
	return position;
}

// sets the colour scheme to input arg value
void Screen::set_colour_scheme(int const attribute) const {
	// if fails to do function then exit
	if (!SetConsoleTextAttribute(standard_out_handle_, attribute)) {
		exit(1);
	}
}
// gets the current colour scheme
int Screen::get_colour_scheme() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	if (!GetConsoleScreenBufferInfo(standard_out_handle_, &buffer)) {
		exit(1);
	}
	return buffer.wAttributes;
}
