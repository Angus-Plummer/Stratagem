#pragma once
#include "stdafx.h" 

// this header file contains the raw data for the game maps
// 
// 1 = grass
// 2 = forest
// 3 = mountains
// 4 = water

std::vector<std::vector<int>> const raw_map1 = {
	{ 4, 4, 4, 4, 4, 2, 2, 2, 3, 3 },
	{ 4, 4, 4, 4, 4, 2, 2, 2, 2, 3 },
	{ 1, 1, 4, 4, 2, 2, 2, 2, 2, 3 },
	{ 1, 1, 1, 1, 2, 2, 2, 2, 2, 3 },
	{ 1, 1, 1, 1, 1, 1, 2, 2, 2, 1 },
	{ 1, 2, 2, 2, 1, 1, 1, 1, 1, 1 },
	{ 3, 2, 2, 2, 2, 2, 1, 1, 1, 1 },
	{ 3, 2, 2, 2, 2, 2, 4, 4, 1, 1 },
	{ 3, 2, 2, 2, 2, 4, 4, 4, 4, 4 },
	{ 3, 3, 2, 2, 2, 4, 4, 4, 4, 4 }
};

std::vector<std::vector<int>> const raw_map2 = {
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 1, 1, 2, 2, 2, 2, 2, 2, 1, 1 },
	{ 1, 1, 1, 2, 4, 4, 2, 1, 1, 1 },
	{ 1, 1, 1, 4, 4, 4, 4, 1, 1, 1 },
	{ 1, 1, 1, 4, 4, 4, 4, 1, 1, 1 },
	{ 1, 1, 1, 1, 4, 4, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 3, 3, 3, 3, 3, 3, 1, 1 },
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
};

std::vector<std::vector<int>> const raw_map3 = {
	{ 3, 3, 3, 2, 2, 2, 2, 3, 3, 3 },
	{ 3, 3, 1, 2, 2, 2, 2, 1, 3, 3 },
	{ 1, 1, 1, 1, 2, 2, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 2, 1, 1, 1, 3, 3, 1, 1, 1, 2 },
	{ 2, 2, 2, 3, 3, 3, 3, 2, 2, 2 },
	{ 2, 2, 2, 4, 4, 4, 4, 2, 2, 2 },
	{ 2, 2, 4, 4, 4, 4, 4, 4, 2, 2 },
	{ 2, 4, 4, 4, 4, 4, 4, 4, 4, 2 }
};

std::vector<std::vector<int>> const raw_map4 = {
	{ 2, 2, 2, 1, 4, 4, 4, 3, 3, 3 },
	{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 3 },
	{ 1, 1, 1, 3, 3, 4, 1, 2, 2, 1 },
	{ 1, 1, 1, 1, 3, 4, 1, 1, 2, 1 },
	{ 1, 2, 2, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 2, 2, 1 },
	{ 1, 2, 1, 1, 4, 3, 1, 1, 1, 1 },
	{ 1, 2, 2, 1, 4, 3, 3, 1, 1, 1 },
	{ 3, 1, 1, 1, 1, 1, 1, 1, 2, 1 },
	{ 3, 3, 3, 4, 4, 4, 1, 2, 2, 2 }
};

std::vector<std::vector<int>> const raw_map5 = {
	{ 3, 3, 3, 1, 1, 2, 2, 2, 2, 2 },
	{ 1, 3, 1, 1, 3, 2, 2, 2, 2, 2 },
	{ 1, 1, 1, 1, 3, 2, 1, 1, 2, 2 },
	{ 1, 4, 1, 1, 1, 1, 1, 1, 1, 2 },
	{ 1, 4, 4, 4, 1, 1, 4, 4, 4, 1 },
	{ 1, 4, 4, 4, 1, 1, 4, 4, 4, 1 },
	{ 2, 1, 1, 1, 1, 1, 1, 1, 4, 1 },
	{ 2, 2, 1, 1, 2, 3, 1, 1, 1, 1 },
	{ 2, 2, 2, 2, 2, 3, 1, 1, 3, 1 },
	{ 2, 2, 2, 2, 2, 1, 1, 3, 3, 3 }
};