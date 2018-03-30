#pragma once

#include "stdafx.h"
#include "screen.h"
#include "map.h"

class GameInstance{
protected:
	Screen display_;
	Map game_map_;
	std::vector<Unit*> p1_units_;
	std::vector<Unit*> p2_units_;
	Unit* selected_unit_;
	int player_turn_;
	// UIObject : actions menu
public:
	GameInstance(Screen const &display);
	~GameInstance();

	// select the target unit
	void SelectUnit(Unit* unit);
	// unselect the currently selected unit
	void GameInstance::DeselectUnit();

	// load in a map from the 5 premade maps
	void LoadMap(int const map_id);
	// render the whole map
	void RenderMap() const;


};
