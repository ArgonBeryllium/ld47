#include <bj/berryJam.h>
#include "gameplay.h"
#include "menu.h"
#include "manager.h"
#include "textCrawl.h"

using namespace bj;

int BJ_MULP_MAIN()
{
	initParams ip{};
	ip.win_title = "OrObOrOs";
	ip.win_h = 960;

	ip.scenes = new Scene*[4]{new TextCrawl{fileIO::readFile("res/intro"), 1}, new Menu{}, new Gameplay{}, new TextCrawl{fileIO::readFile("res/ending"), 1}};
	ip.nscenes = 4;

	shitrndr::WindowProps::setPixScale(3);
	Game::init(ip);

	UI::loadFont("res/font.ttf", .07);
	UI::loadFont("res/font.ttf", .15);
	Manager::init();

	SDL_SetWindowIcon(shitrndr::win, IMG_Load("res/icon.png"));

	Game::start();
}
