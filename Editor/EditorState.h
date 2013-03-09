#pragma once

#include "EditorTexturePaint.h"

namespace ly {

typedef enum {
	MODE_NONE = -1,
	MODE_SELECTION_OBJECT,
	MODE_PAINT_ELEVATION,
	MODE_PAINT_TEXTURE,
	MODE_COUNT
} EditorMode_t;
	
typedef enum {
	SELECTION_NONE = 1,
	SELECTION_POINT,
	SELECTION_FACE,
	SELECTION_COUNT
} SelectionMode_t;

typedef enum {
	PAINT_NONE,
	PAINT_SPECIFIC,
	PAINT_STACK,
	PAINT_ADDITIVE,
	PAINT_COUNT
} ElevationPaintMode_t;
	
class EditorState {
public:
	EditorState();
	
	//EditorValue<float> testFloat;
	//EditorValue<EditorMode_t> testType;
	//EditorValue<int> testInt;
	
	void					resetDefaults();
	void					modifySelection( int amount );
	void					modifyValue( float amount );
	
	int						elevationRange;
	int						elevationHeight;
	int						elevationSelection;
	TexturePaintState		texturePaint;
	bool					showGrid;
	EditorMode_t			mode;
};

}