#pragma once
#include <bj/berryJam.h>

struct Gameplay : public bj::Scene
{
	void onStart() override;
	void onLoad() override;
	void onUnload() override;
	void onRenderBG(float d, float t) override;
	void onRenderFG(float d, float t) override;
};
