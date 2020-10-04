#pragma once
#include <bj/berryJam.h>

struct Consumable : public bj::ecs::Com
{
	Consumable(bj::GameObj* po);
	void onEvent(const bj::ecs::Event& e) override;
	void consume();
};
