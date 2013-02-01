#pragma once

#include "cinder/params/Params.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"

#include <vector>

namespace ly {
	
class EditorPalette;
class Editor;
	
class EditorThumbnail {
public:
	EditorThumbnail() : palette( NULL ) {}
	
	ci::Vec2i				position;
	ci::Vec2i				size;
	ci::gl::Texture*		mTexture;
	void					draw();
	EditorPalette*			palette;
};
	
class EditorPalette {
public:
	EditorPalette();
	
	ci::Vec2i				position;
	ci::Vec2i				size;
	std::string				name;
	void					draw();
	int						innerMargin;
private:
	std::vector<EditorThumbnail*> mThumbnails;
};
	
class EditorPanel {
public:
	EditorPanel( Editor* editor );
	virtual					~EditorPanel();
	
	void					draw();
	void					update( const float deltaTime );
	
	void					test();
	
private:
	Editor*					mEditor;
	ci::params::InterfaceGl mParams;
	EditorPalette			mTexturePicker;
	EditorPalette			mObjectPicker;
};

}