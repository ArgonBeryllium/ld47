#include "gameplay.h"
using namespace bj;
#include "gate.h"
#include "manager.h"

uint8_t Gameplay::level = 0;
void Gameplay::onStart()
{
	SceneManager::getGlosc()->instantiate()->addComponent(new Behaviour(SceneManager::getGlosc()->getObj(0), [](Behaviour* self, const ecs::Event& e)
	{
		if(SceneManager::getActiveScene()!=SceneManager::getScene(1)) return;
		if(e.type==ecs::Event::keyD)
		{
			if(e.key==SDLK_r)Manager::loadLevel(level);
			else if(e.key==SDLK_RETURN && Manager::won)
			{
				level++;
				Manager::loadLevel(level);
			}
		}
		else if(e.type==ecs::Event::frame) SceneManager::getActiveScene()->cam.scale += e.delta*(Input::getKey(SDLK_i)-Input::getKey(SDLK_o));
	}));
}

void Gameplay::onLoad()
{
	Manager::loadLevel(level);
};
void Gameplay::onUnload() {};

void Gameplay::onRenderBG(float d, float t)
{
	shitrndr::bg_col = {(Uint8)((std::sin(t)+1)*20),0,(Uint8)((std::sin(t+.2)+1)*30),255};
}
void Gameplay::onRenderFG(float d, float t)
{
	if(Manager::cicl) UI::renderText(.05, .05, UI::LEFT, 0, ("+"+std::to_string(Manager::cicl)).c_str());
	for(auto& p : getObjs())
	{
		SDL_Rect r = p.second->transform.getScreenRect();
		UI::renderText(r.x, r.y, UI::CENTRED, 0, std::to_string(p.first).c_str());
	}
	if(Manager::won)
	{
		UI::renderText(.5, .5, UI::CENTRED, 1, "CYCLE CLOSED", {255,(Uint8)((std::sin(t*2)+1)*127),255,255});
		UI::renderText(.5, .7, UI::CENTRED, 0, "[ENTER] to continue");
	}
}
