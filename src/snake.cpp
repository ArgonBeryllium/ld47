#include "snake.h"
#include "gate.h"
#include "manager.h"

Snake::Snake(GameObj* po, const uint8_t& length_) : ecs::Com(BehSys::getInstance(), po), length(length_)
{
	tail = new GameObj*[length];
	for(uint8_t i = 0; i < length; i++)
		tail[i] = new GameObj();
}
void Snake::start()
{
	for(uint8_t i = length; i != 0; i--)
		parentObj->parentScene->instantiate(tail[i-1])->transform.pos = parentObj->transform.pos;
}

void Snake::onEvent(const ecs::Event& e)
{
	switch(e.type)
	{
		default: break;
		case bj::ecs::Event::frame:
			SDL_SetRenderDrawColor(shitrndr::ren, 150, 200, 180, 255);
			for(uint8_t i = length; i != 0; i--)
				shitrndr::FillRect(tail[i-1]->transform.getScreenRect());
			SDL_SetRenderDrawColor(shitrndr::ren, 150, 255, 230, 255);
			shitrndr::FillRect(parentObj->transform.getScreenRect());
			break;
		case bj::ecs::Event::keyD:
			switch(e.key)
			{
				default: break;
				case SDLK_a: move(-1, 0); break;
				case SDLK_d: move( 1, 0); break;
				case SDLK_w: move( 0,-1); break;
				case SDLK_s: move( 0, 1); break;
				
				case SDLK_SPACE:
						if(onGate())
							Manager::switchPlanes();
						break;
			}
			break;
	}
}

bool Snake::canMove(const int& x, const int& y)
{
	v2f dp = parentObj->transform.pos + v2f{(float)x, (float)y};

	for(auto& p : parentObj->parentScene->getObjs())
	{
		if((p.second->transform.pos - dp).getLength() < .5)
		{
			/*
			if(p.second->getComponent<Consumable>())
			{
				p.second->getComponent<Consumable>()->consume();
				return 1;
			}*/
			if(p.second->getComponent<Gate>()) return 1;
			if(p.second==tail[length-1])
			{
				//Manager::won = 1;
				return 1;
			}
			return 0;
		}
	}
	return 1;
}

void Snake::move(const int& x, const int& y)
{
	if(!canMove(x, y)) return;
	for(uint8_t i = length-1; i != 0;  i--)
		tail[i]->transform.pos = tail[i-1]->transform.pos;
	tail[0]->transform.pos = parentObj->transform.pos;
	parentObj->transform.pos += v2f((float)x, (float)y);
}

bool Snake::onGate()
{
	for(auto& p : parentObj->parentScene->getObjs())
		if((p.second->transform.pos - parentObj->transform.pos).getLength() < .5 && p.second->getComponent<Gate>())
			return 1;
	return 0;
}
