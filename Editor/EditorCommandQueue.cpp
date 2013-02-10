#include "EditorCommandQueue.h"
#include "EditorConstants.h"

#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

EditorCommandQueue::EditorCommandQueue()
{
	
}

EditorCommandQueue::~EditorCommandQueue() {}

void EditorCommandQueue::update( const float deltaTime ) {}

void EditorCommandQueue::addCommand( EditorCommand* command )
{
	// If the current command is not the end, i.e. an undo has taken place, break off that future branch and delete it
	if ( mCommandQueue.size() > 0 && mCurrentCommand < mCommandQueue.end()-1 ) {
		for( std::vector<EditorCommand*>::iterator iter = mCurrentCommand+1; iter != mCommandQueue.end(); iter++ )
			delete *iter;
		mCommandQueue.erase( mCurrentCommand+1, mCommandQueue.end() );
	}
	// If the command can and did perform (returns true), add to the queue, and set the current command to the end of the queue
	if ( command->execute() ) {
		if ( mCommandQueue.size() >= kMaxElevationUndo ) {
			delete *mCommandQueue.begin();
			mCommandQueue.erase( mCommandQueue.begin() );
		}
		mCommandQueue.push_back( command );
		mCurrentCommand = mCommandQueue.end()-1;
	}
	else delete command;
}

EditorCommand* EditorCommandQueue::getCurrentCommand() const
{
	if ( mCommandQueue.size() == 0 ) return NULL;
	return *(mCommandQueue.end()-1);
}

void EditorCommandQueue::undo()
{
	// Make sure we have commands to undo
	if ( canUndo() ) {
		// Undo and check for success
		bool didSucceed = (*mCurrentCommand)->undo();
		if ( didSucceed ) {
			// Set the current to the previous
			mCurrentCommand--;
		}
		// If it didn't succeed, and the reason is not because it's at the bottom of the queue, there's a problem to address:
		else if ( canUndo() ) {
			// If undo wasn't possible, remove the command from the queue
			std::vector<EditorCommand*>::iterator toErase = mCurrentCommand;
			mCurrentCommand = toErase-1;
			delete *toErase;
			mCommandQueue.erase( toErase );
			
			// If a command fails to undo, undo the next one, that way there is no lack of response
			// Note: this will work recursively for multiple commands that fail to undo
			undo();
		}
	}
}

void EditorCommandQueue::redo()
{
	if ( !canRedo() ) return;
	mCurrentCommand++;
	(*mCurrentCommand)->execute();
}

bool EditorCommandQueue::canUndo() const
{
	return mCommandQueue.size() > 0 && mCurrentCommand >= mCommandQueue.begin();
}

bool EditorCommandQueue::canRedo() const
{
	return mCommandQueue.size() > 0 && mCurrentCommand < mCommandQueue.end()-1;
}