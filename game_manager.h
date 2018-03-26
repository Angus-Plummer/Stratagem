#pragma once
#include "tile_map.h"
class GameManager{
protected:
	TileMap game_map;
	// std::vector<Unit> team1;
	// std::vector<Unit> team2;
	// std::vector<std::shared_ptr<Selectable>>
public:
	GameManager();
	~GameManager();
};

