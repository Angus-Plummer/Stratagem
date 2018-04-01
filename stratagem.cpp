// stratagem.cpp : Defines the entry point for the console application.
//
// TODO: include proper error handling ( usually needed in places with exit(1)
//		 use smart pointers instead of regular pointers where appropriate
//		 implement rule of 3 (or 5) : make copy and move constructors and assignment operators for each class that needs it
//		 check where passing pointers to functions and see if just a reference will do instead

// make class(es) to help with making a context menu, implement context menu

#include "stdafx.h"
#include "game.h"
#include "screen.h"
#include "map.h"
#include "move_sequence.h"
#include "raw_maps.h"
#include "unit_archer.h"
#include "unit_knight.h"
#include "unit_priest.h"

GameObject* selected;

int main() {
	// 700x700 pixel window
	Screen display(700, 700);
	Game* the_game = new Game(display);	

	the_game->LoadMap(1);

	Unit* knight = new Knight(1);
	Unit* archer = new Archer(2);
	COORD pos = { 3,5 };
	COORD pos2 = { 4,5 };
	the_game->AddUnit(knight, pos);
	the_game->AddUnit(archer, pos2);
	
	the_game->RenderMap();
	
	display.GoTo(0, 32);
	//std::cout << "selected: ";
	for (int i = 0; i < 256; i++) {
	//	std::cout<< i << " = " << char(i) << std::endl;
	}

	/*
	while (true) {
		pos.X -= 1;
		knight->AnimateMovement(the_game->get_map().GetTile(pos));
		knight->set_map_coords(pos);
		pos.Y += 1;
		knight->AnimateMovement(the_game->get_map().GetTile(pos));
		knight->set_map_coords(pos);
		pos.X += 1;
		knight->AnimateMovement(the_game->get_map().GetTile(pos));
		knight->set_map_coords(pos);
		pos.Y -= 1;
		knight->AnimateMovement(the_game->get_map().GetTile(pos));
		knight->set_map_coords(pos);
	}
	knight->Render();
	*/
	
	// select and repeatedly move a unit
	while (true) {
		COORD mouse_down_pos = Game::instance().get_display().MouseDownPosition();
		if (mouse_down_pos.X != -1 && mouse_down_pos.Y != -1) {
			Game::instance().HandleMouseDown(mouse_down_pos);
		}
	}
	
	/*
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

