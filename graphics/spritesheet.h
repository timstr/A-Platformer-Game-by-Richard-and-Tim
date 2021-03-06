#pragma once

#include "SFML/Graphics.hpp"
#include "entity.h"
#include <functional>
#include <map>

struct SpriteSheet {
	SpriteSheet();

	void setTexture(const sf::Texture& _texture);

	void setFrameSize(vec2 _framesize);

	void setFrameCenter(vec2 _framecenter);

	void setFramesPerRow(int _frames_per_row);

	void setFramesPerSecond(int _frames_per_second);

	void addAnimation(std::string name, int start_frame, int end_frame);

	void setFlip(bool _flip);

private:

	const sf::Texture* texture;
	vec2 framecenter;
	vec2 framesize;
	int frames_per_row;
	int frames_per_second;
	bool flip;

	struct Animation {
		Animation(int _start_frame = 0, int _end_frame = 0)
			: start_frame(_start_frame), end_frame(_end_frame) {
		}
		int start_frame;
		int end_frame;
	};

	std::map<std::string, Animation> animations;

	friend struct SpriteSheetPlayer;
};

extern Event AnimationEnd;

struct SpriteSheetPlayer : sf::Drawable, sf::Transformable {
	SpriteSheetPlayer(std::string name, std::function<void()> _onComplete = {});

	void setOnComplete(std::function<void()> _onComplete);

	void play(const std::string& animation_name);

	std::string currentAnimation() const;

	void tick();

	void faceLeft();
	void faceRight();
	void flip();

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

private:

	void updateClipRect();

	bool facingright;
	float frames_carryover;
	sf::Time timestamp;
	const SpriteSheet& spritesheet;
	sf::Sprite sprite;
	sf::IntRect cliprect;
	int current_frame;
	std::map<std::string, SpriteSheet::Animation>::const_iterator current_animation;
	std::function<void()> onComplete;

	static sf::Clock clock;
};