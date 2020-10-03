#include "manager.h"
#include "gameplay.h"

Snake* Gameplay::snake;
void Manager::init()
{
}
static bool switched = 0;
void Manager::switchPlanes()
{
	SceneManager::getActiveScene()->cam.pos.y = switched?0:10;
	Gameplay::snake->parentObj->transform.pos.y += switched?-5:5;
	switched=!switched;
}
