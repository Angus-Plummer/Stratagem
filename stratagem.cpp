// stratagem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "screen.h"

int main() {
	Screen display(700, 700);

	std::shared_ptr<TileMap> map(new TileMap(map1));
	display.SetMap(map);
	display.PrintMap();

	std::cout << std::endl;
	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

