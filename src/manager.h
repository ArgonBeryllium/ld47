#pragma once
#include "snake.h"

struct Manager
{
	Manager() = delete;
	void init();
	Snake* snake;
	bool won;
};
