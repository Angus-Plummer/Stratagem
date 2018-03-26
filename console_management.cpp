#include "stdafx.h"
#include "console_management.h"

// Reconfigures the console window to desired settings.
void WindowSetup(int const width, int const height) {
	HWND console = GetConsoleWindow(); // handle for console window
	RECT r; // rectangle to store window position and dimensions
	DWORD fdwMode; // Holds the information on the input/output mode of the console
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	// disable quick edit mode so that mouse input works
	fdwMode = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hStdin, fdwMode)) { exit(1); }
	// enable mouse input events
	fdwMode = ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode)) { exit(1); }

	//stores the console's current dimensions
	GetWindowRect(console, &r);
	// resize window height and width
	MoveWindow(console, r.left, r.top, width, height, TRUE); // 400 width, 600 height
}

void ClearScreen(){
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

// x is the column, y is the row. The origin (0,0) is top-left.
void set_console_cursor_position(int const x, int const y){
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}
// function to return console cursor position
COORD get_console_cursor_position(){
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	// attempt to get buffer info
	if (GetConsoleScreenBufferInfo(hOut, &buffer_info)){
		return buffer_info.dwCursorPosition;
	}
	// if function fails then return -1, -1
	else{
		COORD fail = { -1, -1 };
		return fail;
	}
}
// returns a POINT of the cursors position relative to top left corner of console window
POINT get_mouse_cursor_position() {
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

// function to return location of mouse if LMB is pressed down
// location is in terms of console cells. If LMB not pressed, returns COORD {-1,-1}
COORD get_mouse_cursor_down_pos() {
	COORD position = { -1,-1 }; // defualt position, will be returned if no LMB down detected
	DWORD cNumRead, i; //var to hold number of inputs detected and an interator value
	INPUT_RECORD irInBuf[128]; // record of the inputs in the buffer
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); // handle for console input buffer
	// perform read of input and print error message if error occurs in function
	if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead)) { std::cout << "Error reading console input\n"; }
	// iterate through the record of the input buffer
	for (i = 0; i < cNumRead; i++) {
		// if there is a LMB mouse down event then update the position COORD with its location
		if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			position = irInBuf[i].Event.MouseEvent.dwMousePosition;
		}
	}
	return position;
}

// sets the colour scheme
void set_colour_scheme(int const attribute) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); // handle for console output
	SetConsoleTextAttribute(hStdout, attribute);
}
int get_colour_scheme() {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); // handle for console output
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	if (GetConsoleScreenBufferInfo(hStdout, &buffer_info)) {
		return buffer_info.wAttributes;
	}
}
