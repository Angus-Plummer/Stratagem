#include "stdafx.h"
#include "forest_tile.h"


ForestTile::ForestTile(){
// member data for tiles
type_ = "forest";
marker_ = char(176);// char(237);
colour_scheme_ = 32;
def_modifier_ = 1.5;
atk_modifier_ = 1;
}


ForestTile::~ForestTile()
{
}
