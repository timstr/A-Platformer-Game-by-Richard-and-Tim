#pragma once
#include "spritesheet.h"
#include "ImageStore.h"
#include <map>
#include <fstream>
#include <vector>

struct SpriteSheetStore {

	static const SpriteSheet& getSpriteSheet(std::string name){
		auto it = spritesheets.find(name);
		if (it == spritesheets.end()){
			return (spritesheets[name] = loadSpriteSheet(name));
		} else {
			return it->second;
		}
	}

	private:
	SpriteSheetStore(){

	}

	static SpriteSheet loadSpriteSheet(std::string name){
		std::ifstream ifs;
		ifs.open("images/" + name + ".spritesheet");

		SpriteSheet spritesheet;

		while (ifs.good()){
			std::string tag;
			ifs >> tag;
			if (tag == "framesize"){
				ifs >> spritesheet.framesize.x >> spritesheet.framesize.y;
			} else if (tag == "framecenter"){
				ifs >> spritesheet.framecenter.x >> spritesheet.framecenter.y;
			} else if (tag == "framesperrow"){
				ifs >> spritesheet.frames_per_row;
			} else if (tag == "animation"){
				std::string anim_name;
				int startframe, endframe;
				ifs >> anim_name >> startframe >> endframe;
				spritesheet.addAnimation(anim_name, startframe, endframe);
			} else {
				ifs.close();
				throw std::runtime_error("Unrecognized tag while parsing spritesheet file");
			}
		}

		spritesheet.sprite.setTexture(ImageStore::getTexture(name));
		spritesheet.cliprect.width = spritesheet.framesize.x;
		spritesheet.cliprect.height = spritesheet.framesize.y;

		ifs.close();

		return spritesheet;
	}

	static std::map<std::string, SpriteSheet> spritesheets;
};
std::map<std::string, SpriteSheet> SpriteSheetStore::spritesheets;