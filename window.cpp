#include "stdafx.h"
#include "Window.h"
#include "colour_scheme.h"

// ---------- ctor and dtor ---------- //

// ctor (creates window of specific pixel dimensions with locked size, removes scrollbar, enables mouse input)
Window::Window(const int &width, const int &height) : console_window_(GetConsoleWindow()),
standard_in_handle_(GetStdHandle(STD_INPUT_HANDLE)),
standard_out_handle_(GetStdHandle(STD_OUTPUT_HANDLE))
{
	RECT r; // rectangle to store window position and dimensions
	// disable quick edit mode so that mouse input works
	fdwMode_ = ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(standard_in_handle_, fdwMode_);
	// enable mouse input events
	fdwMode_ = ENABLE_MOUSE_INPUT;
	SetConsoleMode(standard_in_handle_, fdwMode_);

	//stores the console's current dimensions
	GetWindowRect(console_window_, &r);
	// resize window height and width
	MoveWindow(console_window_, r.left, r.top, width, height, TRUE);
	// make cursor invisible
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(standard_out_handle_, &cursor_info);
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(standard_out_handle_, &cursor_info);

	// disable console window resizing (resizing caused rendering issues until everything was rerendered)
	LONG window_style = GetWindowLongPtr(console_window_, GWL_STYLE);
		// grey out and disable maximise button and manual drag resizing
	window_style = window_style & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
	SetWindowLongPtr(console_window_, GWL_STYLE, window_style);

	// set console buffer size to the number of cells visible in the window.
		// get buffer info
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(standard_out_handle_, &buffer);
	// coord to hold new buffer size, set to the size of the console screen buffer
	COORD console_buffer_size;
	console_buffer_size.X = buffer.dwSize.X;
	console_buffer_size.Y = Height() +1;
	// set the buffer size
	SetConsoleScreenBufferSize(standard_out_handle_, console_buffer_size);
}

// dtor
Window::~Window(){}

// ---------- public functions ---------- //

// get console window width in terms of columns of console cells
const int Window::Width() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(standard_out_handle_, &buffer);
	return buffer.srWindow.Right - buffer.srWindow.Left;

}

// get console window height in terms of rows of console cells
const int Window::Height() const{
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(standard_out_handle_, &buffer);
	return buffer.srWindow.Bottom - buffer.srWindow.Top;
}

// sets the colour scheme
void Window::set_colour_scheme(const ColourScheme &colour_scheme) const {
	// determine the id number of the colour scheme (hexadecimal, first digit background, second text)
	int colour_scheme_hex = colour_scheme.background_colour * 16 + colour_scheme.text_colour;
	// set the console text attribute
	SetConsoleTextAttribute(standard_out_handle_, colour_scheme_hex);
}

// gets the current colour scheme of the console output
const ColourScheme Window::get_colour_scheme() const {
	// get console window buffer info
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(standard_out_handle_, &buffer);
	// integer value of the colour scheme (hexadecimal first digit background, second digit text)
	int colour_scheme = buffer.wAttributes;
	// divide by 16 and ignore remainder to get background colour
	ConsoleColour background_colour = static_cast<ConsoleColour>(colour_scheme / 16);
	// text colour is remainder of the division by 16
	ConsoleColour text_colour = static_cast<ConsoleColour>(colour_scheme % 16);
	return ColourScheme(background_colour, text_colour);
}

// get the position of the console cursor
const Coord Window::CursorPosition() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	// get buffer info
	GetConsoleScreenBufferInfo(standard_out_handle_, &buffer);
	return Coord(buffer.dwCursorPosition.X, buffer.dwCursorPosition.Y);
}

// set the position of the console cursor
void Window::GoTo(const Coord &in_coord) const {
	std::cout.flush();
	COORD coord = { (SHORT)in_coord.x, (SHORT)in_coord.y };
	SetConsoleCursorPosition(standard_out_handle_, coord);
}

// clears the window
void Window::Clear() const {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	COORD top_left = { 0, 0 };

	// flush cout buffer so no text gets written after clearing Window
	std::cout.flush();

	// Figure out the current width and height of the console window
	GetConsoleScreenBufferInfo(standard_out_handle_, &buffer);
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

// gets the position (console cell location) of the mouse cursor if LMB is pressed clicked down. (acts like detecting a mouse up event)
const Coord Window::MouseDownPosition() const {
	bool detected_down = false;
	Coord down_position = { -1,-1 };// defualt position, will be returned if no LMB down detected
	DWORD num_read, i; //var to hold number of inputs detected and an interator value
	INPUT_RECORD buffer_input_record[128]; // record of the inputs in the buffer
	// perform read of input
	ReadConsoleInput(standard_in_handle_, buffer_input_record, 128, &num_read);
	// iterate through the record of the input buffer
	for (i = 0; i < num_read; i++) {
		// if there is a LMB mouse down event then update the position COORD with its location
		if (buffer_input_record[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			detected_down = true;
			down_position = { buffer_input_record[i].Event.MouseEvent.dwMousePosition.X, buffer_input_record[i].Event.MouseEvent.dwMousePosition.Y };
		}
	}

	// if a mouse down has been detected then wait until the mouse comes up before returning (i.e. its no longer down)
	if (detected_down) {
		while (detected_down == true) {
			detected_down = false; // set back to false, if it doesnt get set to true through the loop then continue
			// perform read of input
			ReadConsoleInput(standard_in_handle_, buffer_input_record, 128, &num_read);
			// iterate through the record of the input buffer
			for (i = 0; i < num_read; i++) {
				// if there is a LMB mouse down event then update the position COORD with its location and set detected down to true
				if (buffer_input_record[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					detected_down = true;
					// update the position to the most recent position
					down_position = { buffer_input_record[i].Event.MouseEvent.dwMousePosition.X, buffer_input_record[i].Event.MouseEvent.dwMousePosition.Y };
				}
			}
		}
	}
	return down_position;
}

// wait for mouse input (function will keep looping until a click is detected)
void Window::WaitForMouse() const {
	Coord mouse_down_pos{ -1,-1 }; // {-1,-1 is returned from MouseDownPosition if no click is detected}
								   // keep getting the mouse down position until it returns an a value corresponding to a click
	while (mouse_down_pos == Coord{ -1,-1 }) {
		mouse_down_pos = MouseDownPosition();
	}
}