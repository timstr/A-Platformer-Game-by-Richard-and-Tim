#pragma once

#include "ImageStore.h"
#include "imagespath.h"

const sf::Image& ImageStore::getImage(std::string filename){
	const std::string path = images_path + "/" + filename + ".png";
	auto it = image_map.find(filename);
	if (it == image_map.end()){
		if (!image_map[filename].loadFromFile(path)){
			throw std::runtime_error("The image could not be loaded");
		}
		return image_map[filename];
	} else {
		return it->second;
	}
}

const sf::Texture& ImageStore::getTexture(std::string filename, bool load_image_too){
	auto it = texture_map.find(filename);
	if (it == texture_map.end()){
		auto it2 = image_map.find(filename);
		if (it2 != image_map.end()){
			texture_map[filename].loadFromImage(it2->second);
		} else {
			if (load_image_too){
				texture_map[filename].loadFromImage(getImage(filename));
			} else {
				const std::string path = images_path + "/" + filename + ".png";
				if (!texture_map[filename].loadFromFile(path)){
					throw std::runtime_error("The texture could not be loaded");
				}
			}
		}
		texture_map[filename].setSmooth(true);
		return texture_map[filename];
	} else {
		return it->second;
	}
}

void ImageStore::discardImage(const std::string& filename){
	auto it = image_map.find(filename);
	if (it != image_map.end()){
		image_map.erase(it);
	}
}

void ImageStore::discardTexture(const std::string& filename){
	auto it = texture_map.find(filename);
	if (it != texture_map.end()){
		texture_map.erase(it);
	}
}

void ImageStore::clear(){
	image_map.clear();
	texture_map.clear();
}

ImageStore::ImageStore(){

}
ImageStore::~ImageStore(){
	clear();
}

std::map<std::string, sf::Image> ImageStore::image_map;
std::map<std::string, sf::Texture> ImageStore::texture_map;