#include "manager.h"
#include "gameplay.h"
#include "snake.h"
#include "gate.h"
#include "consumable.h"

MapLoader* Manager::ml;
v2f Manager::wPos, Manager::wScl;
Snake* Manager::snake;
float Manager::lGap = 5;
static SDL_Texture* wtex; 

void Manager::init()
{
	ml = new MapLoader();
	ml->parseTileSym = [](MapLoader* self, const float& x, const float& y, uint16_t& i, const char& id)
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
				Manager::snake = self->objs[ii]->getComponent<Snake>();
				break;
			}
		case 'c':
			self->objs[i]->addComponent(new Consumable(self->objs[i]));
			break;
		case 'G':
			self->objs[i]->addComponent(new Gate(self->objs[i]));
			break;
		case 'B': lGap = y+self->origin.y+1; break;
		case '#':
			if(!wtex) wtex = fileIO::loadImage("res/wall.png");
			self->objs[i]->addComponent(new SpriteRen(self->objs[i], wtex));
			bool ln = i%self->w!=0&&self->tiles[i-1]&&self->tiles[i-1]=='#',
				 rn = i%self->w<self->w-1&&self->tiles[i+1]&&self->tiles[i+1]=='#',
				 tn = i>self->w && self->tiles[i-self->w]=='#';

			SpriteRen* sr = self->objs[i]->getComponent<SpriteRen>();
			sr->sourceRect = new SDL_Rect{ln?rn?0:8:rn?16:24, 0, 8, 8};
			if(tn) sr->sourceRect->x += 32;
			//sr->offset.scl.y = 1.75;
			break;
		}
	};
}

uint8_t Manager::cicl = 0;
uint8_t Manager::score = 0;
bool Manager::won;
bool Manager::switched = 0;
void Manager::loadLevel(uint8_t i)
{
	ml->loadFile(("res/map_"+std::to_string(i)+".map").c_str());
	ml->apply(SceneManager::getActiveScene());
	snake->start();
	cicl = 0;
	switched = 0;
	won = 0;
	wPos = SceneManager::getActiveScene()->cam.getWorldPoint({0,0});
	wScl = v2f{(float)shitrndr::WindowProps::getWidth(), (float)shitrndr::WindowProps::getHeight()} * SceneManager::getActiveScene()->cam.getScreenspaceScalar();
}

Mix_Chunk* c_gate;
void Manager::switchPlanes()
{
	SceneManager::getActiveScene()->cam.pos.y += (switched?-1:1) * lGap;
	snake->parentObj->transform.pos.y += (switched?-1:1) * lGap;
	switched=!switched;
	Gameplay::shakeCam(.2, .1);
	if(!c_gate) c_gate = fileIO::loadSound("res/gate.wav");
	Audio::playSound(c_gate, .4);
	if(snake->parentObj->transform.pos == snake->tail[snake->length-1]->transform.pos) win();
}

void Manager::consume()
{
	cicl++;
}

void Manager::win()
{
	won = 1;
	score += cicl;
	Gameplay::shakeCam(.3, .2);
}
