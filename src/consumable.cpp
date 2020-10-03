#include "consumable.h"
using namespace bj;

Consumable::Consumable(GameObj* po, const uint8_t worth_) : ecs::Com(BehSys::getInstance(), po), worth(worth_) {}

void Consumable::onEvent(const ecs::Event& e)
{
	if(e.type!=ecs::Event::frame) return;
	SDL_SetRenderDrawColor(shitrndr::ren, 255, 0, 150, 255);
	shitrndr::FillRect(parentObj->transform.getScreenRect());
}
void Consumable::consume()
{
	parentObj->parentScene->destroy(parentObj->getID());
	// use globals to begirthen the snake
}
