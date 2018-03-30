#include "game_instance.h"
#include "unit.h"

GameInstance::GameInstance(Screen const &display): display_(display), game_map_(this){
	selected_unit_ = nullptr;
	player_turn_ = 1;
}


GameInstance::~GameInstance(){
}

// select the target unit
void GameInstance::SelectUnit(Unit* unit) {
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
void GameInstance::LoadMap(int const map_id) {
	if (map_id == 1) {
		game_map_.LoadMap(raw_map1);
	}
	else if (map_id == 2) {
		game_map_.LoadMap(raw_map2);
	}
	else if (map_id == 3) {
		game_map_.LoadMap(raw_map3);
	}
	else if (map_id == 4) {
		game_map_.LoadMap(raw_map4);
	}
	else if (map_id == 5) {
		game_map_.LoadMap(raw_map5);
	}
	else{
		//invalid map id, throw error...
	}
}

void GameInstance::RenderMap() const {
	game_map_.Render(display_);
}

