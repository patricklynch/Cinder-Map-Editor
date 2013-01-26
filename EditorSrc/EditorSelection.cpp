#include "EditorSelection.h"
#include "Camera.h"
#include "GameConstants.h"
#include "Terrain.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

bool testModeBlock = false;

EditorSelection::EditorSelection( ci::Vec3f iPosition ) : isPoint( false ), mIsSelected( false ), mBlock( NULL )
{
	tilePosition = iPosition;
	position = tilePosition * kTileSize;
	defineBoundingBox();
}

void EditorSelection::defineBoundingBox()
{
	Vec3f min = -Vec3f( 0.5f, 0.0f, 0.5f ) * kTileSize;
	Vec3f max = Vec3f( 0.5f, 1.0f, 0.5f ) * kTileSize;
	mBoundingBox = AxisAlignedBox3f( position + min, position + max );
}

void EditorSelection::resetTilePosition( ci::Vec3f iPosition )
{
	if ( iPosition == tilePosition ) return;
	tilePosition = iPosition;
	position = tilePosition * kTileSize;
	defineBoundingBox();
}

void EditorSelection::update( const float deltaTime )
{
}

bool EditorSelection::pick( ci::Ray ray )
{
	return mBoundingBox.intersects( ray );
}

float EditorSelection::cameraDistance()
{
	return position.distance( ly::Camera::get()->globalPosition() );
}

bool EditorSelection::sortCameraDistance( EditorSelection* a, EditorSelection* b )
{
	return a->cameraDistance() <= b->cameraDistance();
}

void EditorSelection::draw()
{
	if ( mIsSelected ) {
		gl::enableWireframe();
		gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
		gl::drawStrokedCube( mBoundingBox );
		
		/*glPushMatrix();
		glMultMatrixf( mBlock->mNode->transform().m );
		gl::draw( *mBlock->mNode->mVboMesh );
		glPopMatrix();*/
	}
}


