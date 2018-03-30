// stratagem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game_instance.h"
#include "screen.h"
#include "raw_maps.h"
#include "unit_archer.h"
#include "unit_knight.h"
#include "unit_priest.h"

GameObject* selected;

int main() {
	// 700x700 pixel window
	Screen display(700, 700);
	GameInstance* the_game = new GameInstance(display);	

	the_game->LoadMap(1);
	/*
	Unit* knight = new Knight(the_game, map1, 1);
	Unit* archer = new Archer(the_game, map1, 2);
	COORD pos = { 3,5 };
	COORD pos2 = { 4,5 };
	map1->AddUnit(knight, pos);
	map1->AddUnit(archer, pos2);
	*/
	the_game->RenderMap();
	/*
	display.GoTo(0, 32);

	std::vector<Tile*> reachable_tiles = archer->ReachableTiles();
	for (auto iter = reachable_tiles.begin(); iter != reachable_tiles.end(); iter++) {
		(*iter)->set_highlighted(true);
	}
	
	
	std::cout << "selected: ";
	while (true) {
		GameObject* select_new = map1->Select(display);
		if (select_new != nullptr) {
			if(selected != nullptr){
				selected->set_selected(false);
				selected->Render(display);
			}
			selected = select_new;
			selected->set_selected(true);
			selected->Render(display);
			display.GoTo(10, 32);
			std::cout << "                    ";
			display.GoTo(10, 32);
			std::cout << char(65 + selected->get_map_coords().X) << selected->get_map_coords().Y << " : " << selected->get_type();
		}
	}
	
	while (knight->is_alive() && archer->is_alive()) {
		archer->Attack(knight);
		knight->Attack(archer);
		display.GoTo(0, 32);
		std::cout << "knight 1 : " << knight->get_current_hp() << "/" << knight->get_max_hp() << "hp  ";
		display.GoTo(0, 33);
		std::cout << "archer 2 : " << archer->get_current_hp() << "/" << archer->get_max_hp() << "hp  ";
		Sleep(1000);
	}
	*/
	
	display.GoTo(0, 35);
	std::cout << std::endl;
	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

