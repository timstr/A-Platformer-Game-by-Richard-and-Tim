#pragma once

#include "renderable.h"
#include <map>

struct SpriteSheet {
	SpriteSheet();
	SpriteSheet(const sf::Texture& texture, vec2 _framecenter, vec2 _framesize, int _frames_per_row);

	private:

	void addAnimation(std::string name, int start_frame, int end_frame);

	sf::Sprite sprite;

	vec2 framecenter;
	vec2 framesize;
	int frames_per_row;

	sf::IntRect cliprect;

	void render(sf::RenderWindow& rw, vec2 position, int frame);

	struct Animation {
		Animation(int _start_frame = 0, int _end_frame = 0){
			start_frame = _start_frame;
			end_frame = _end_frame;
		}
		int start_frame;
		int end_frame;
	};

	std::map<std::string, Animation> animations;

	friend struct SpriteSheetPlayer;
	friend struct SpriteSheetStore;
};

struct SpriteSheetPlayer : Renderable {
	SpriteSheetPlayer(SpriteSheet& _spritesheet);

	void goTo(std::string animation_name);

	void tick();

	void render(sf::RenderWindow& rw, vec2 offset) override;

	private:
	SpriteSheet& spritesheet;
	int current_frame;
	std::map<std::string, SpriteSheet::Animation>::const_iterator current_animation;
};