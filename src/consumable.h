#pragma once
#include <bj/berryJam.h>

struct Consumable : public bj::ecs::Com
{
	uint8_t worth;
	Consumable(bj::GameObj* po, uint8_t worth_);
	void onEvent(const bj::ecs::Event& e) override;
	void consume();
};
