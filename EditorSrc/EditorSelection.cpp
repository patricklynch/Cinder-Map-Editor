#include "EditorSelection.h"
#include "Camera.h"
#include "GameConstants.h"
#include "AssetManager.h"
#include "Terrain.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

bool testModeBlock = false;

EditorSelection::EditorSelection( Block* block ) : mIsSelected( false ), mBlock( block ), mHasBeenEdited( false ), mBlockMeshType( BlockMeshCenter )
{
	tilePosition = block->tilePosition;
	position = tilePosition * kTileSize;
	defineBoundingBox();
}

void EditorSelection::setBlockMeshType( BlockMeshType type )
{
}

void EditorSelection::defineBoundingBox()
{
	Vec3f min = -Vec3f( 0.5f, 0.0f, 0.5f ) * kTileSize;
	Vec3f max = Vec3f( 0.5f, 1.0f, 0.5f ) * kTileSize;
	mBoundingBox = AxisAlignedBox3f( position + min, position +  max );
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
	mBlock->tilePosition = tilePosition;
	mBlock->mNode->position = position;
	updateMesh( tilePosition.y, &mBlock->mTopVboMesh, &mBlock->mRotationTop );
	for( int i = 0; i < tilePosition.y; i++  ) {
		updateMesh( i, &mBlock->mStackVboMesh, &mBlock->mRotationStack );
	}
}

void EditorSelection::updateSurrounding( std::vector<EditorSelection*>& selections )
{
	// This vector holds pointers to surrounding pieces.  The indicies correspond to specific
	// relation positions, so we need to fill it up with NULL values first in order to check
	// whether there is a block in that position.
	mSurroundings.clear();
	for( int i = 0; i < 8; i++ ) {
		mSurroundings.push_back( NULL );
	}
	
	int  n = kDefaultMaxVisibleTileRadius * 2;
	
	for( std::vector<EditorSelection*>::iterator iter = selections.begin(); iter != selections.end(); iter++) {
		EditorSelection* edge = *iter;
		Vec2i relPos = edge->tilePosition.xz() - tilePosition.xz();
		SurroundingType index = NONE;
		
		if (relPos == Vec2i( -1, -1 ) || relPos == Vec2i( n,  n ) ||
			relPos == Vec2i( -1,  n ) || relPos == Vec2i( n, -1 ) ) index = BL;
		
		if (relPos == Vec2i(  0, -1 ) || relPos == Vec2i(  0,  n ) ) index = BC;
		
		if (relPos == Vec2i(  1, -1 ) || relPos == Vec2i( -n,  n ) ||
			relPos == Vec2i( -n, -1 ) || relPos == Vec2i(  1,  n ) ) index = BR;
		
		if (relPos == Vec2i( -1,  0 ) || relPos == Vec2i(  n,  0 ) ) index = ML;
		
		if (relPos == Vec2i(  1,  0 ) || relPos == Vec2i( -n,  0 ) ) index = MR;
		
		if (relPos == Vec2i( -1,  1 ) || relPos == Vec2i(  n, -n ) ||
			relPos == Vec2i(  n,  1 ) || relPos == Vec2i( -1, -n ) ) index = TL;
		
		if (relPos == Vec2i(  0,  1 ) || relPos == Vec2i(  0, -n ) ) index = TC;
		
		if (relPos == Vec2i(  1,  1 ) || relPos == Vec2i( -n, -n ) ||
			relPos == Vec2i(  1, -n ) || relPos == Vec2i( -n,  1 ) ) index = TR;

		if ( index != NONE ) {
			mSurroundings[ (int) index ] = *iter;
		}
	}
}

