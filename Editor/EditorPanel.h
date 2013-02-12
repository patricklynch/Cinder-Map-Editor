#pragma once

#include "EditorPalette.h"
#include "Terrain.h"

#include "cinder/params/Params.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/app/App.h"

#include <vector>

namespace ly {
	
class Editor;
	
enum {
	TEXTURE_PICKER,
	BRUSH_PICKER,
	MODEL_PICKER
};

class EditorPanel : public IEditorPaletteDelegate {
public:
	EditorPanel( Editor* editor );
	virtual					~EditorPanel();
	
	void					draw();
	void					update( const float deltaTime );
	
	void					test();
	
	/** IEditorPaletteDelegate methods */
	void					didUpdateSelection( EditorPalette* palette, int index );
	
private:
	Editor*					mEditor;
	ci::params::InterfaceGl mParams;
	
	std::map<int, EditorPalette*> mPalettes;
	
	std::vector<ci::gl::Texture*> mBlockThumbs;
};

}