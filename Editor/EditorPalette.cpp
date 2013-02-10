#include "EditorPalette.h"
#include "Input.h"

#include "cinder/gl/Texture.h"
#include "cinder/Area.h"

#include <boost/bind.hpp>

using namespace ly;

using namespace ci;

EditorPalette::EditorPalette( std::vector<ci::gl::Texture*> textures, int* selectionIndexPtr ) : mSelectionIndex( selectionIndexPtr ), mDelegate( NULL )
{
	int height = 100;
	innerMargin = 10;
	int thumbSize = height-innerMargin*2;
	
	for( int i = 0; i < textures.size(); i++ ) {
		EditorThumbnail* thumbnail = new EditorThumbnail( i, textures[i] );
		thumbnail->palette = this;
		thumbnail->size = Vec2i( thumbSize, thumbSize );
		thumbnail->position = Vec2i( innerMargin + i * ( thumbnail->size.x + innerMargin ), innerMargin );
		mThumbnails.push_back( thumbnail );
	}
	size = Vec2i( innerMargin + textures.size() * (thumbSize + innerMargin), height );
}

void EditorPalette::thumbnailSelected( EditorThumbnail* thumbnail )
{
	*mSelectionIndex = thumbnail->index;
	mDelegate->didUpdateSelection( this, *mSelectionIndex );
}

void EditorPalette::update( const float deltaTime )
{
	for( std::vector<EditorThumbnail*>::iterator iter = mThumbnails.begin(); iter != mThumbnails.end(); iter++ ) {
		(*iter)->isSelected = (*iter)->index == *mSelectionIndex;
	}
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

EditorThumbnail::EditorThumbnail( int assignedIndex, ci::gl::Texture* texture ) : index( assignedIndex ), mTexture( texture ), palette( NULL ), isSelected( false )
{
	ly::Input::get()->addListenerForMouseDown( boost::bind( &EditorThumbnail::onClick, this, _1 ) );
}

void EditorThumbnail::onClick( ci::app::MouseEvent event )
{
	Vec2i p = globalPosition();
	Area area( p.x, p.y, p.x + size.x, p.y + size.y );
	if ( !area.contains( event.getPos() ) ) return;
	
	if ( palette != NULL ) {
		palette->thumbnailSelected( this );
	}
}

ci::Vec2i EditorThumbnail::globalPosition()
{
	Vec2i parentOffset = Vec2i::zero();
	if ( palette != NULL ) parentOffset += palette->position;
	return parentOffset + position;
}

void EditorThumbnail::draw()
{
	Vec2i p = globalPosition();
	int m = 2;
	Rectf r1 = Rectf( p.x, p.y, p.x + size.x, p.y + size.y );
	Rectf r2 = Rectf( r1.x1 - m, r1.y1 - m, r1.x2 + m, r1.y2 + m );
	gl::color( ColorA::black() );
	gl::drawSolidRect( r2 );
	gl::color( ColorA::white() );
	gl::draw( *mTexture, r1 );
	if ( isSelected ) {
		gl::color( 1.0f, 0.0f, 0.0f, 1.0f );
	} else {
		gl::color( 0.6f, 0.6f, 0.6f, 1.0f );
	}
	gl::drawStrokedRect( r2 );
}

