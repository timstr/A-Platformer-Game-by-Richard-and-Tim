#pragma once

#include <map>
#include <SFML\Graphics.hpp>

// TODO: get rid of maps and use shared_ptrs instead?

struct ImageStore {
	// returns an Image object from the image file found at the given path
	// or throws an exception if it can't be loaded
	static const sf::Image& getImage(const std::string& filename);

	// returns a Texture object from the image file found at the given path,
	// if the file is already loaded as an image, it will be reused
	// throws an exception if it can't be loaded
	static const sf::Texture& getTexture(const std::string& filename, bool load_image_too = true);

	// frees the memory used by the image object
	// obvious potential for dangling references, so use wisely
	static void discardImage(const std::string& filename);

	// frees the memory used by the texture object
	// obvious potential for dangling references, so use wisely
	static void discardTexture(const std::string& filename);

	// frees all memory used by the image and texture objects
	// use only when safe
	static void clear();

	private:

	ImageStore();
	~ImageStore();

	static std::map<std::string, sf::Image> image_map;
	static std::map<std::string, sf::Texture> texture_map;
};