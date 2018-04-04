#include "game.h"
#include "screen.h"
#include "map.h"
#include "tile.h"
#include "unit.h"

Game::Game() {
	display_ = new Screen(700, 700); // default screen size 700x700
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
}

Game::Game(Screen &display): display_(&display){
	game_map_ = new Map(); // create empty map
	selected_unit_ = nullptr; // no unit selected
	player_turn_ = 1; // start with player 1's turn
	instance_ = *this; // set the game instance to be this newly constructed game instance
}

// dtor : NEED TO SET THIS UP PROPERLY
Game::~Game(){
}

// static game instance, globally accessible
Game Game::instance_;

// select the target unit
void Game::SelectUnit(Unit *unit) {
	DeselectUnit();
	selected_unit_ = unit;
}

// deselect the currently selected unit
void Game::DeselectUnit() {
	if (selected_unit_) {
		selected_unit_->DeselectUnit();
	}
	selected_unit_ = nullptr;
}

// load in a map from the 5 premade maps
void Game::LoadMap(const int &map_id) const {
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
void Game::RenderMap() const {
	game_map_->Render();
}

// add a unit to the map
void Game::AddUnit(Unit *unit, const Coord &pos) {
	game_map_->AddUnit(unit, pos);
	if (unit->get_team() == 1) {
		p1_units_.push_back(unit);
	}
	else if (unit->get_team() == 2) {
		p2_units_.push_back(unit);
	}
}


// select a unit on the map
void Game::SelectUnitOnMap() const {
	Unit* unit = game_map_->SelectUnit();
	if (unit) {
		unit->SelectUnit();
	}
}

// handles a mouse down event (i.e. the user clicking somewhere on the screen)
void Game::HandleLeftMouseButtonDown(const Coord &location) { // Coord location is in terms of console cells over whole display
	// check for no mouse click returns a Coord of {-1, -1}
	if (location.x == -1  && location.y == -1  ) {
		// throw error as should not be able to get here
		exit(1);
	}
	// get the tile at the location (if any), will handle unit later
	Tile* tile = game_map_->GetTileFromConsoleCoord(location);
	// if a tile was selected... (i.e. the click was on the map)
	if (tile) {
		// get unit on tile if any ( will be nullptr if no unit)
		Unit* unit = game_map_->GetUnit(tile->get_map_coords());
		// ... and there is a unit on the tile...
		if (unit){
			// ... and there is no unit currently selected...
			if (!selected_unit_) {
				unit->SelectUnit();
			}
			else {
				// ...and the currently selected unit is trying to attack..
				// if (selected_unit->currently_attacking) {
					// and can attack that unit..
					//if ( selected_unit_->CanAttack(unit) ) {
						// then handle the attack and deselect the unit as its go is now over
					//}
					// ... but cannot attack that unit
					//else{
						// either do nothing or remove the attacking status of the currently selected unit
				// .. and the currently selected unit is trying to move
				// if (selected_unit->currently_moving){
					// then deselect the currently selected unit
					selected_unit_->DeselectUnit();
					unit->SelectUnit();
			}
		}
		// ...and there is no unit on the tile...
		else{
			// ... and there is a unit currently selected..
			if (selected_unit_) {
				// ...and the currently selected unit can reach the tile and has yet to move..
				if (selected_unit_->CanReach(tile) && selected_unit_->CanMove() && !unit) {
					// move the currently selected unit to the tile
					selected_unit_->MoveTo(tile);
				}
				// ... and the currently selected unit either cant move or cant reach the tile
				else {
					// deselect the currently selected unit
					selected_unit_->DeselectUnit();
				}
			}
		}
		
	}
	// if the mouse click did not occur on the map...
	else {

	}

}
