#include "snake.h"
#include "consumable.h"
#include "gate.h"
#include "manager.h"
#include "gameplay.h"

static Mix_Chunk* c_move;
Snake::Snake(GameObj* po, const uint8_t& length_) : ecs::Com(BehSys::getInstance(), po), length(length_)
{
	if(!c_move)c_move = fileIO::loadSound("res/move.wav"); 
	tail = new GameObj*[length];
	for(uint8_t i = 0; i < length; i++)
		tail[i] = new GameObj();
}
static SDL_Texture* tex, *tex_alt;
void Snake::start()
{
	if(!tex) tex = fileIO::loadImage("res/snake.png");
	if(!tex_alt) tex_alt = fileIO::loadImage("res/snake-alt.png");
	for(uint8_t i = length; i != 0; i--)
	{
		parentObj->parentScene->instantiate(tail[i-1])->transform.pos = parentObj->transform.pos;
		tail[i-1]->addComponent(new SpriteRen(tail[i-1], tex));
		tail[i-1]->getComponent<SpriteRen>()->sourceRect = new SDL_Rect{16,0,8,8};
	}
	parentObj->addComponent(new SpriteRen(parentObj, tex));
	parentObj->getComponent<SpriteRen>()->sourceRect = new SDL_Rect{0, 0, 8,8};
}

void Snake::onEvent(const ecs::Event& e)
{
	switch(e.type)
	{
		default: break;
		case bj::ecs::Event::frame:
				for(uint8_t i = 0; i != length; i++)
				{
					 SpriteRen* sr = tail[i]->getComponent<SpriteRen>();
					sr->offset.scl = v2f{1.1,1.1} + v2f{(float)(sr->sourceRect->x!=16), (float)(sr->sourceRect->x==16||sr->sourceRect->x>56)} * std::sin((e.elapsed+i*6/(float)length)*4)*.05;
					 sr->onEvent(e);
				}
				break;
		case bj::ecs::Event::keyD:
			switch(e.key)
			{
				default: break;
				case SDLK_a: case SDLK_LEFT:  move(-1, 0); break;
				case SDLK_d: case SDLK_RIGHT: move( 1, 0); break;
				case SDLK_w: case SDLK_UP:    move( 0,-1); break;
				case SDLK_s: case SDLK_DOWN:  move( 0, 1); break;
				
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
	if(Manager::won) return 0;
	v2f dp = parentObj->transform.pos + v2f{(float)x, (float)y};

	for(auto& p : parentObj->parentScene->getObjs())
	{
		if((p.second->transform.pos - dp).getLength() < .5)
		{
			if(p.second->getComponent<Consumable>())
			{
				p.second->getComponent<Consumable>()->consume();
				return 1;
			}
			if(p.second->getComponent<Gate>()) return 1;
			if(p.second==tail[length-1])
			{
				Manager::win();
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
	Audio::playSound(c_move, .2);
	
	SpriteRen* sr = parentObj->getComponent<SpriteRen>();
	sr->flipX = x<0;
	sr->sourceRect->x = y?8:0;
	sr->flipY = y<0;

	updateSprites();
}

bool Snake::onGate()
{
	for(auto& p : parentObj->parentScene->getObjs())
		if((p.second->transform.pos - parentObj->transform.pos).getLength() < .5 && p.second->getComponent<Gate>())
			return 1;
	return 0;
}


void Snake::updateSprites()
{
	for(uint8_t i = 0; i < length; i++)
	{
		SpriteRen* sr = tail[i]->getComponent<SpriteRen>();
		sr->tex = tex;
		if(i>0)
		{
			sr->enabled = (tail[i-1]->transform.pos-tail[i]->transform.pos).getLengthSquare()>.25;
			if(!sr->enabled) continue;
		}
		v2f dp, dn;
		if(i>0) dp = tail[i-1]->transform.pos - tail[i]->transform.pos;
		else dp = parentObj->transform.pos - tail[i]->transform.pos;
		if(dp.x && dp.getLengthSquare()>2.1)
		{
			sr->tex = tex_alt;
			if(dp.y<0)dp.y+=Manager::lGap;
			else dp.y-=Manager::lGap;
		}
		if(i!=length-1)
		{
			dn = tail[i+1]->transform.pos-tail[i]->transform.pos;
			if(dn.getLengthSquare()>2.1)
			{
				sr->tex = tex_alt;
				if(dn.y<0)dn.y+=Manager::lGap;
				else dn.y-=Manager::lGap;
			}
		}
		if(i==length-1)
		{
			if(!Manager::won)
			{
				sr->sourceRect->x = dp.y==0?64:72;
				sr->flipX = dp.x<0;
				sr->flipY = dp.y<0;
				return;
			}
			dn = parentObj->transform.pos - tail[i]->transform.pos; 
		}
		if(dp.x==0 && dn.x==0) sr->sourceRect->x = 24;
		else if(dp.y==0 && dn.y==0) sr->sourceRect->x = 16;
		else
		{
			sr->sourceRect->x = ((dp.x<0&&dn.y>0)||(dn.x<0&&dp.y>0))?32:
								((dp.y>0&&dn.x>0)||(dn.y>0&&dp.x>0))?40:
								((dp.x<0&&dn.y<0)||(dn.x<0&&dp.y<0))?48:56;
		}
	}

}
