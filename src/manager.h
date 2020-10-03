#pragma once
#include "snake.h"

struct Manager
{
	Manager() = delete;
	static void init();
	static bool won;
	static void switchPlanes();
};
