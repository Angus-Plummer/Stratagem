// stratagem.cpp : Defines the entry point for the console application.
//
// TODO: include proper error handling ( usually needed in places with exit(1) )
//		 use smart pointers instead of regular pointers where appropriate
//		 implement rule of 3 (or 5) : make copy and move constructors and assignment operators for each class that needs it
//		 check where passing pointers to functions and see if just a reference will do instead
//		 make the constructors and destructors of abstracrt classes protected
//		 go through classes and determine whether public, protected, and private status are right for members
//		 try using switch(condition){ case (condition1): ... case (condition2) ... instead of messy if else if blocks
//		 find where enums may be appropriate as a way of giving different objects ids
//		 check whether it is approriate to include accessors, mutators and other simple function definitions in the header files or if they should actually be in cpp files
//		 check whether all accessors and mutators are actually used and appropriate. If not then remove.
//		 go through all functions and check whether I should be returning a const reference instead of reference so that the original object data cannot be modified

// Sad realities: Code is highly coupled. Can't change this now without major restructuring of the code, implementing e.g. the component pattern

// Next: make class(es) to help with making a context menu, implement context menu

#include "stdafx.h"
#include "game.h"
#include "screen.h"
#include "map.h"
#include "move_sequence.h"
#include "raw_maps.h"
#include "unit_archer.h"
#include "unit_knight.h"
#include "unit_priest.h"
#include "button.h"
#include "menu.h"

GameObject* selected;

int main() {
	
	// 700x700 pixel window
	Screen display(1200, 700);
	Game* the_game = new Game(display);

	the_game->LoadMap(1);

	Unit* knight1 = new Knight(1);
	Unit* archer1 = new Archer(1);
	Unit* priest1 = new Priest(1);

	Unit* knight2 = new Knight(2);
	Unit* archer2 = new Archer(2);
	Unit* priest2 = new Priest(2);

	the_game->AddUnit(knight1, Coord{ 3,5 });
	the_game->AddUnit(archer1, Coord{ 4,5 });
	the_game->AddUnit(priest1, Coord{ 2,4 });

	the_game->AddUnit(knight2, Coord{ 5,4 });
	the_game->AddUnit(archer2, Coord{ 6,5 });
	the_game->AddUnit(priest2, Coord{ 6,4 });
	
	the_game->RenderMap();
	
	/*
	for (int i = 0; i < 256; i++) {
		Game::instance().get_display().set_colour_scheme(15);
		std::cout << i << " = ";
		Game::instance().get_display().set_colour_scheme(i);
		int row = Game::instance().get_display().CursorPosition().Y;
		Game::instance().get_display().GoTo(6, row);
		std::cout << char(176);
		Game::instance().get_display().GoTo(8, row);
		std::cout << char(177);
		Game::instance().get_display().GoTo(10, row);
		std::cout << char(178) << std::endl;
	}
	*/
	
	
	// select and repeatedly move a unit
	while (true) {
		Coord mouse_down_pos = Game::instance().get_display().MouseDownPosition();
		if (mouse_down_pos.x != -1 && mouse_down_pos.y != -1) {
			Game::instance().HandleLeftMouseButtonDown(mouse_down_pos);
		}
	}
	
	/*
	while (knight->is_alive() && archer->is_alive()) {
		archer->Attack(knight);
		knight->Attack(archer);
		display.GoTo({0, 32});
		std::cout << "knight 1 : " << knight->get_current_hp() << "/" << knight->get_max_hp() << "hp  ";
		display.GoTo({0, 33});
		std::cout << "archer 2 : " << archer->get_current_hp() << "/" << archer->get_max_hp() << "hp  ";
		Sleep(1000);
	}
	*/
	
	display.GoTo(Coord{ 0, 35 });
	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

