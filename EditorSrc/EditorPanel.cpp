#include "EditorPanel.h"
#include "EditorConstants.h"

#include <boost/bind.hpp>

using namespace ly;
using namespace ci;


EditorPalette::EditorPalette()
{
	int height = 50;
	innerMargin = 10;
	int thumbSize = height-innerMargin*2;
	int numThumbnails = 10;
	for( int i = 0; i < numThumbnails; i++ ) {
		EditorThumbnail* thumbnail = new EditorThumbnail();
		thumbnail->palette = this;
		thumbnail->size = Vec2i( thumbSize, thumbSize );
		thumbnail->position = Vec2i( innerMargin + i * ( thumbnail->size.x + innerMargin ), innerMargin );
		mThumbnails.push_back( thumbnail );
	}
	size = Vec2i( innerMargin + numThumbnails * (thumbSize + innerMargin), height );
}

void EditorPalette::draw()
{
	Vec2i p = position;
	Rectf r = Rectf( p.x, p.y, p.x + size.x, p.y + size.y );
	gl::color( 0.2f, 0.2f, 0.2f, 0.5f );
	gl::drawSolidRect( r );
	gl::color( 0.6f, 0.6f, 0.6f, 1.0f );
	gl::drawStrokedRect( r );
	
	for( std::vector<EditorThumbnail*>::iterator iter = mThumbnails.begin(); iter != mThumbnails.end(); iter++ ) {
		(*iter)->draw();
	}
}

void EditorThumbnail::draw()
{
	Vec2i parentOffset = Vec2i::zero();
	if ( palette != NULL ) parentOffset += palette->position;
	Vec2i p = parentOffset + position;
	Rectf r = Rectf( p.x, p.y, p.x + size.x, p.y + size.y );
	gl::color( 1.0f, 0.2f, 0.2f, 1.0f );
	gl::drawSolidRect( r );
	gl::color( 0.6f, 0.6f, 0.6f, 1.0f );
	gl::drawStrokedRect( r );
}

EditorPanel::EditorPanel()
{
	mParams = params::InterfaceGl( "EDITOR", Vec2i( 200, 400 ) );
	mParams.addSeparator();
	
	/*std::vector<std::string> toolStrings;
	toolStrings.push_back( "Selection" );
	toolStrings.push_back( "Texture" );
	toolStrings.push_back( "Elevation" );
	toolStrings.push_back( "Object" );
	mCurrentTool = EditorPaintSelection2;
	 mParams.addParam( "CURRENT TOOL", toolStrings, &mCurrentTool );*/
	
	brushSize = 3;
	mParams.addParam( "Brush Size", &brushSize, "max=10 min=1 step=1" );
	
	brushErase = false;
	mParams.addParam( "Erase", &brushErase );
	
	mParams.addSeparator();
	mParams.addButton( "UNDO", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "REDO", boost::bind( &EditorPanel::test, this ) );
	
	mParams.addSeparator();
	mParams.addButton( "NEW", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "OPEN", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "SAVE", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "RESET", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "COPY", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "EXIT", boost::bind( &EditorPanel::test, this ) );

	
	mTexturePicker.position = Vec2i( 240, 15 );
	
	/*mParams.addParam( "Mix Red", &mMixColorRed, "min=-1.0 max=1.0 step=0.01 keyIncr=r keyDecr=R" );
	 mParams.addParam( "Mix Green", &mMixColorGreen, "min=-1.0 max=1.0 step=0.01 keyIncr=g keyDecr=G" );
	 mParams.addParam( "Mix Blue", &mMixColorBlue, "min=-1.0 max=1.0 step=0.01 keyIncr=b keyDecr=B" );*/
}

EditorPanel::~EditorPanel() {}


void EditorPanel::test()
{
	std::cout << "TEST" << std::endl;
}

void EditorPanel::draw()
{
	params::InterfaceGl::draw();
	
	gl::enableAlphaBlending();
	gl::disableDepthRead();
	mTexturePicker.draw();
}

void EditorPanel::update( const float deltaTime )
{
	
}