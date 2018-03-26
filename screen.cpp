#include "stdafx.h"
#include "screen.h"


Screen::Screen(int const width, int const height) : console_window_(GetConsoleWindow()),
													hStdin_(GetStdHandle(STD_INPUT_HANDLE)),
													hStdout_(GetStdHandle(STD_OUTPUT_HANDLE))
{
	RECT r; // rectangle to store window position and dimensions
	// disable quick edit mode so that mouse input works
	fdwMode_ = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hStdin_, fdwMode_)) { exit(1); }
	// enable mouse input events
	fdwMode_ = ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin_, fdwMode_)) { exit(1); }

	//stores the console's current dimensions
	GetWindowRect(console_window_, &r);
	// resize window height and width
	MoveWindow(console_window_, r.left, r.top, width, height, TRUE);
}


Screen::~Screen(){
}

void Screen::Clear() {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hStdout_, &buffer)) {
		exit(1);
	}
	DWORD length = buffer.dwSize.X * buffer.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hStdout_, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hStdout_, buffer.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hStdout_, topLeft);
}

// get console screen width and height in terms of rows and columns of console cells
int Screen::Width() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	if (!GetConsoleScreenBufferInfo(hStdout_, &buffer)) {
		exit(1);
	}
	return buffer.srWindow.Right - buffer.srWindow.Left;

}
int Screen::Height() const{
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	if (!GetConsoleScreenBufferInfo(hStdin_, &buffer)) {
		exit(1);
	}
	return buffer.srWindow.Bottom - buffer.srWindow.Top;
}

// get and set the position of the console cursor
void Screen::GoTo(int const x, int const y) const {
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hStdout_, coord);
}
COORD Screen::CursorPosition() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	// attempt to get buffer info
	if (GetConsoleScreenBufferInfo(hStdout_, &buffer)) {
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
	if (!ReadConsoleInput(hStdin_, irInBuf, 128, &cNumRead)) { std::cout << "Error reading console input\n"; }
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
	if (!SetConsoleTextAttribute(hStdout_, attribute)) {
		exit(1);
	}
}
// gets the current colour scheme
int Screen::get_colour_scheme() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	if (!GetConsoleScreenBufferInfo(hStdout_, &buffer)) {
		exit(1);
	}
	return buffer.wAttributes;
}

// prints map out to the screen
void Screen::PrintMap() const {
	int original_colour_scheme = get_colour_scheme(); // save original colour scheme to set back later
	// offset from edge of screen to make space for cell labels
	int x_offset = 3;
	int y_offset = 1;
	// map dimensions
	int map_width = game_map_->get_map_width();
	int map_height = game_map_->get_map_height();
	// tile dimensions
	int tile_width = game_map_->get_tile_width();
	int tile_height = game_map_->get_tile_height();

	// print numbers above top row (in y_offset region)
	for (int map_i = 0; map_i < map_width; map_i++) {
		GoTo(map_i*game_map_->get_tile_width() + (int)floor(tile_width / 2) + x_offset, (int)floor(y_offset / 2)); // places cursor centrally along width of the tile
		std::cout << map_i;
	}
	// print alphabetic characters left of first column (in x_offset region)
	for (int map_j = 0; map_j < game_map_->get_map_height(); map_j++) {
		GoTo((int)floor(x_offset / 2), map_j*tile_height + (int)floor(tile_height / 2) + y_offset); // places cursor centrally along the height of the tile and the width of the x_offset
		std::cout << char(65 + map_j); // char(65) is ASCII code for A
	}
	// print map
	// iterate over the map tiles
	for (int map_j = 0; map_j < map_height; map_j++) {
		for (int map_i = 0; map_i < map_width; map_i++) {
			// set colour scheme for tyle type
			set_colour_scheme((game_map_->get_tile(map_i, map_j)->get_colour_scheme()));
			// iterate over console cells for tile width and height
			for (int tile_j = 0; tile_j < tile_height; tile_j++) {
				for (int tile_i = 0; tile_i < tile_width; tile_i++) {
					// set console cursor position to given tile component
					GoTo(x_offset + map_i * tile_width + tile_i, y_offset + map_j * tile_height + tile_j);
					// output the tile marker
					std::cout << game_map_->get_tile(map_i,map_j)->get_marker();

				}
			}
		}
	}
	// set colour scheme back to original
	set_colour_scheme(original_colour_scheme);
}


void Screen::SetMap(std::shared_ptr<TileMap> const game_map) {
	game_map_ = game_map;
}

