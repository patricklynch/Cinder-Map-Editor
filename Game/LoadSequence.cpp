#include "LoadSequence.h"

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

LoadSequence::LoadSequence() : mProgress( 0.0f )
{
	float width = 200;
	float height = 50;
	mLocalRect = Rectf( -width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f );
}

LoadSequence::~LoadSequence() {}

void LoadSequence::update( const float deltaTime )
{
	
}

void LoadSequence::draw()
{
	// Draw solid background
	float g = 0.2f;
	gl::color( g, g, g, 1.0f );
	gl::drawSolidRect( app::getWindowBounds() );
	
	glPushMatrix();
	{
		// Total bar
		Vec2i c = app::getWindowCenter();
		glTranslatef( c.x, c.y, 0.0f );
		gl::color( 0, 0, 0, 1 );
		gl::drawSolidRect( mLocalRect );
		
		// Loaded bar
		Rectf loadedRect = mLocalRect;
		loadedRect.x2 = loadedRect.x1 + mLocalRect.getWidth() * mProgress;
		gl::color( 1, 0, 0, 1 );
		gl::drawSolidRect( loadedRect );
		
		// Outline
		g = 0.7f;
		gl::color( g, g, g, 1.0f );
		gl::drawStrokedRect( loadedRect );
		
	}
	glPopMatrix();
}