void EditorSelection::updateMesh( int elevation, ci::gl::VboMesh** vboMesh, float* rotation )
{
	// Collect data in a nice short hand form for easier syntax when we analyze it
	// A true value for any property indicates that the space or spaces are filled
	// 'tr' means top right is filled, 'rowt' means to the whole top row is filled, 'horizontal' means the middle row is filled
	MeshPositionData& md	=	mMeshPositionData;
	md.top					=	mSurroundings[ TL ]->tilePosition.y >= elevation &&
								mSurroundings[ TC ]->tilePosition.y >= elevation &&
								mSurroundings[ TR ]->tilePosition.y >= elevation;
	md.bottom				=	mSurroundings[ BL ]->tilePosition.y >= elevation &&
								mSurroundings[ BC ]->tilePosition.y >= elevation &&
								mSurroundings[ BR ]->tilePosition.y >= elevation;
	md.horizontal			=	mSurroundings[ MR ]->tilePosition.y >= elevation &&
								mSurroundings[ ML ]->tilePosition.y >= elevation;
	md.vertical				=	mSurroundings[ TC ]->tilePosition.y >= elevation &&
								mSurroundings[ BC ]->tilePosition.y >= elevation;
	md.br					=	mSurroundings[ BR ]->tilePosition.y >= elevation;
	md.bc					=	mSurroundings[ BC ]->tilePosition.y >= elevation;
	md.bl					=	mSurroundings[ BL ]->tilePosition.y >= elevation;
	md.mr					=	mSurroundings[ MR ]->tilePosition.y >= elevation;
	md.ml					=	mSurroundings[ ML ]->tilePosition.y >= elevation;
	md.tr					=	mSurroundings[ TR ]->tilePosition.y >= elevation;
	md.tc					=	mSurroundings[ TC ]->tilePosition.y >= elevation;
	md.tl					=	mSurroundings[ TL ]->tilePosition.y >= elevation;
	md.rowl					=	mSurroundings[ BL ]->tilePosition.y >= elevation &&
								mSurroundings[ ML ]->tilePosition.y >= elevation &&
								mSurroundings[ TL ]->tilePosition.y >= elevation;
	md.rowr					=	mSurroundings[ BR ]->tilePosition.y >= elevation &&
								mSurroundings[ MR ]->tilePosition.y >= elevation &&
								mSurroundings[ TR ]->tilePosition.y >= elevation;
	md.rowt					=	mSurroundings[ TL ]->tilePosition.y >= elevation &&
								mSurroundings[ TC ]->tilePosition.y >= elevation &&
								mSurroundings[ TR ]->tilePosition.y >= elevation;
	md.rowb					=	mSurroundings[ BL ]->tilePosition.y >= elevation &&
								mSurroundings[ BC ]->tilePosition.y >= elevation &&
								mSurroundings[ BR ]->tilePosition.y >= elevation;
	
	if ( mSurroundings.size() == 0 ) return;
	
	BlockMeshType meshType = BlockMeshCenter;
	
	float rotationY = 0.0f;
	
	// Solid cube when compeltely surrounded
	if ( md.rowt && md.horizontal && md.rowb ) {
		meshType = BlockMeshCenter;
	}
	
	// Inner corner mesh and rotation
	// TODO: With the right models, do we even need this?
	else if ( md.horizontal && md.vertical && !md.tr ) {
		meshType = BlockMeshEdgeInnerCorner;
		rotationY = 180.0f;
	}
	else if ( md.horizontal && md.vertical && !md.tl ) {
		meshType = BlockMeshEdgeInnerCorner;
		rotationY = 90.0f;
	}
	else if ( md.horizontal && md.vertical && !md.br ) {
		meshType = BlockMeshEdgeInnerCorner;
		rotationY = -90.0f;
	}
	else if ( md.horizontal && md.vertical && !md.bl ) {
		meshType = BlockMeshEdgeInnerCorner;
		rotationY = 0.0f;
	}
	
	// Wall mesh and rotation
	else if ( md.horizontal && md.rowt ) {
		meshType = BlockMeshEdgeWall;
		rotationY = -90.0f;
	}
	else if ( md.horizontal && md.rowb ) {
		meshType = BlockMeshEdgeWall;
		rotationY = 90.0f;
	}
	else if ( md.vertical && md.rowr) {
		meshType = BlockMeshEdgeWall;
		rotationY = 0.0f;
	}
	else if ( md.vertical && md.rowl ) {
		meshType = BlockMeshEdgeWall;
		rotationY = 180.0f;
	}
	
	// Corner mesh and rotation
	else if ( md.mr && md.tc && !md.ml && !md.bc ) {
		meshType = BlockMeshEdgeOuterCorner;
		rotationY = 0.0f;
	}
	else if ( md.ml && md.tc && !md.mr && !md.bc ) {
		meshType = BlockMeshEdgeOuterCorner;
		rotationY = -90.0f;
	}
	else if ( md.mr && md.bc && !md.ml && !md.tc ) {
		meshType = BlockMeshEdgeOuterCorner;
		rotationY = 90.0f;
	}
	else if ( md.ml && md.bc && !md.mr && !md.tc ) {
		meshType = BlockMeshEdgeOuterCorner;
		rotationY = 180.0f;
	}
	
	// If a single block is surrounded by one or no other blocks, just push it back down
	else if (( md.tc && !md.bc && !md.mr && !md.ml ) || 
			 ( !md.tc && md.bc && !md.mr && !md.ml ) || 
			 ( !md.tc && !md.bc && md.mr && !md.ml ) || 
			 ( !md.tc && !md.bc && !md.mr && md.ml ) ||
			 ( !md.rowt && !md.horizontal && !md.rowb ) ) {
		Vec3i tpos = tilePosition;
		tpos.y--;
		resetTilePosition( tpos );
	}
	
	*rotation = rotationY;
	
	switch( meshType ) {
		case BlockMeshCenter:
			*vboMesh = AssetManager::get()->getVboMesh( "models/cube.obj" );
			break;
		case BlockMeshEdgeWall:
			*vboMesh = AssetManager::get()->getVboMesh( "models/edge_straight.obj" );
			break;
		case BlockMeshEdgeOuterCorner:
			*vboMesh = AssetManager::get()->getVboMesh( "models/edge_diagonal.obj" );
			break;
		case BlockMeshEdgeInnerCorner:
			*vboMesh = AssetManager::get()->getVboMesh( "models/edge_inner_corner.obj" );
			break;
		default:
			break;
	}
}

bool EditorSelection::pick( ci::Ray ray )
{
	if ( mHasBeenEdited ) return false;
	return mBoundingBox.intersects( ray );
}

void EditorSelection::editingComplete()
{
	mHasBeenEdited = false;
}

void EditorSelection::editingStarted()
{
	mHasBeenEdited = true;
}

float EditorSelection::cameraDistance()
{
	return position.distance( ly::Camera::get()->globalPosition() );
}

bool EditorSelection::sortCameraDistance( EditorSelection* a, EditorSelection* b )
{
	return a->cameraDistance() <= b->cameraDistance();
}

bool EditorSelection::sortHeight( EditorSelection* a, EditorSelection* b )
{
	return a->tilePosition.y <= b->tilePosition.y;
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


