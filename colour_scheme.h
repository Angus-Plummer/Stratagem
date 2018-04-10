#pragma once

// console colours and colour scheme, this header is included in the precompiled header as it is used by many other files

enum ConsoleColour {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	CYAN,
	DARK_RED,
	PURPLE,
	BROWN,
	LIGHT_GREY,
	DARK_GREY,
	BLUE,
	GREEN,
	LIGHT_BLUE,
	RED,
	PINK,
	YELLOW,
	WHITE
};

// struct to hold the two console colours that define a colour scheme together
struct ColourScheme {
	// console colours of background and text (no trailing underscore as these variables are public)
	ConsoleColour background_colour;
	ConsoleColour text_colour;

	// defualt ctor: white on black
	ColourScheme() : background_colour(BLACK), text_colour(WHITE) {};
	// parameterised constructor taking two console colours
	ColourScheme(const ConsoleColour &background, const ConsoleColour &text) : background_colour(background), text_colour(text) {};
	// parameterised constructor taking one console colour sets both to that colour
	ColourScheme(const ConsoleColour &colour) : background_colour(colour), text_colour(colour) {};
	// copy constructor
	ColourScheme(const ColourScheme & colour_scheme): background_colour(colour_scheme.background_colour), text_colour(colour_scheme.text_colour) {};

	// copy assignment
	ColourScheme& operator=(const ColourScheme &colour_scheme) { background_colour = colour_scheme.background_colour; text_colour = colour_scheme.text_colour; return *this; }
};

// equality operator
//bool operator==(const ColourScheme &lhs, const ColourScheme &rhs) { return lhs.background_colour == rhs.background_colour && lhs.text_colour == rhs.text_colour; }
// inequality operator
//bool operator!=(const ColourScheme &lhs, const ColourScheme &rhs) { return !(lhs == rhs); }
