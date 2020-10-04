#include "gameplay.h"
using namespace bj;
#include "gate.h"
#include "manager.h"

uint8_t Gameplay::level = 0;

static ParticleEm* parts1, *parts2;

static float sh_t, sh_a;
static v2f cp;

void Gameplay::shakeCam(float t, float a)
{
	if(sh_t<=0) cp = SceneManager::getActiveScene()->cam.pos;
	sh_t = t;
	sh_a = a;
}
static Mix_Chunk* c_reset;
void Gameplay::onStart()
{
	c_reset = fileIO::loadSound("res/reset.wav");
	SceneManager::getGlosc()->instantiate()->addComponent(new Behaviour(SceneManager::getGlosc()->getObj(0), [](Behaviour* self, const ecs::Event& e)
	{
		if(SceneManager::getActiveScene()!=SceneManager::getScene(2)) return;
		if(e.type==ecs::Event::keyD)
		{
			if(e.key==SDLK_r) { Audio::playSound(c_reset, .5); Manager::loadLevel(level); shakeCam(.15, .05); }
			else if(e.key==SDLK_RETURN && Manager::won)
			{
				Audio::playSound(c_reset, .5);
				level++;
				if(level==7)SceneManager::setActiveScene(3);
				else
				{
					Manager::loadLevel(level);
					shakeCam(.1, .4);
				}
			}
		}
	}));
	SceneManager::getGlosc()->instantiate()->addComponent(new ParticleEm(SceneManager::getGlosc()->getObj(1), 80));
	parts1 = SceneManager::getGlosc()->getObj(1)->getComponent<ParticleEm>();
	parts1->getNewPos = [](ParticleEm* self, const uint32_t& i) { return v2f{(float)(std::rand()%20-10), (float)(std::rand()%20-10)}; };
	parts1->renderPart = [](ParticleEm* self, const uint32_t& i, const float& x, const float& y)
	{
		SDL_SetRenderDrawColor(shitrndr::ren, 120, 60, 120, 255);
		int s = self->pLiv[i]>1?4.0/self->pLiv[i]:self->pLiv[i]*4;
		SDL_Rect r = self->getPartRect(i);
		r.w = r.h = s;
		shitrndr::FillRect(r);
	};
	SceneManager::getGlosc()->instantiate()->addComponent(new ParticleEm(SceneManager::getGlosc()->getObj(2), 30));
	parts2 = SceneManager::getGlosc()->getObj(2)->getComponent<ParticleEm>();
	parts2->getNewPos = [](ParticleEm* self, const uint32_t& i) { return v2f{(float)(std::rand()%20-10), (float)(std::rand()%20-8)+Manager::lGap}; };
	parts2->renderPart = [](ParticleEm* self, const uint32_t& i, const float& x, const float& y)
	{
		SDL_SetRenderDrawColor(shitrndr::ren, 255, 100, 255, 255);
		int s = self->pLiv[i]>1?6.0/self->pLiv[i]:self->pLiv[i]*6;
		SDL_Rect r = self->getPartRect(i);
		r.w = r.h = s;
		shitrndr::FillRect(r);
	};

	for(uint32_t i = 0; i<parts1->count; i++) parts1->pLiv[i] = 0;
	for(uint32_t i = 0; i<parts2->count; i++) parts2->pLiv[i] = 0;
}

void Gameplay::onLoad()
{
	Manager::loadLevel(level);
};
void Gameplay::onUnload() {};

void Gameplay::onRenderBG(float d, float t)
{
	shitrndr::bg_col = {(Uint8)((std::sin(t)+1)*(Manager::switched?20:3)),0,(Uint8)((std::sin(t+.2)+1)*30),255};

	if(sh_t>0)
	{
		sh_t-=d;
		cam.pos = cp + v2f(std::sin(std::rand()), std::sin(std::rand()))*sh_a;
		if(sh_t<=0) cam.pos = cp;
	}
}
void Gameplay::onRenderFG(float d, float t)
{
	if(Manager::cicl) UI::renderText(.05, .05, UI::LEFT, 0, ("+"+std::to_string(Manager::cicl)).c_str());
	if(level == 1 && Manager::snake->onGate()) UI::renderText(.5, .8, UI::CENTRED, 0, "[SPACE]");
	for(auto& p : getObjs())
	{
		SDL_Rect r = p.second->transform.getScreenRect();
		UI::renderText(r.x, r.y, UI::CENTRED, 0, std::to_string(p.first).c_str());
	}
	if(Manager::won)
	{
		UI::renderText(.5, .51, UI::CENTRED, 1, "CYCLE CLOSED", {0,0,0,255});
		UI::renderText(.5, .5, UI::CENTRED, 1, "CYCLE CLOSED", {255,(Uint8)((std::sin(t*2)+1)*127),255,255});
		UI::renderText(.5, .71, UI::CENTRED, 0, "[ENTER] to continue", {0,0,0,255});
		UI::renderText(.5, .7, UI::CENTRED, 0, "[ENTER] to continue");
	}
}
