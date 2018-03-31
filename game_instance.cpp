#include "game_instance.h"
#include "screen.h"
#include "map.h"
#include "unit.h"

GameInstance::GameInstance() {
	display_ = new Screen(700, 700); // default screen size 700x700
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
}

GameInstance::GameInstance(Screen &display): display_(&display){
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
}

// dtor : NEED TO SET THIS UP PROPERLY
GameInstance::~GameInstance(){
}

// static game instance, globally accessible
GameInstance GameInstance::instance_;

// select the target unit
void GameInstance::SelectUnit(Unit *unit) {
	DeselectUnit();
	selected_unit_ = unit;
}

// deselect the currently selected unit
void GameInstance::DeselectUnit() {
	if (selected_unit_) {
		selected_unit_->DeselectUnit();
	}
	selected_unit_ = nullptr;
}

// load in a map from the 5 premade maps
void GameInstance::LoadMap(const int &map_id) const {
	// maps identified by an integer id
	if (map_id == 1) {
		game_map_->LoadMap(raw_map1);
	}
	else if (map_id == 2) {
		game_map_->LoadMap(raw_map2);
	}
	else if (map_id == 3) {
		game_map_->LoadMap(raw_map3);
	}
	else if (map_id == 4) {
		game_map_->LoadMap(raw_map4);
	}
	else if (map_id == 5) {
		game_map_->LoadMap(raw_map5);
	}
	else{
		exit(1);
		//invalid map id, throw error...
	}
}
// update the map
void GameInstance::RenderMap() const {
	game_map_->Render();
}

// add a unit to the map
void GameInstance::AddUnit(Unit *unit, const COORD &pos) {
	game_map_->AddUnit(unit, pos);
	if (unit->get_team() == 1) {
		p1_units_.push_back(unit);
	}
	else if (unit->get_team() == 2) {
		p2_units_.push_back(unit);
	}
}


// select a unit on the map
void GameInstance::SelectUnitOnMap() const {
	Unit* unit = game_map_->SelectUnit();
	if (unit) {
		unit->SelectUnit();
	}
}
