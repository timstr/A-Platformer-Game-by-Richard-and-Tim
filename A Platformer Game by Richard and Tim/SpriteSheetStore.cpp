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
			int x, y;
			ifs >> x >> y;
			spritesheet.setFrameSize(vec2(x, y));
		} else if (tag == "framecenter"){
			int x, y;
			ifs >> x >> y;
			spritesheet.setFrameCenter(vec2(x, y));
		} else if (tag == "framesperrow"){
			int fpr;
			ifs >> fpr;
			spritesheet.setFramesPerRow(fpr);
		} else if (tag == "animation"){
			std::string anim_name;
			int startframe, endframe;
			ifs >> anim_name >> startframe >> endframe;
			spritesheet.addAnimation(anim_name, startframe, endframe);
		} else if (tag == "framespersecond"){
			int fps;
			ifs >> fps;
			spritesheet.setFramesPerSecond(fps);
		} else {
			ifs.close();
			throw std::runtime_error("Unrecognized tag while parsing spritesheet file");
		}
	}

	ifs.close();

	return spritesheet;
}

std::map<std::string, SpriteSheet> SpriteSheetStore::spritesheets;