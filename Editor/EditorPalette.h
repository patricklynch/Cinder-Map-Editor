#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"

namespace ly {

class EditorPalette;
class Editor;

class EditorThumbnail {
public:
	EditorThumbnail( int index, ci::gl::Texture* texture  );
	
	ci::Vec2i				position;
	ci::Vec2i				size;
	void					draw();
	void					update( const float deltaTime );
	void					onClick( ci::app::MouseEvent event );
	EditorPalette*			palette;
	int						index;
	ci::Vec2i				globalPosition();
	bool					isSelected;
	
private:
	ci::gl::Texture* mTexture;
};
	
struct IEditorPaletteDelegate {
	virtual void didUpdateSelection( EditorPalette* palette, int index ) = 0;
};

class EditorPalette {
public:
	EditorPalette( std::vector<ci::gl::Texture*> textures, int* selectionIndexPtr );
	
	ci::Vec2i				position;
	ci::Vec2i				size;
	std::string				name;
	void					draw();
	void					update( const float deltaTime );
	int						innerMargin;
	void					thumbnailSelected( EditorThumbnail* thumbnail );
	int*					mSelectionIndex;
	int						mId;
	IEditorPaletteDelegate* mDelegate;
	
private:
	std::vector<EditorThumbnail*> mThumbnails;
};

}