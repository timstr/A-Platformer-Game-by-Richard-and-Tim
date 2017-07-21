#pragma once

#include "spritesheet.h"
#include "ImageStore.h"
#include <map>

SpriteSheet::SpriteSheet(){

}

SpriteSheet::SpriteSheet(const sf::Texture& texture, vec2 _framecenter, vec2 _framesize, int _frames_per_row){
	framecenter = _framecenter;
	framesize = _framesize;
	frames_per_row = _frames_per_row;
	sprite.setTexture(texture);
	cliprect.left = 0;
	cliprect.top = 0;
	cliprect.width = framesize.x;
	cliprect.height = framesize.y;
}

void SpriteSheet::addAnimation(std::string name, int start_frame, int end_frame){
	animations[name] = Animation(start_frame - 1, end_frame - 1);
}

void SpriteSheet::render(sf::RenderWindow& rw, vec2 position, int frame){
	cliprect.left = framesize.x * (frame % frames_per_row);
	cliprect.top = framesize.y * (frame / frames_per_row);
	sprite.setTextureRect(cliprect);
	sprite.setPosition(position - framecenter);
	rw.draw(sprite);
}

SpriteSheetPlayer::SpriteSheetPlayer(SpriteSheet& _spritesheet) : spritesheet(_spritesheet) {

}

void SpriteSheetPlayer::goTo(std::string animation_name){
	current_animation = spritesheet.animations.find(animation_name);
	if (current_animation == spritesheet.animations.end()){
		throw std::runtime_error("The animation could not be found");
	} else {
		current_frame = current_animation->second.start_frame;
	}
}

void SpriteSheetPlayer::tick(){
	current_frame += 1;
	if (current_frame > current_animation->second.end_frame){
		//TODO: callback system?
		current_frame = current_animation->second.start_frame;
	}
}

void SpriteSheetPlayer::render(sf::RenderWindow& rw, vec2 offset){
	spritesheet.render(rw, offset, current_frame);
}