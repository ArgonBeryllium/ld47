#include "consumable.h"
#include "manager.h"
using namespace bj;

Consumable::Consumable(GameObj* po) : ecs::Com(BehSys::getInstance(), po)
{
	static SDL_Texture* stex;
	if(!stex) stex = fileIO::loadImage("res/specs.png");
	po->addComponent(new ParticleEm(po, 10));
	ParticleEm* pe = po->getComponent<ParticleEm>();
	pe->renderPart = [](ParticleEm* self, const uint32_t& i, const float& x, const float& y)
	{
		SDL_Rect dr = self->getPartRect(i), sr = {(int)(i+self->pLiv[i]*5)%4, 0, 8, 8};
		dr.w = (int)(self->pLiv[i]*(float)dr.w/2 + .5);
		dr.h = (int)(self->pLiv[i]*(float)dr.h/2 + .5);
		SDL_RenderCopy(shitrndr::ren, stex, &sr, &dr);
	};
	pe->getNewPos = [](ParticleEm* self, const uint32_t& i) { return self->parentObj->transform.pos + v2f{1,1}; };
	pe->getNewVel = [](ParticleEm* self, const uint32_t& i) { return v2f{(float)(std::rand()%100)/500, (float)(std::rand()%100)/500}; };
	for(uint32_t i = 0; i != 10; i++)
		pe->pLiv[i] = 0;
}

void Consumable::onEvent(const ecs::Event& e) {}
void Consumable::consume()
{
	parentObj->parentScene->destroy(parentObj->getID());
	Manager::consume();
}
