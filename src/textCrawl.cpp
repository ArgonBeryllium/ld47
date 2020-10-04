#include "textCrawl.h"

static std::vector<std::string> split(const std::string& s, const char& c)
{
	std::vector<std::string> out;
	std::string cur;
	for(char e : s)
	{
		if(e==c && !cur.empty())
		{
			out.push_back(cur);
			cur = {};
			continue;
		}
		cur += e;
	}
	if(!cur.empty()) out.push_back(cur);
	return out;
}

TextCrawl::TextCrawl(const std::string& text, const uint8_t& sceneToLoad) : lines(split(text, '#')), stl(sceneToLoad) {}

static Mix_Chunk* c_char;
void TextCrawl::onStart()
{
	cam.scale = 2;
	if(!c_char)c_char = bj::fileIO::loadSound("res/move.wav");
}

void TextCrawl::onLoad() { shitrndr::bg_col = {0,0,0,255}; }
void TextCrawl::onUnload() {}

void TextCrawl::onRenderBG(float d, float t)
{
	using namespace bj;
	if(curP<lines[line].length())
	{
		curP += d*15+5*Input::getKey(SDLK_SPACE);
		if((int)(curP)>(int)(curP-d*15+5*Input::getKey(SDLK_SPACE)) && lines[line][curP] && lines[line][curP] != ' ' && lines[line][curP] != '\n' && lines[line][curP] != ',' && lines[line][curP] != '.')
			Audio::playSound(c_char, .2);
	}
	std::string l;
	for(uint8_t i = 0; i < curP && i < lines[line].length(); i++)
		l += lines[line][i];

	float yo = 0;
	for(std::string subline : split(l, '\n'))
	{
		UI::renderText(.5, .5+yo, UI::CENTRED, 0, subline.c_str());
		yo += .07;
	}

	if(curP>=lines[line].length())
	{
		if(!Input::getKey(SDLK_RETURN) && !Input::getKey(SDLK_SPACE)) return;
		curP = 0;
		line++;
		if(line==lines.size())SceneManager::setActiveScene(stl);
	}
}
void TextCrawl::onRenderFG(float d, float t) {}
