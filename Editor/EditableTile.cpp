#include "EditableTile.h"
#include "Camera.h"
#include "GameConstants.h"
#include "AssetManager.h"
#include "Terrain.h"
#include "Editor.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

bool testModeBlock = false;

EditableTile::EditableTile( Tile* tile, Editor* editor ) : mEditor( editor ), mIsHighlighted( false ), mTile( tile ), mHasBeenEdited( false ), mElevationDidUpdate( false )
{
	defineBoundingBox();
}

void EditableTile::setSelectionMode( SelectionMode_t mode )
{
	if ( mode == selectionMode ) return;
	selectionMode = mode;
	defineBoundingBox();
}

void EditableTile::defineBoundingBox()
{
	Vec3f offset;
	if ( selectionMode == SELECTION_POINT ) {
		offset = -Vec3f( 0.5f, 0.0f, 0.5f ) * kTileSize;
	} else if ( selectionMode == SELECTION_FACE ) {
		offset = Vec3f::zero();
	}
	
	Vec2i tilePos = mTile->getTilePosition();
	Vec3f position = Vec3f( tilePos.x, kMinTileElevation, tilePos.y ) * kTileSize;
	Vec3f min = -Vec3f( 0.5f, 0.0f, 0.5f ) * kTileSize;
	Vec3f max = Vec3f( 0.5f, mTile->getElevation() - kMinTileElevation + 1.0f, 0.5f ) * kTileSize;
	mBoundingBoxCenter = position - offset;
	mBoundingBox = AxisAlignedBox3f( mBoundingBoxCenter + min, mBoundingBoxCenter + max );
}

void EditableTile::update( const float deltaTime )
{
}

bool EditableTile::pick( ci::Ray ray )
{
	return mBoundingBox.intersects( ray );
}

void EditableTile::editingComplete()
{
	mHasBeenEdited = false;
	defineBoundingBox();
}

void EditableTile::editingStarted()
{
	mHasBeenEdited = true;
}

float EditableTile::cameraDistance()
{
	Vec2i tilePos = mTile->getTilePosition();
	Vec3f position = Vec3f( tilePos.x, mTile->getElevation(), tilePos.y ) * kTileSize;
	return position.distance( ly::Camera::get()->globalPosition() );
}

bool EditableTile::sortCameraDistance( EditableTile* a, EditableTile* b )
{
	return a->cameraDistance() <= b->cameraDistance();
}

bool EditableTile::sortHeight( EditableTile* a, EditableTile* b )
{
	return a->getTile()->getElevation() <= b->getTile()->getElevation();
}

void EditableTile::draw( int elevationHeight, bool gridLines )
{
	if ( mIsHighlighted ) {
	
		/*gl::disableWireframe();
		gl::enableAlphaBlending();
		Vec3f size = Vec3f( 1.0f, elevationHeight, 1.0f );
		Vec3f center = mBoundingBox.getCenter() + Vec3f( 0.0f, elevationHeight * 0.5f, 0.0f );
		
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 0.1f ) );
		gl::drawCube( center, size );
		
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 1.0f ) );
		gl::drawStrokedCube( center, size );*/
		
		int min = math<int>::max( kMinTileElevation, mTile->getElevation() );
		gl::enableAlphaBlending();
		Vec3f size = mBoundingBox.getSize();
		size.y = -min + math<int>::max( elevationHeight, mTile->getElevation() );
		Vec3f pos = mBoundingBox.getCenter();
		pos.y = min + size.y * 0.5f + 1.0f;
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 0.1f ) );
		gl::drawCube( pos, size );
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 1.0f ) );
		gl::drawStrokedCube( pos, size );
	}
	
	/*
	Vec3f pos = mBoundingBox.getCenter();
	Vec3f size = mBoundingBox.getSize();
	gl::color( 1, 0, 0, 1 );
	gl::enableWireframe();
	gl::drawCube( pos, size );
	gl::disableWireframe();
	 */
}


