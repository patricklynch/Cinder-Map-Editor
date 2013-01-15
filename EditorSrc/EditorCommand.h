#pragma once

#include "EditableTile.h"
#include "TIle.h"

#include "cinder/Vector.h"

namespace ly {

class EditorCommand {
public:
	EditorCommand() {}
	virtual bool execute() = 0;
	virtual bool undo() = 0;
};

	
class ChangeTileTextureCommand : public EditorCommand {
public:
	ChangeTileTextureCommand( EditableTile* tile, ci::Vec2i textureLoc ) : mTile( tile )
	{
		mTargetTextureLoc = textureLoc;
		mCurrentTextureLoc = mTile->tile().node().textureLoc;
	}
	virtual bool execute()
	{
		if ( mTile->tile().node().textureLoc == mTargetTextureLoc) return false;
		mTile->setTextureLoc( mTargetTextureLoc );
		return true;
	}
	virtual bool undo()
	{
		mTile->setTextureLoc( mCurrentTextureLoc );
		return true;
	}
private:
	EditableTile* mTile;
	ci::Vec2i mTargetTextureLoc;
	ci::Vec2i mCurrentTextureLoc;
};
	

class ChangeTileElevationCommand : public EditorCommand {
public:
	ChangeTileElevationCommand( EditableTile* tile, float elevation ) : mTile( tile )
	{
		mTargetElevation = elevation;
	}
	virtual bool execute()
	{
		if ( mTargetElevation == 0.0f ) return false;
		mTile->setElevation( mTargetElevation );
		return true;
	}
	virtual bool undo()
	{
		mTile->setElevation( -mTargetElevation );
		return true;
	}
private:
	EditableTile* mTile;
	float mTargetElevation;
};
	
}