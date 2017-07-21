#pragma once
#include "spritesheet.h"
#include <map>

struct SpriteSheetStore {

	static const SpriteSheet& getSpriteSheet(std::string name);

	private:
	SpriteSheetStore();

	static SpriteSheet loadSpriteSheet(std::string name);

	static std::map<std::string, SpriteSheet> spritesheets;
};