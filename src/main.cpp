#include <bj/berryJam.h>
#include "gameplay.h"

using namespace bj;

int BJ_MULP_MAIN()
{
	initParams ip{};
	ip.win_title = "OrObOrOs";

	ip.scenes = new Scene*{new Gameplay{}};
	ip.nscenes = 1;

	Game::init(ip);
	shitrndr::WindowProps::setPixScale(3);
	Game::start();
}
