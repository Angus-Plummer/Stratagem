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
	Coord& operator+=(const Coord &coord);
	Coord& operator+=(const int &val);
	Coord operator+(const Coord &coord) const ;
	Coord operator+(const int &val) const;
	// subtraction
	Coord& operator-=(const Coord &coord);
	Coord& operator-=(const int &val);
	Coord operator-(const Coord &coord) const;
	Coord operator-(const int &val) const;
	// multiplication
	Coord& operator*=(const Coord &coord);
	Coord& operator*=(const int &val);
	Coord operator*(const Coord &coord) const;
	Coord operator*(const int &val) const;
	// division, will round down
	Coord& operator/=(const Coord &coord);
	Coord& operator/=(const int &val);
	Coord operator/(const Coord &coord) const;
	Coord operator/(const int &val) const;
	// copy assignment
	Coord& operator=(const Coord &coord);

	// friend operator overloads (defined in .cpp file)
	// equality operator
	friend const bool operator==(const Coord &lhs, const Coord &rhs);
	// inequality operator
	friend const bool operator!=(const Coord &lhs, const Coord &rhs);
};

