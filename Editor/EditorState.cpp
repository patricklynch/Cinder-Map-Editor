#include "EditorState.h"

#include "cinder/app/App.h"
using namespace ci::app;

using namespace ly;

EditorState::EditorState()
{
	resetDefaults();
	
	/*testFloat = EditorValue<float>( 0.0f, -2.0f, 4.0f );
	
	testFloat = 1.0f;
	testInt = 5;
	testType = MODE_PAINT_ELEVATION;
	
	if ( testFloat == 1.0f ) console() << "PASS" << std::endl;
	else  console() << "FAIL" << std::endl;
	if ( testFloat != 1.0f ) console() << "FAIL" << std::endl;
	else console() << "PASS" << std::endl;
	
	
	if ( testInt != 5 ) console() << "FAIL" << std::endl;
	else console() << "PASS" << std::endl;
	if ( testInt == 5 ) console() << "PASS" << std::endl;
	else  console() << "FAIL" << std::endl;*/
}

void EditorState::resetDefaults()
{
	mode						= MODE_PAINT_TEXTURE;
	
	texturePaint.brushSize		= 1;
	texturePaint.brushStrength	= 1.0f;
	texturePaint.textureIndex	= 2;
	texturePaint.brushIndex		= 2;
	
	elevationHeight				= 1;
	elevationRange				= 3;
	elevationSelection			= 0;

	showGrid					= false;
}

void EditorState::modifySelection( int amount )
{
	// TODO: Clamp to min/max
	
	if ( mode == MODE_PAINT_TEXTURE ) {
		texturePaint.brushSize += amount;
	}
	else if ( mode == MODE_PAINT_ELEVATION ) {
		elevationRange += amount;
	}
}

void EditorState::modifyValue( float amount )
{
	if ( mode == MODE_PAINT_TEXTURE ) {
		texturePaint.brushStrength += amount * 0.1f;
	}
	else if ( mode == MODE_PAINT_ELEVATION ) {
		elevationHeight += (int) amount;
	}
}