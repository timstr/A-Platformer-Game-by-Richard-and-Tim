#pragma once

#include "spritesheet.h"
#include "SpriteSheetStore.h"
#include "ImageStore.h"
#include <map>

SpriteSheet::SpriteSheet(){

}

void SpriteSheet::setTexture(const sf::Texture& _texture){
	texture = &_texture;
}

void SpriteSheet::setFrameSize(vec2 _framesize){
	framesize = _framesize;
}

void SpriteSheet::setFrameCenter(vec2 _framecenter){
	framecenter = _framecenter;
}

void SpriteSheet::setFramesPerRow(int _frames_per_row){
	frames_per_row = _frames_per_row;
}

void SpriteSheet::addAnimation(std::string name, int start_frame, int end_frame){
	animations[name] = Animation(start_frame - 1, end_frame - 1);
}

SpriteSheetPlayer::SpriteSheetPlayer(const std::string& name) : spritesheet(SpriteSheetStore::getSpriteSheet(name)) {
	sprite.setTexture(*spritesheet.texture);
	cliprect.width = spritesheet.framesize.x;
	cliprect.height = spritesheet.framesize.y;
	current_frame = 0;
	current_animation = spritesheet.animations.end();
}

SpriteSheetPlayer::SpriteSheetPlayer(SpriteSheet& _spritesheet) : spritesheet(_spritesheet) {
	sprite.setTexture(*spritesheet.texture);
	cliprect.width = spritesheet.framesize.x;
	cliprect.height = spritesheet.framesize.y;
	current_frame = 0;
	current_animation = spritesheet.animations.end();
}

void SpriteSheetPlayer::play(std::string animation_name){
	current_animation = spritesheet.animations.find(animation_name);
	if (current_animation == spritesheet.animations.end()){
		throw std::runtime_error("The animation could not be found");
	} else {
		current_frame = current_animation->second.start_frame;
	}
}

void SpriteSheetPlayer::tick(){
	if (current_animation != spritesheet.animations.end()){
		current_frame += 1;
		if (current_frame > current_animation->second.end_frame){
			//TODO: callback system?
			current_frame = current_animation->second.start_frame;
		}
	}
}

void SpriteSheetPlayer::setScale(vec2 scale){
	sprite.setScale(scale);
}

void SpriteSheetPlayer::render(sf::RenderWindow& rw, vec2 offset){
	cliprect.left = spritesheet.framesize.x * (current_frame % spritesheet.frames_per_row);
	cliprect.top = spritesheet.framesize.y * (current_frame / spritesheet.frames_per_row);
	sprite.setTextureRect(cliprect);
	sprite.setPosition(offset - vec2(spritesheet.framecenter.x * sprite.getScale().x, spritesheet.framecenter.y * sprite.getScale().y));
	rw.draw(sprite);
}