// stratagem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "maps.h"
#include "tile_map.h"
#include<iostream>
#include <windows.h>
#include "console_management.h"




int main() {
	WindowSetup(600, 600);
	/*
	std::cout << "(x, y) = (   ,   )\n";
	int count = 0;
	for (count; count < 500; count++) {
		set_colour_scheme(count);
		std::cout << count << " gives this scheme: "
			<< char(237) << char(237) << char(237)
			<< "  /" << char(238) << "\\"
			<< "  " << char(176) << char(176) << char(176)
			<< "  ,,," << std::endl;
	*/

	TileMap map(map1);
	map.print();

	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

