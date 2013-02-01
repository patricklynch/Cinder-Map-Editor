#include "EditorCommandQueue.h"
#include "EditorConstants.h"

using namespace ly;

EditorCommandQueue::EditorCommandQueue()
{
	
}

EditorCommandQueue::~EditorCommandQueue() {}

void EditorCommandQueue::update( const float deltaTime ) {}

void EditorCommandQueue::addCommand( EditorCommand* command )
{
	// If the current command is not the end, i.e. an undo has taken place, break off that future branch and delete it
	if ( mCommandQueue.size() > 0 && currentCommand < mCommandQueue.end()-1 ) {
		for( std::vector<EditorCommand*>::iterator iter = currentCommand+1; iter != mCommandQueue.end(); iter++ )
			delete *iter;
		mCommandQueue.erase( currentCommand+1, mCommandQueue.end() );
	}
	// If the command can and did perform (returns true), add to the queue, and set the current command to the end of the queue
	if ( command->execute() ) {
		if ( mCommandQueue.size() >= kMaxUndo ) {
			delete *mCommandQueue.begin();
			mCommandQueue.erase( mCommandQueue.begin() );
		}
		mCommandQueue.push_back( command );
		currentCommand = mCommandQueue.end()-1;
	}
	else delete command;
}

void EditorCommandQueue::undo()
{
	if ( !canUndo() ) return;
	(*currentCommand)->undo();
	currentCommand--;
}

void EditorCommandQueue::redo()
{
	if ( !canRedo() ) return;
	currentCommand++;
	(*currentCommand)->execute();
}

bool EditorCommandQueue::canUndo() const
{
	return mCommandQueue.size() > 0 && currentCommand >= mCommandQueue.begin();
}

bool EditorCommandQueue::canRedo() const
{
	return mCommandQueue.size() > 0 && currentCommand < mCommandQueue.end()-1;
}