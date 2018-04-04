#include "coordinate.h"

// equality operator
bool operator==(const Coord &lhs, const Coord &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
// inequality operator
bool operator!=(const Coord &lhs, const Coord &rhs) { return !(lhs == rhs); }

// writing the coordinate to a stream
std::ostream& operator<<(std::ostream &os, const Coord &coord) {
	os << "(" << coord.x << ", " << coord.y << ")";
	return os;
}