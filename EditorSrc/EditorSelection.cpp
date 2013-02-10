#include "EditorSelection.h"
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

EditorSelection::EditorSelection( Block* block, Editor* editor ) : mEditor( editor ), mIsHighlighted( false ), mBlock( block ), mHasBeenEdited( false ), mTopBlockMeshType( BlockMeshNone )
{
	tilePosition = block->tilePosition;
	position = tilePosition * kTileSize;
	defineBoundingBox();
}

void EditorSelection::setSelectionMode( SelectionMode_t mode )
{
	if ( mode == selectionMode ) return;
	selectionMode = mode;
	defineBoundingBox();
}

void EditorSelection::defineBoundingBox()
{
	Vec3f offset;
	if ( selectionMode == SELECTION_POINT ) {
		offset = -Vec3f( 0.5f, 0.0f, 0.5f ) * kTileSize;
	}
	else if ( selectionMode == SELECTION_FACE ) {
		offset = Vec3f::zero();
	}
	Vec3f min = -Vec3f( 0.5f, 0.0f, 0.5f ) * kTileSize;
	Vec3f max = Vec3f( 0.5f, 1.0f, 0.5f ) * kTileSize;
	mBoundingBoxCenter = position + offset;
	mBoundingBox = AxisAlignedBox3f( mBoundingBoxCenter + min, mBoundingBoxCenter + max );
}

void EditorSelection::resetTilePosition( ci::Vec3f iPosition )
{
	if ( iPosition == tilePosition ) return;
	tilePosition = iPosition;
	position = tilePosition * kTileSize;
	if ( !mHasBeenEdited ) defineBoundingBox();
}

void EditorSelection::update( const float deltaTime )
{
	mBlock->tilePosition = tilePosition;
	mBlock->mNode->position = position;
	
	updateMesh( mBlock );
	for( std::vector<Block*>::iterator iter = mBlockStack.begin(); iter != mBlockStack.end(); iter++ ) {
		updateMesh( *iter );
	}
}

void EditorSelection::addBlock( Block* block )
{
	Vec3i tilePos = tilePosition;
	tilePos.y -= 1;
	mBlockStack.push_back( block );
	update( 0.0f );
}

Block*  EditorSelection::removeBlock()
{
	if ( mBlockStack.size() > 0 ) {
		std::vector<Block*>::iterator top = mBlockStack.end()-1;
		Block* output = *top;
		mBlockStack.erase( top );
		return output;
	}
	return NULL;
}

void EditorSelection::findSurroundingBlocks( std::vector<EditorSelection*>& selections )
{
	mMeshSelector.setSurrounding( selections, tilePosition );
}

void EditorSelection::updateMesh( Block* block )
{
	EditorMeshSelectorResult result = mMeshSelector.getMeshSelection( block->tilePosition.y );
	block->setMeshType( result.type, result.rotation );
	
	// If it's the top block
	if ( mBlock->tilePosition.y == tilePosition.y ) {
		mTopBlockMeshType = result.type;
	}
}

bool EditorSelection::pick( ci::Ray ray )
{
	return mBoundingBox.intersects( ray );
}

void EditorSelection::editingComplete()
{
	mHasBeenEdited = false;
	defineBoundingBox();
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

void EditorSelection::draw( int elevationHeight, bool gridLines )
{
	gl::disableWireframe();
	gl::enableAlphaBlending();
	Vec3f size = mBoundingBox.getSize();
	size.y *= elevationHeight;
	Vec3f center = Vec3f( tilePosition.x, 1.0f + 0.5f * elevationHeight, tilePosition.z );
	
	if ( mIsHighlighted ) {
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 0.1f ) );
		gl::drawCube( center, size );
		
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 1.0f ) );
		gl::drawStrokedCube( center, size );
	}
	
	if ( gridLines && mTopBlockMeshType == BlockMeshFill ) {
		gl::color( ColorA::white() );
		Vec3f offset = Vec3f( 0.5f, 1.2f, 0.5f );
		offset = Vec3f( 0.5f, 1.2f, 0.5f );
		SurroundingType types[] = { BC, MR, TC, ML };
		for( int i = 0; i < sizeof( types ) / sizeof( SurroundingType ); i++ ) {
			gl::drawLine( mMeshSelector.surroundings()[ types[ i ] ]->position + offset, position + offset );
		}
	}
}


