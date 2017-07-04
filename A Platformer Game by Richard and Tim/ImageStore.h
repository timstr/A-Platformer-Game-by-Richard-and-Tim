#pragma once

#include <map>
#include <SFML\Graphics.hpp>

struct ImageStoreType {

	// returns an Image object from the image file found at the given path
	// or throws an exception if it can't be loaded
	sf::Image& getImage(std::string filename){
		auto it = image_map.find(filename);
		if (it == image_map.end()){
			if (!image_map[filename].loadFromFile(filename)){
				throw std::runtime_error("The image could not be loaded");
			}
			return image_map[filename];
		} else {
			return it->second;
		}
	}

	// returns a Texture object from the image file found at the given path,
	// if the file is already loaded as an image, it will be reused
	// throws an exception if it can't be loaded
	sf::Texture& getTexture(std::string filename){
		auto it = texture_map.find(filename);
		if (it == texture_map.end()){
			auto it2 = image_map.find(filename);
			if (it2 != image_map.end()){
				if (!texture_map[filename].loadFromImage(it2->second)){
					throw std::runtime_error("The texture could not be loaded");
				}
			} else {
				texture_map[filename].loadFromFile(filename);
			}
			return texture_map[filename];
		} else {
			return it->second;
		}
	}

	// frees the memory used by the image object
	// obvious potential for dangling references, so use wisely
	void discardImage(std::string filename){
		auto it = image_map.find(filename);
		if (it != image_map.end()){
			image_map.erase(it);
		}
	}

	// frees the memory used by the texture object
	// obvious potential for dangling references, so use wisely
	void discardTexture(std::string filename){
		auto it = texture_map.find(filename);
		if (it != texture_map.end()){
			texture_map.erase(it);
		}
	}

	// frees all memory used by the image and texture objects
	// use only when safe
	void clear(){
		image_map.clear();
		texture_map.clear();
	}

	private:
	ImageStoreType(){

	}
	~ImageStoreType(){
		clear();
	}

	std::map<std::string, sf::Image> image_map;
	std::map<std::string, sf::Texture> texture_map;
} ImageStore;