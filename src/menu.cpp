#include "menu.h"

using namespace bj;

static SpriteRen* logoRen;

static constexpr uint8_t nops = 2;
static const char* options[nops] = { "start", "quit" };
static uint8_t ch = 0;

void Menu::onStart()
{
	instantiate()->addComponent(new SpriteRen(getObj(0), "res/logo.png"));
	getObj(0)->transform.scl.x = 2;
	logoRen = getObj(0)->getComponent<SpriteRen>();
	logoRen->sourceRect = new SDL_Rect{0,0,128,64};
	cam.scale = 2;
	cam.pos = {1, 1};

	getObj(0)->addComponent(new Behaviour(getObj(0), [](Behaviour* self, const ecs::Event& e)
	{
		if(e.type==ecs::Event::keyD)
			switch(e.key)
			{
				case SDLK_w:
				case SDLK_UP:
					ch--;
					break;
				case SDLK_s:
				case SDLK_DOWN:
					ch++;
					break;
				case SDLK_SPACE:
				case SDLK_RETURN:
					switch(ch)
					{
						case 0: SceneManager::setActiveScene(1); break;
						case 1: std::exit(0); break;
					}
					break;
			}
		ch%=2;
	}));
};

void Menu::onLoad()
{
	shitrndr::bg_col = { 2, 1, 3, 255 };
	logoRen->sourceRect->x = 0;
};
void Menu::onUnload() {}

void Menu::onRenderBG(float d, float t)
{
	for(uint8_t i = 0; i != ch; i++) if(i!=ch) UI::renderText(.5, .5 + (float)i*-.1, UI::CENTRED, 0, options[i]);
	UI::renderText(.5, .5, UI::CENTRED, 0, (std::string("> ") + options[ch]).c_str(), {255,0,255,255});
	for(uint8_t i = ch+1; i != nops; i++) if(i!=nops)UI::renderText(.5, .5 + (float)i*.1, UI::CENTRED, 0, options[i]);
}
void Menu::onRenderFG(float d, float t)
{
	if(t<1.1) logoRen->sourceRect->x = 128 * ((int)(t*10)%10);
}
