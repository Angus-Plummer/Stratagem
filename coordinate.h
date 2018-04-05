#pragma once
#include<iostream>

// structure for 2D x, y coordinate.
struct Coord{
	// coordinate values, no trailing underscore as these are publicly accessible
	int x;
	int y;

	// ctors
	Coord() : x(0), y(0) {};
	Coord(const int _x, const int _y) : x(_x), y(_y) {};
	Coord(const Coord &coord) : x(coord.x), y(coord.y) {};

	// operator overloading
	// addition
	Coord& operator+=(const Coord &coord) { x += coord.x; y += coord.y; return *this; }
	Coord& operator+=(const int &val) { x += val; y += val; return *this; }
	Coord operator+(const Coord &coord) const { return Coord(x + coord.x, y + coord.y); }
	Coord operator+(const int &val) const { return (Coord(x + val, y + val)); }
	// subtraction
	Coord& operator-=(const Coord &coord) { x -= coord.x; y -= coord.y; return *this; }
	Coord& operator-=(const int &val) { x -= val; y -= val; return *this; }
	Coord operator-(const Coord &coord) const { return Coord(x - coord.x, y - coord.y); }
	Coord operator-(const int &val) const { return (Coord(x - val, y - val)); }
	// multiplication
	Coord& operator*=(const Coord &coord) { x *= coord.x; y *= coord.y; return *this; }
	Coord& operator*=(const int &val) { x *= val; y *= val; return *this; }
	Coord operator*(const Coord &coord) const { return Coord(x * coord.x, y * coord.y); }
	Coord operator*(const int &val) const { return (Coord(x * val, y * val)); }
	// division, will round down
	Coord& operator/=(const Coord &coord) { x /= coord.x; y /= coord.y; return *this; }
	Coord& operator/=(const int &val) { x /= val; y /= val; return *this; }
	Coord operator/(const Coord &coord) const { return Coord(x / coord.x, y / coord.y); }
	Coord operator/(const int &val) const { return (Coord(x / val, y / val)); }
	// copy assignment
	Coord& operator=(const Coord &coord) { x = coord.x; y = coord.y; return *this; }

	// friend operator overloads (defined in .cpp file)
	friend bool operator==(const Coord &lhs, const Coord &rhs);
	// inequality operator
	friend bool operator!=(const Coord &lhs, const Coord &rhs);
	// writing the coordinate to a stream
	friend std::ostream& operator<<(std::ostream &os, const Coord &coord);
};
