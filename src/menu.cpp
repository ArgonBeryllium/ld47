#include "menu.h"
#include "gameplay.h"

using namespace bj;

static SpriteRen* logoRen;

static constexpr uint8_t nops = 2;
static const char* options[nops] = { "start", "quit" };
static uint8_t ch = 0;
static float st;

static Mix_Chunk* selChunk, *chChunk;

void Menu::onStart()
{
	selChunk = fileIO::loadSound("res/select.wav");
	chChunk = fileIO::loadSound("res/choose.wav");
	instantiate()->addComponent(new SpriteRen(getObj(0), "res/logo.png"));
	getObj(0)->transform.scl.x = 2;
	logoRen = getObj(0)->getComponent<SpriteRen>();
	logoRen->sourceRect = new SDL_Rect{0,0,128,64};
	cam.scale = 9;
	cam.pos = {1, 1};

	getObj(0)->addComponent(new Behaviour(getObj(0), [](Behaviour* self, const ecs::Event& e)
	{
		if(e.type==ecs::Event::keyD)
			switch(e.key)
			{
				case SDLK_w:
				case SDLK_UP:
					ch--;
					Audio::playSound(selChunk, .3);
					break;
				case SDLK_s:
				case SDLK_DOWN:
					Audio::playSound(selChunk, .3);
					ch++;
					break;
				case SDLK_SPACE:
				case SDLK_RETURN:
					Audio::playSound(chChunk, .4);
					if(Gameplay::level!=7)
						switch(ch)
						{
							case 0: SceneManager::setActiveScene(2); break;
							case 1: std::exit(0); break;
						}
					else
						std::exit(0);
					break;
			}
		ch%=2;
	}));
};

void Menu::onLoad()
{
	shitrndr::bg_col = { 2, 1, 3, 255 };
	logoRen->sourceRect->x = 0;
	st = SDL_GetTicks()/1000;
};
void Menu::onUnload() {}

void Menu::onRenderBG(float d, float t)
{
	if(Gameplay::level!=7)
	{
		for(uint8_t i = 0; i != ch; i++) if(i!=ch) UI::renderText(.55, .5 - (float)(i)*.1 + std::sin(t+i)*.005, UI::RIGHT, 0, options[i]);
	UI::renderText(.55, .6 + std::sin(t*3)*.005, UI::RIGHT, 0, (std::string("> ") + options[ch]).c_str(), {255,0,255,255});
		for(uint8_t i = ch+1; i != nops; i++) if(i!=nops)UI::renderText(.55, .6 + (float)i*.1+ std::sin(t+i)*.005, UI::RIGHT, 0, options[i]);
		return;
	}
	else
	{
		UI::renderText(.5, .5, UI::CENTRED, 0, "OROBOROS - a game by ArBe");
		UI::renderText(.5, .55, UI::CENTRED, 0, "made within 48 hours");
		UI::renderText(.5, .6, UI::CENTRED, 0, "for Ludum Dare 47");
		UI::renderText(.5 + std::sin(t+.2)*.005, .7 + std::sin(t)*.005, UI::CENTRED, 0, "Thanks for playing!", {255,0,255,255});
		UI::renderText(.5, .8+ std::cos(t)*.005, UI::CENTRED, 0, "Press [SPACE] or [ENTER] to quit");
	}
}
void Menu::onRenderFG(float d, float t)
{
	if(t-st>.5&&t-st<1.5) logoRen->sourceRect->x = 128 * ((int)((t-st-.5)*10)%10);
	else logoRen->sourceRect->x = 0;
	logoRen->offset.pos = v2f{std::sin(t+.3f), std::sin(t-.1f)}*.01;
	logoRen->angle = std::cos(t*.5)*.03;
}
