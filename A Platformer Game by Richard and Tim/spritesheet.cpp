#pragma once

#include "spritesheet.h"
#include "SpriteSheetStore.h"
#include "ImageStore.h"
#include <map>

SpriteSheet::SpriteSheet(){
	texture = nullptr;
	framecenter = {0, 0};
	framesize = {0, 0};
	frames_per_row = 0;
	frames_per_second = 30;
	flip = false;
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

void SpriteSheet::setFramesPerSecond(int _frames_per_second){
	frames_per_second = _frames_per_second;
}

void SpriteSheet::addAnimation(const std::string& name, int start_frame, int end_frame){
	animations[name] = Animation(start_frame - 1, end_frame - 1);
}

void SpriteSheet::setFlip(bool _flip){
	flip = _flip;
}

Event AnimationEnd;

SpriteSheetPlayer::SpriteSheetPlayer(const std::string& name, const std::function<void()>& _onComplete)
	: spritesheet(SpriteSheetStore::getSpriteSheet(name)) {
	facingright = true;
	sprite.setTexture(*spritesheet.texture);
	if (spritesheet.flip){
		sprite.setScale(-1.0f, 1.0f);
	}
	cliprect.width = spritesheet.framesize.x;
	cliprect.height = spritesheet.framesize.y;
	current_frame = 0;
	current_animation = spritesheet.animations.end();
	timestamp = clock.getElapsedTime();
	frames_carryover = 0;
}

void SpriteSheetPlayer::setOnComplete(const std::function<void()>& _onComplete){
	onComplete = _onComplete;
}

void SpriteSheetPlayer::play(const std::string& animation_name){
	auto prev = current_animation;
	current_animation = spritesheet.animations.find(animation_name);
	if (prev == current_animation){
		return;
	}
	if (current_animation == spritesheet.animations.end()){
		throw std::runtime_error("The animation could not be found");
	} else {
		current_frame = current_animation->second.start_frame;
	}
	frames_carryover = 0;
	updateClipRect();
}

void SpriteSheetPlayer::tick(){
	sf::Time now = clock.getElapsedTime();

	if (current_animation != spritesheet.animations.end()){

		sf::Time elapsed = now - timestamp;
		float seconds = elapsed.asSeconds();
		frames_carryover += seconds * spritesheet.frames_per_second;

		for (; frames_carryover > 1; frames_carryover -= 1){
			current_frame += 1;
			if (current_frame > current_animation->second.end_frame){
				if (onComplete){
					onComplete();
				}
				current_frame = current_animation->second.start_frame;
			}
		}
	}
	updateClipRect();

	timestamp = now;
}

void SpriteSheetPlayer::faceLeft(){
	if (facingright){
		scale(-1.0f, 1.0f);
		facingright = false;
	}
}
void SpriteSheetPlayer::faceRight(){
	if (!facingright){
		scale(-1.0f, 1.0f);
		facingright = true;
	}
}
void SpriteSheetPlayer::flip(){
	facingright = !facingright;
	scale(-1.0f, 1.0f);
}

void SpriteSheetPlayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	states.transform *= getTransform();
	rt.draw(sprite, states);
}

void SpriteSheetPlayer::updateClipRect(){
	cliprect.left = spritesheet.framesize.x * (current_frame % spritesheet.frames_per_row);
	cliprect.top = spritesheet.framesize.y * (current_frame / spritesheet.frames_per_row);
	sprite.setTextureRect(cliprect);
	sprite.setPosition(-vec2(spritesheet.framecenter.x * sprite.getScale().x, spritesheet.framecenter.y * sprite.getScale().y));
}

sf::Clock SpriteSheetPlayer::clock;