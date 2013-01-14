#pragma once

#include <string>
#include <map>

#include "cinder/gl/Texture.h"

namespace ly {

class AssetManager {
public:
	virtual ~AssetManager();
	static AssetManager* get();
	
	ci::gl::Texture& getTexture( std::string path );
	
private:
	AssetManager();
	static AssetManager* sInstance;
	
	std::map<std::string, ci::gl::Texture> mTextures;
};

}