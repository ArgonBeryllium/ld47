#pragma once
#include <bj/berryJam.h>

struct Gate : public bj::SpriteRen
{
	Gate(bj::GameObj* po);

	void onEvent(const bj::ecs::Event& e) override;
};
