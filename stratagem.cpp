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

// Sad realities: Code is highly coupled. Difficult to change this now without major restructuring of the code, implementing e.g. the component pattern

#include "stdafx.h"
#include "game_main_menu.h"
#include "game_instance.h"
#include "screen.h"
#include "map.h"
#include "move_sequence.h"
#include "raw_maps.h"
#include "unit_archer.h"
#include "unit_warrior.h"
#include "unit_rogue.h"
#include "button.h"
#include "menu.h"

GameObject* selected;

int main() {

	
	// 700x700 pixel window
	Screen display(610, 550);
	GameMainMenu pre_game_menu(display);
	pre_game_menu.ShowTitleScreen();
	Sleep(2000);
	display.Clear();

	GameInstance* the_game = new GameInstance(display);

	the_game->LoadMap(1);

	Unit* warrior1 = new Warrior(1);
	Unit* archer1 = new Archer(1);
	Unit* rogue1 = new Rogue(1);

	Unit* warrior2 = new Warrior(2);
	Unit* archer2 = new Archer(2);
	Unit* rogue2 = new Rogue(2);

	GameInstance::instance().AddUnit(warrior1, Coord{ 0,4 });
	GameInstance::instance().AddUnit(archer1, Coord{ 0,5 });
	GameInstance::instance().AddUnit(rogue1, Coord{ 0,3 });

	GameInstance::instance().AddUnit(warrior2, Coord{ 9,5 });
	GameInstance::instance().AddUnit(archer2, Coord{ 9,4 });
	GameInstance::instance().AddUnit(rogue2, Coord{ 9,6 });

	GameInstance::instance().Run();
	
	/*
	// print out all the colours
	for (int i = 0; i < 256; i++) {
		GameInstance::instance().get_display().set_colour_scheme(15);
		std::cout << i << " = ";
		GameInstance::instance().get_display().set_colour_scheme(i);
		int row = GameInstance::instance().get_display().CursorPosition().Y;
		GameInstance::instance().get_display().GoTo(6, row);
		std::cout << char(176);
		GameInstance::instance().get_display().GoTo(8, row);
		std::cout << char(177);
		GameInstance::instance().get_display().GoTo(10, row);
		std::cout << char(178) << std::endl;
	}
	
	for (int i = 0; i < 256; i++) {
		std::cout << i << " " << char(i) << std::endl;
	}
	*/
	
	
	
	display.GoTo(Coord{ 0, 25 });
	
	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

