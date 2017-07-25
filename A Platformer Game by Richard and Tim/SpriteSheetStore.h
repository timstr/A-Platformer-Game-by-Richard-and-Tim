#pragma once
#include "spritesheet.h"
#include <string>
#include <map>

struct SpriteSheetStore {

	static const SpriteSheet& getSpriteSheet(const std::string& name);

	private:
	SpriteSheetStore();

	static SpriteSheet loadSpriteSheet(const std::string& name);

	static std::map<std::string, SpriteSheet> spritesheets;
};