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

EditorSelection::EditorSelection( ci::Vec3f iPosition, Block* block ) : mIsSelected( false ), mBlock( block ), mHasBeenEdited( false ), mBlockMeshType( BlockMeshCenter ), mNeedsSurroundingUpdate( false )
{
	tilePosition = iPosition;
	position = tilePosition * kTileSize;
	defineBoundingBox();
}

void EditorSelection::setBlockMeshType( BlockMeshType type )
{
	//if ( type == mBlockMeshType ) return;
	
	switch( mBlockMeshType ) {
		case BlockMeshCenter:
			mBlock->mNode->mVboMesh = AssetManager::get()->getVboMesh( "models/cube.obj" );
			break;
		case BlockMeshEdgeWall:
			mBlock->mNode->mVboMesh = AssetManager::get()->getVboMesh( "models/edge_straight.obj" );
			break;
		case BlockMeshEdgeOuterCorner:
			mBlock->mNode->mVboMesh = AssetManager::get()->getVboMesh( "models/edge_diagonal.obj" );
			break;
		default:
			break;
	}
	mBlockMeshType = type;
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
	mBlock->tilePosition = tilePosition;
	mBlock->mNode->position = position;
}

void EditorSelection::updateSurrounding( std::vector<EditorSelection*>& selections )
{
	mSurroundings.clear();
	for( int i = 0; i < 8; i++ ) {
		mSurroundings.push_back( NULL );
	}
	
	for( std::vector<EditorSelection*>::iterator iter = selections.begin(); iter != selections.end(); iter++) {
		EditorSelection* edge = *iter;
		Vec2i relPos = edge->tilePosition.xz() - tilePosition.xz();
		if ( relPos.length() >= 2 ) continue;
		
		int relHeight = edge->tilePosition.y - tilePosition.y;
		SurroundingType index = NONE;
		if ( relPos == Vec2i( -1, -1 ) && relHeight == 0 )	index = BL;
		if ( relPos == Vec2i(  0, -1 ) && relHeight == 0 )	index = BC;
		if ( relPos == Vec2i(  1, -1 ) && relHeight == 0 )	index = BR;
		if ( relPos == Vec2i( -1,  0 ) && relHeight == 0 )	index = ML;
		if ( relPos == Vec2i(  1,  0 ) && relHeight == 0 )	index = MR;
		if ( relPos == Vec2i( -1,  1 ) && relHeight == 0 )	index = TL;
		if ( relPos == Vec2i(  0,  1 ) && relHeight == 0 )	index = TC;
		if ( relPos == Vec2i(  1,  1 ) && relHeight == 0 )	index = TR;
				
		if ( index != NONE ) {
			mSurroundings[ (int) index ] = *iter;
			
			MeshPositionData& md = mMeshPositionData;
			md.top				= mSurroundings[ TL ] != NULL && mSurroundings[ TC ] != NULL && mSurroundings[ TR ] != NULL;
			md.bottom			= mSurroundings[ BL ] != NULL && mSurroundings[ BC ] != NULL && mSurroundings[ BR ] != NULL;
			md.horizontal		= mSurroundings[ MR ] != NULL && mSurroundings[ ML ] != NULL;
			md.vertical			= mSurroundings[ TC ] != NULL && mSurroundings[ BC ] != NULL;
			md.br				= mSurroundings[ BR ] != NULL;
			md.bc				= mSurroundings[ BC ] != NULL;
			md.bl				= mSurroundings[ BL ] != NULL;
			md.mr				= mSurroundings[ MR ] != NULL;
			md.ml				= mSurroundings[ ML ] != NULL;
			md.tr				= mSurroundings[ TR ] != NULL;
			md.tc				= mSurroundings[ TC ] != NULL;
			md.tl				= mSurroundings[ TL ] != NULL;
			md.rowl				= mSurroundings[ BL ] != NULL && mSurroundings[ ML ] != NULL && mSurroundings[ TL ] != NULL;
			md.rowr				= mSurroundings[ BR ] != NULL && mSurroundings[ MR ] != NULL && mSurroundings[ TR ] != NULL;
			md.rowt				= mSurroundings[ TL ] != NULL && mSurroundings[ TC ] != NULL && mSurroundings[ TR ] != NULL;
			md.rowb				= mSurroundings[ BL ] != NULL && mSurroundings[ BC ] != NULL && mSurroundings[ BR ] != NULL;
		}
	}
}

void EditorSelection::updateMesh()
{
	if ( mSurroundings.size() == 0 ) return;
	BlockMeshType newType = mBlockMeshType;
	
	float rotationY = 0.0f;
	
	MeshPositionData& md = mMeshPositionData;
	if ( md.horizontal && md.vertical ) {
		newType = BlockMeshCenter;
	}
	
	else if ( md.horizontal && md.tc ) {
		newType = BlockMeshEdgeWall;
		rotationY = -90.0f;
	}
	else if ( md.horizontal && md.bc ) {
		newType = BlockMeshEdgeWall;
		rotationY = 90.0f;
	}
	else if ( md.vertical && md.mr ) {
		newType = BlockMeshEdgeWall;
		rotationY = 0.0f;
	}
	else if ( md.vertical && md.ml ) {
		newType = BlockMeshEdgeWall;
		rotationY = 180.0f;
	}
	
	else if ( md.mr && md.tc && !md.ml && !md.bc ) {
		newType = BlockMeshEdgeOuterCorner;
		rotationY = 0.0f;
	}
	else if ( md.ml && md.tc && !md.mr && !md.bc ) {
		newType = BlockMeshEdgeOuterCorner;
		rotationY = -90.0f;
	}
	else if ( md.mr && md.bc && !md.ml && !md.tc ) {
		newType = BlockMeshEdgeOuterCorner;
		rotationY = 90.0f;
	}
	else if ( md.ml && md.bc && !md.mr && !md.tc ) {
		newType = BlockMeshEdgeOuterCorner;
		rotationY = 180.0f;
	}
	
	mBlock->mNode->rotation.y = rotationY;
	setBlockMeshType( newType );
	mNeedsSurroundingUpdate = false;
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


