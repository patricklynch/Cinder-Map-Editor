#pragma once

#include "cinder/DataSource.h"
#include "cinder/gl/Texture.h"
#include "cinder/ObjLoader.h"
#include "cinder/app/App.h"
#include "cinder/gl/Vbo.h"

#include <vector>
#include <map>
#include <string>

namespace ly {
		
typedef enum  {
	ShaderVertex,
	ShaderFragment,
	Texture,
	AssetTypeFont,
	AssetTypeModel,
	Audio,
	Geometry
} GameAssetType;
	
class GameAsset {
public:
	GameAssetType mType;
	std::string mPath;
	virtual bool load() { return false; }
	ci::DataSourceRef data() const { return mDataSourceRef; }
	ci::DataSourceRef mDataSourceRef;
};

class GameAssetTexture : public GameAsset {
public:
	virtual bool load();
	ci::gl::Texture* texture() const { return mTexture; }
private:
	ci::gl::Texture* mTexture;
};

class GameAssetModel : public GameAsset {
public:
	virtual bool load();
	ci::TriMesh* mesh() { return &mMesh; }
	ci::gl::VboMesh* vboMesh() { return &mVboMesh; }
private:
	ci::TriMesh mMesh;
	ci::gl::VboMesh mVboMesh;
};

class GameAssetFont : public GameAsset {
public:
	virtual bool load();
	ci::Font* font() const { return mFont; }
private:
	ci::Font* mFont;
};

class AssetManager {
public:
	static AssetManager* get();
	void loadAssets( std::vector<std::string>& assetPaths );
	void loadAssets( std::string* assets, int numElements );
	void loadAsset( std::string assetPath );
	
	/** To get at the asset object itself, use this method */
	template <typename T>
	T* getAsset( std::string key )
	{
		T* asset = NULL;
		if ( mLoadedAssets[ key ] ) {
			asset = dynamic_cast<T*>( mLoadedAssets[ key ] );
			if ( asset ) return asset;
		}
		return NULL;
	}
	
	/** To get right to the asset, use these convenience methods to bypass the wordy template method */
	ci::gl::Texture*	getTexture( const std::string path );
	ci::Font*			getFont( const std::string path );
	ci::TriMesh*		getMesh( const std::string path ); 
	ci::gl::VboMesh*	getVboMesh( const std::string path );
	
private:
	AssetManager();
	static AssetManager* sInstance;
	std::map<std::string, GameAsset*> mLoadedAssets;
};

}