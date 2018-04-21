#include "coordinate.h"

// addition
Coord& Coord::operator+=(const Coord &coord) { x += coord.x; y += coord.y; return *this; }
Coord& Coord::operator+=(const int &val) { x += val; y += val; return *this; }
Coord Coord::operator+(const Coord &coord) const { return Coord(x + coord.x, y + coord.y); }
Coord Coord::operator+(const int &val) const { return (Coord(x + val, y + val)); }

// subtraction
Coord& Coord::operator-=(const Coord &coord) { x -= coord.x; y -= coord.y; return *this; }
Coord& Coord::operator-=(const int &val) { x -= val; y -= val; return *this; }
Coord Coord::operator-(const Coord &coord) const { return Coord(x - coord.x, y - coord.y); }
Coord Coord::operator-(const int &val) const { return (Coord(x - val, y - val)); }

// multiplication
Coord& Coord::operator*=(const Coord &coord) { x *= coord.x; y *= coord.y; return *this; }
Coord& Coord::operator*=(const int &val) { x *= val; y *= val; return *this; }
Coord Coord::operator*(const Coord &coord) const { return Coord(x * coord.x, y * coord.y); }
Coord Coord::operator*(const int &val) const { return (Coord(x * val, y * val)); }

// division, will round down
Coord& Coord::operator/=(const Coord &coord) { x /= coord.x; y /= coord.y; return *this; }
Coord& Coord::operator/=(const int &val) { x /= val; y /= val; return *this; }
Coord Coord::operator/(const Coord &coord) const { return Coord(x / coord.x, y / coord.y); }
Coord Coord::operator/(const int &val) const { return (Coord(x / val, y / val)); }

// copy assignment
Coord& Coord::operator=(const Coord &coord) { x = coord.x; y = coord.y; return *this; }

// equality operator
const bool operator==(const Coord &lhs, const Coord &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
// inequality operator
const bool operator!=(const Coord &lhs, const Coord &rhs) { return !(lhs == rhs); }