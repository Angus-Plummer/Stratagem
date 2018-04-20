#pragma once

#include "tile.h"

// class for grass terrain type
class GrassTile : public Tile {
protected:
	// helper function returns a pointer to a copy of this object
	GrassTile* clone_impl() const;
public:
	// ctor and dtor
	GrassTile(Map &map, const Coord &map_coords);
	~GrassTile();
};

