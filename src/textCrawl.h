#pragma once
#include <bj/berryJam.h>

struct TextCrawl : public bj::Scene
{
	std::vector<std::string> lines;
	uint8_t stl;
	TextCrawl(const std::string& text_, const uint8_t& sceneToLoad);

	float curP = 0;
	uint8_t line = 0;

	void onStart() override;
	void onLoad() override;
	void onUnload() override;
	void onRenderBG(float d, float t) override;
	void onRenderFG(float d, float t) override;
};
