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
	Vec3f position = getTilePosition() * kTileSize;
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

void EditorSelection::update( const float deltaTime )
{
	updateMesh( mBlock );
	for( std::vector<Block*>::iterator iter = mBlockStack.begin(); iter != mBlockStack.end(); iter++ ) {
		updateMesh( *iter );
	}
}

void EditorSelection::addBlock( Block* block )
{
	Vec3i tilePos = getTilePosition();
	tilePos.y -= 1;
	mBlockStack.insert( mBlockStack.begin(), block );
	update( 0.0f );
}

Block*  EditorSelection::removeBlock()
{
	if ( mBlockStack.size() > 0 ) {
		Block* output = *mBlockStack.begin();
		mBlockStack.erase( mBlockStack.begin() );
		return output;
	}
	return NULL;
}

void EditorSelection::findSurroundingBlocks( std::vector<EditorSelection*>& selections )
{
	mMeshSelector.setSurrounding( selections, getTilePosition() );
}

void EditorSelection::updateMesh( Block* block )
{
	EditorMeshSelectorResult result = mMeshSelector.getMeshSelection( block );
	block->setMeshType( result.type, result.rotation );
	
	// If it's the top block
	if ( mBlock->tilePosition.y == getTilePosition().y ) {
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
	Vec3f position = getTilePosition() * kTileSize;
	return position.distance( ly::Camera::get()->globalPosition() );
}

bool EditorSelection::sortCameraDistance( EditorSelection* a, EditorSelection* b )
{
	return a->cameraDistance() <= b->cameraDistance();
}

bool EditorSelection::sortHeight( EditorSelection* a, EditorSelection* b )
{
	return a->getTilePosition().y <= b->getTilePosition().y;
}

Block* EditorSelection::getBlockAtElevation( int elevation )
{
	if ( mBlock->tilePosition.y == elevation ) return mBlock;
	for( std::vector<Block*>::iterator iter = mBlockStack.begin(); iter != mBlockStack.end(); iter++ ) {
		if ( (*iter)->tilePosition.y == elevation ) {
			return *iter;
		}
	}
	return NULL;
}

void EditorSelection::draw( int elevationHeight, bool gridLines )
{
	gl::disableWireframe();
	gl::enableAlphaBlending();
	Vec3f size = mBoundingBox.getSize();
	size.y *= elevationHeight;
	Vec3f center = Vec3f( getTilePosition().x, 1.0f + 0.5f * elevationHeight, getTilePosition().z );
	
	if ( mIsHighlighted ) {
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 0.1f ) );
		gl::drawCube( center, size );
		
		gl::color( ColorA( 0.0f, 1.0f, 0.0f, 1.0f ) );
		gl::drawStrokedCube( center, size );
	}
	
	if ( gridLines ) {
		gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
		Vec3f targ = getTilePosition() + Vec3f(0.0f, 1.02f, 0.0f );
		gl::drawCube( targ, Vec3f( 0.2f, 0.05f, 0.2f ) );
		gl::drawLine( Vec3f( getTilePosition().x, 0.0f, getTilePosition().z ), targ );
	}
	
	//if ( gridLines && mTopBlockMeshType == BlockMeshCenter ) {
	if ( false ) {
		gl::color( ColorA::white() );
		Vec3f offset = Vec3f( 0.5f, 1.2f, 0.5f );
		offset = Vec3f( 0.5f, 1.2f, 0.5f );
		SurroundingType types[] = { BC, MR, TC, ML };
		for( int i = 0; i < sizeof( types ) / sizeof( SurroundingType ); i++ ) {
			Vec3f position = mMeshSelector.surroundings()[ types[ i ] ]->getTilePosition() * kTileSize;
			gl::drawLine( position + offset, position + offset );
		}
	}
}


