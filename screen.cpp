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

// prints map out to the screen
void Screen::PrintMap() const {
	// map dimensions
	int map_width = game_map_->get_map_width();
	int map_height = game_map_->get_map_height();

	// print numbers above top row (in y_offset region)
	for (int map_i = 0; map_i < map_width; map_i++) {
		GoTo(map_i*tile_width_ + (int)floor(tile_width_ / 2) + map_x_offset_, (int)floor(map_y_offset_ / 2)); // places cursor centrally along width of the tile
		std::cout << char(65 + map_i); // char(65) is ASCII code for A
	}
	// print alphabetic characters left of first column (in x_offset region)
	for (int map_j = 0; map_j < game_map_->get_map_height(); map_j++) {
		GoTo((int)floor(map_x_offset_ / 2), map_j*tile_height_ + (int)floor(tile_height_ / 2) + map_y_offset_); // places cursor centrally along the height of the tile and the width of the x_offset
		std::cout << map_j; 
	}
	// print map
	// iterate over the map tiles
	for (int map_j = 0; map_j < map_height; map_j++) {
		for (int map_i = 0; map_i < map_width; map_i++) {
			Render(game_map_->get_tile(map_i, map_j));
		}
	}
}


void Screen::SetMap(std::shared_ptr<TileMap> const game_map) {
	game_map_ = game_map;
}

// updates the rendering of argument tile object
void Screen::Render(std::shared_ptr<GameObject> const tile) const{
	int original_colour_scheme = get_colour_scheme(); // save original colour scheme to set back later
	if (tile == game_map_->get_tile(tile->get_map_coords().X, tile->get_map_coords().Y)) {
		// set colour scheme for tyle type
		set_colour_scheme(tile->get_colour_scheme());
		// iterate over console cells for tile width and height
		for (int tile_j = 0; tile_j < tile_height_; tile_j++) {
			for (int tile_i = 0; tile_i < tile_width_; tile_i++) {
				// set console cursor position to given tile component
				GoTo(map_x_offset_ + tile->get_map_coords().X * tile_width_ + tile_i, map_y_offset_ + tile->get_map_coords().Y * tile_height_ + tile_j);
				// output the tile marker
				std::cout << tile->get_marker();
			}
		}
		set_colour_scheme(original_colour_scheme);
		return;
	}
}

// select an object on the screen. Priority is: ui object > unit > terrain tile.
std::shared_ptr<GameObject> Screen::Select() const {
	// get location of mouse down ({-1,-1} if no mouse down detected)
	COORD event_location = MouseDownPosition();
	//---ui buttons
	//for (auto iterator = interface_objects_.begin(); iterator != interface_objects_.end(); iterator++) {
	//}
	//---units
//	for (auto iterator = team1.begin() ...) {
//	}
//	for (auto iterator = team2.begin() ..) {
//	}
	//---map tiles
	// map dimensions
	int map_width = game_map_->get_map_width();
	int map_height = game_map_->get_map_height();
	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			COORD tile_coord = {map_x_offset_ + i*tile_width_, map_y_offset_ + j*tile_height_}; // console cell coord of top left of tile
			if (event_location.X >= tile_coord.X && event_location.X < tile_coord.X + tile_width_  // if clicked cell is between tile x range
				&& event_location.Y >= tile_coord.Y && event_location.Y < tile_coord.Y + tile_height_) { // and between tile y range
				return game_map_->get_tile(i, j);
			}
		}
	}
	// if found nothing at location of click then returns a null pointer
	return std::shared_ptr<GameObject>(nullptr);
}

