#pragma once

namespace ly {

typedef enum {
	MODE_SELECTION_OBJECT,
	MODE_PAINT_ELEVATION,
} EditorMode;
	
typedef enum {
	SELECTION_POINT,
	SELECTION_FACE
} SelectionMode;

typedef enum {
	PAINT_SPECIFIC,
	PAINT_STACK,
	PAINT_ADDITIVE,
} ElevationPaintMode;


class EditorState {
public:
	EditorState() : brushSize(3), targetElevation(1), showGrid(false) {}
	int										brushSize;
	int										targetElevation;
	bool									showGrid;
};

}