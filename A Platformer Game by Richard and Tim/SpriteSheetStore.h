#pragma once
#include "spritesheet.h"
#include <string>
#include <map>

// TODO: consider getting rid of map and using shared_ptr<SpriteSheet> instead

struct SpriteSheetStore {

	static const SpriteSheet& getSpriteSheet(const std::string& name);

	private:
	SpriteSheetStore();

	static SpriteSheet loadSpriteSheet(const std::string& name);

	static std::map<std::string, SpriteSheet> spritesheets;
};