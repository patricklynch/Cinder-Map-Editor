#include "AssetManager.h"

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

AssetManager* AssetManager::sInstance = NULL;

AssetManager* AssetManager::get()
{
	if ( !sInstance ) sInstance = new AssetManager();
	return sInstance;
}

ci::gl::Texture& AssetManager::getTexture( std::string path )
{
	if ( !mTextures[ path ] ) {
		gl::Texture::Format format;
		//format.enableMipmapping();
		//format.setMagFilter( GL_NEAREST );
		mTextures[ path ] = gl::Texture( loadImage( app::loadResource( path ) ), format );
	}
	return mTextures[ path ];
}

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}