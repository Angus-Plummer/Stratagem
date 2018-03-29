// stratagem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "screen.h"

std::shared_ptr<GameObject> selected;

int main() {
	// 700x700 pixel window
	Screen display(700, 700);

	std::shared_ptr<TileMap> map1(new TileMap(map1_raw.size(), map1_raw[0].size(), 3));
	
	map1->LoadMap(map1_raw);

	display.SetMap(map1);
	display.PrintMap();
	display.GoTo(0, 32);
	std::cout << "selected: ";
	while (true) {
		std::shared_ptr<GameObject> select_new = display.Select();
		if (select_new) {
			if(selected){
				selected->set_selected(false);
				//display.Render(selected);
			}
			selected = select_new;
			selected->set_selected(true);
			//display.Render(selected);
			display.PrintMap();
			display.GoTo(10, 32);
			std::cout << "                    ";
			display.GoTo(10, 32);
			std::cout << char(65 + selected->get_map_coords().X) << selected->get_map_coords().Y << " : " << selected->get_type();
		}
	}

	std::cout << std::endl;
	std::cout << "finished" << std::endl;
	std::cin.ignore();
	return 0;
}

