#pragma once
#include "SpriteSheetStore.h"
#include "ImageStore.h"
#include <fstream>

const SpriteSheet& SpriteSheetStore::getSpriteSheet(std::string name){
	auto it = spritesheets.find(name);
	if (it == spritesheets.end()){
		return (spritesheets[name] = loadSpriteSheet(name));
	} else {
		return it->second;
	}
}

SpriteSheetStore::SpriteSheetStore(){

}

SpriteSheet SpriteSheetStore::loadSpriteSheet(std::string name){
	std::ifstream ifs;
	ifs.open("images/" + name + ".spritesheet");
	if (!ifs.is_open()){
		throw std::runtime_error("The spritesheet file could not be opened");
	}

	SpriteSheet spritesheet;

	spritesheet.setTexture(ImageStore::getTexture(name));

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

	ifs.close();

	return spritesheet;
}

std::map<std::string, SpriteSheet> SpriteSheetStore::spritesheets;