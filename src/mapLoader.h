#pragma once
#include <bj/berryJam.h>

struct MapLoader
{
	uint16_t w, h, size;
	char* tiles;
	const char* specialSyms = ".[]o";
	bj::v2f origin, scale;
	bj::Camera cam;
	std::map<uint16_t, bj::GameObj*> objs;

	void loadFile(const char* path);
	void parseTile(const float& x, const float& y, uint16_t& i);
	void (*parseTileSym)(MapLoader*, const float&, const float&, uint16_t&, const char&) =
		[](MapLoader* self, const float& x, const float& y, uint16_t& i, const char& id)
		{
			using namespace bj;
			self->objs[i] = new GameObj(); 
			self->objs[i]->transform.pos = {x, y};
			switch(id)
			{
			case '0':
				self->objs[i]->addComponent(new BasicRen(self->objs[i], {255, 255, 255, 255}));
				self->objs[i]->addComponent(new RBody(self->objs[i]));
				break;
			case '1':
				self->objs[i]->addComponent(new BasicRen(self->objs[i], {155, 55, 55, 55}));
				self->objs[i]->addComponent(new RBody(self->objs[i], 0));
				break;
			case '2':
				self->objs[i]->addComponent(new BasicRen(self->objs[i], {55, 155, 55, 55}));
				self->objs[i]->addComponent(new ParticleEm(self->objs[i], 100));
				break;
			}
		};

	void apply(bj::Scene* target);
};
