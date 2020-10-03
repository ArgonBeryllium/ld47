#pragma once
#include <bj/berryJam.h>
using namespace bj;

struct Snake : public ecs::Com 
{
	const uint8_t length = 4;
	GameObj** tail;
	Snake(GameObj* po, const uint8_t& length_);

	void start();
	void onEvent(const ecs::Event& e) override;
	bool canMove(const int& x, const int& y);
	void move(const int& x, const int& y);
};
