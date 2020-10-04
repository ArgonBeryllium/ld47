#include "gate.h"
using namespace bj;

SDL_Texture* pt;
Gate::Gate(GameObj* po) : SpriteRen(po, pt?pt:pt=fileIO::loadImage("res/portal.png"))
{
	sourceRect = new SDL_Rect{0,0,8,8};
}

void Gate::onEvent(const bj::ecs::Event& e)
{
	if(e.type==ecs::Event::frame) sourceRect->x = ((int)(e.elapsed*5)%4)*8;
	SpriteRen::onEvent(e);
}
