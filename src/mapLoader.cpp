#include "mapLoader.h"

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

void MapLoader::loadFile(const char* path)
{
	if(tiles) delete[] tiles;
	objs.clear();

	cam = {};
	scale = {};
	origin = {};
	std::string data = bj::fileIO::readFile(path);

	std::vector<std::string> lines = split(data, '\n');

	uint16_t i = 0;
	for(auto& l : lines)
	{
		if(l == "mb") { i++; break; }
		
		std::vector<std::string> args = split(l.substr(1), ',');
		switch(l[0])
		{
		case 'C':
			cam = {};
			cam.pos = {std::stof(args[0]), std::stof(args[1])};
			cam.scale = std::stof(args[2]);
			break;
		case 'S': scale = {std::stof(args[0]), std::stof(args[1])}; break;
		}
		i++;
	}

	uint16_t b = i; 
	for (;lines[i] != "me";i++);
	w = lines[b].length();
	h = i-b;

	size=w*h;
	tiles = new char[size];
	for(uint16_t r = 0; r < h; r++)
		for(uint16_t c = 0; c < w;c++)
		{
			tiles[r*w+c] = lines[b+r][c];
			if(lines[b+r][c]=='o') origin = {scale.x*c, scale.y*r};
		}

	for(uint16_t t = 0; t < size; t++) parseTile(scale.x*(t%w)-origin.x, scale.y*(t/w)-origin.y, t);
}

void MapLoader::parseTile(const float& x, const float& y, uint16_t& i)
{
	size_t j = 0;
	while(specialSyms[j])
		if(tiles[i]==specialSyms[j]) return;
		else j++;

	parseTileSym(this, x, y, i, tiles[i]);
	if(tiles[i-1]=='[')
	{
		uint16_t ii = i;
		for(; tiles[i]!=']'; i++);
		objs[ii]->transform.pos.x -= scale.x; 
		objs[ii]->transform.scl.x = scale.x * (i-ii+2);
	}
}

void MapLoader::apply(bj::Scene* target)
{
	target->cam = cam;
	target->clearObjs();
	for(auto p : objs) target->instantiate(p.second);
}
