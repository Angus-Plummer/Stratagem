#pragma once
#include<Windows.h>
#include<iostream>
#include<WindowsX.h>

// sets the window up for mouse interaction
void WindowSetup(int const width, int const height);
// clears the screen
void ClearScreen();

// get and set the position of the console cursor
void set_console_cursor_position(int const x, int const y);
COORD get_console_cursor_position();

// gets the position of the mouse cursor (pixel location, relative to window)
POINT get_mouse_cursor_position();
// gets the position of the mouse cursor if LMB is pressed down. (console cell location)
COORD get_mouse_cursor_down_pos();

// paints a console cell
void set_colour_scheme(int const attribute);
int get_colour_scheme();