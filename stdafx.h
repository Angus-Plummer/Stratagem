// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#include "targetver.h"
#include "coordinate.h"
#include<string> // for object type info and for printing
#include<vector> // most commonly used container in code
#include<list> // used when container will vary in size but there are pointers to members (in pathfinding)
#include<iostream> // for outputting to console
#include<algorithm> // for operations on containers, find and erase
#include<cmath>

#include<Windows.h> // need for console and window manipulation (libraries: wincon.h, winbase.h, WinUser.h, windef.h) 
					// cant include the desired headers individually because of some interdependencies