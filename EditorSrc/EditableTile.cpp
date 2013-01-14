#include "EditableTile.h"
#include "Camera.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

EditableTile::EditableTile( Tile* tile ) : mTile( tile ), mIsSelected( false ) {}

void EditableTile::update( const float deltaTime )
{
	Vec3f size = mTile->node().size;
	size = Vec3f( size.x, size.y, size.z );
	Vec3f pos = mTile->node().position;
	Vec3f anchor = mTile->node().anchor;
	pos += mTile->node().anchoredPosition();
	Vec3f min = pos - size * 0.5f;
	Vec3f max = pos + size * 0.5f;
	mBoundingBox = AxisAlignedBox3f( min, max );
}

void EditableTile::setTexture( int index )
{
	Vec2i loc = Vec2i::zero();
	int max = mTile->node().textureUnitSize();
	loc.y = ci::math<int>::floor( index / max );
	loc.x = index - loc.y * max;
	mTile->node().textureLoc = loc;
}

void EditableTile::setElevation(float value)
{
	mTile->node().size.y += value;
}

bool EditableTile::pick( ci::Ray ray )
{
	return mBoundingBox.intersects( ray );
}

float EditableTile::cameraDistance()
{
	return mTile->node().position.distance( ly::Camera::get()->globalPosition() );
}

bool EditableTile::sortCameraDistance( EditableTile* a, EditableTile* b )
{
	return false;
	return a->cameraDistance() <= b->cameraDistance();
}

void EditableTile::draw()
{
	if ( mIsSelected )
		gl::drawStrokedCube( mBoundingBox );
}