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

GameObject* selected;

int main() {
	
	// 700x700 pixel window
	Screen display(700, 700);
	Game* the_game = new Game(display);	

	std::string statement = "huzzah";

	Button print_statement(Coord(5, 5), "click here", [&statement]() {Game::instance().get_display().GoTo(Coord{ 10,10 }); std::cout << statement; });
	print_statement.Render();
	Sleep(1000);
	print_statement.Interact();
	Sleep(1000);

	/*
	the_game->LoadMap(1);

	Unit* knight = new Knight(1);
	Unit* archer = new Archer(2);
	Coord pos = { 3,5 };
	Coord pos2 = { 4,5 };
	the_game->AddUnit(knight, pos);
	the_game->AddUnit(archer, pos2);
	
	the_game->RenderMap();
	
	display.GoTo({0,32});
	std::cout << "selected: ";
	*/
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
	
	/*
	// select and repeatedly move a unit
	while (true) {
		Coord mouse_down_pos = Game::instance().get_display().MouseDownPosition();
		if (mouse_down_pos.x != -1 && mouse_down_pos.y != -1) {
			Game::instance().HandleLeftMouseButtonDown(mouse_down_pos);
		}
	}
	*/
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
	
	std::cout << std::endl;
	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

