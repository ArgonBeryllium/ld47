#include <bj/berryJam.h>
#include "gameplay.h"
#include "menu.h"
#include "manager.h"

using namespace bj;

int BJ_MULP_MAIN()
{
	initParams ip{};
	ip.win_title = "OrObOrOs";
	ip.win_h = 960;

	ip.scenes = new Scene*[2]{new Menu{}, new Gameplay{}};
	ip.nscenes = 2;

	Game::init(ip);

	shitrndr::WindowProps::setPixScale(4);
	UI::loadFont("res/font.ttf", .07);
	UI::loadFont("res/font.ttf", .15);
	Manager::init();

	Game::start();
}
