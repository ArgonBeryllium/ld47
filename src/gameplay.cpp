#include "gameplay.h"
using namespace bj;
#include "mapLoader.h"
#include "gate.h"

static MapLoader ml;
Snake* Gameplay::snake;

void Gameplay::onStart()
{
	ml.parseTileSym = [](MapLoader* self, const float& x, const float& y, uint16_t& i, const char& id)
	{
		using namespace bj;
		self->objs[i] = new GameObj(); 
		self->objs[i]->transform.pos = {x, y};
		switch(id)
		{
		case 'p':
			{
				std::string l{};
				uint16_t ii = i;
				while(self->tiles[++i]!=';') l+=self->tiles[i];
				self->objs[ii]->addComponent(new Snake(self->objs[ii], std::stoi(l)));
				snake = self->objs[ii]->getComponent<Snake>();
				break;
			}
		case 'G':
			self->objs[i]->addComponent(new Gate(self->objs[i]));
			break;
		case '#':
			self->objs[i]->addComponent(new BasicRen(self->objs[i], {155, 55, 55, 55}));
			break;
		}
	};
}

void Gameplay::onLoad()
{
	ml.loadFile("res/tmap.map");
	ml.apply(this);
	snake->start();
};
void Gameplay::onUnload() {};

void Gameplay::onRenderBG(float d, float t) {}
void Gameplay::onRenderFG(float d, float t) {}
