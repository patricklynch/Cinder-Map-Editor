#include "EditorCommand.h"
#include "Editor.h"

using namespace ly;

using namespace ci;

bool EditorCommandModifyElevation::execute()
{
	std::vector<EditorSelection*>::iterator iter;
	bool elevationDidChange = false;
	for( iter = activeSelections.begin(); iter != activeSelections.end(); iter++ ) {
		EditorSelection* activeSelection = *iter;
		previousAmounts.push_back( activeSelection->getTilePosition().y );
		previousTerrainIndices.push_back( activeSelection->mBlock->terrainIndex() );
		activeSelection->editingStarted();
	}
	for( iter = activeSelections.begin(); iter != activeSelections.end(); iter++ ) {
		EditorSelection* activeSelection = *iter;
		if ( editor->currentElevationTarget != activeSelection->getTilePosition().y ) {
			editor->setElevation( activeSelection, amount, terrainIndex );
			elevationDidChange = true;
		}
	}
	return elevationDidChange;
}

bool EditorCommandModifyElevation::undo()
{
	int i = 0;
	std::vector<EditorSelection*>::iterator iter;
	for( iter = activeSelections.begin(); iter != activeSelections.end(); iter++ ) {
		EditorSelection* activeSelection = *iter;
		editor->setElevation( activeSelection, previousAmounts[i++], previousTerrainIndices[i] );
	}
	return true;
}

EditorCommandPaintTexture::EditorCommandPaintTexture( EditorTexturePaint* texturePaint, TexturePaintState state ) : mTargetState( state ), mTexturePaint( texturePaint ), mHasBeenApplied( false )
{}

void EditorCommandPaintTexture::addPosition( ci::Vec3f pos )
{
	mPositions.push_back( pos );
	mTexturePaint->setState( mTargetState );
	mTexturePaint->apply( pos );
}

void EditorCommandPaintTexture::commandComplete()
{
	mHasBeenApplied = true;
	mTexturePaint->editingComplete();
}

bool EditorCommandPaintTexture::execute()
{
	// Only if the command has already been applied, i.e. this is redo
	if ( mHasBeenApplied ) {
		mTexturePaint->setState( mTargetState );
		for( std::vector<Vec3f>::const_iterator iter = mPositions.begin(); iter != mPositions.end(); iter++ ) {
			mTexturePaint->apply( *iter );
		}
		mTexturePaint->editingComplete();
	}
	return true;
}

bool EditorCommandPaintTexture::undo()
{
	return mTexturePaint->popPaintLayer();
}