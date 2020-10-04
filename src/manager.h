#pragma once
#include "snake.h"
#include "mapLoader.h"

struct Manager
{
	Manager() = delete;

	static v2f wPos, wScl;
	static MapLoader* ml;
	static Snake* snake;
	static bool won;
	static float lGap;


	static uint8_t score, cicl;

	static void init();
	static void loadLevel(uint8_t i);
	static void win();
	static void consume();
	static void switchPlanes();
};
