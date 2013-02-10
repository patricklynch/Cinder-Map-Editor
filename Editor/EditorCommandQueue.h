#pragma once

#include "EditorCommand.h"
#include "EditorCommand.h"

#include <vector>

namespace ly {

class EditorCommandQueue {
public:
											EditorCommandQueue();
	virtual									~EditorCommandQueue();
	
	void									update( const float deltaTime );
	void									addCommand( EditorCommand* command );
	void									redo();
	void									undo();
	bool									canUndo() const;
	bool									canRedo() const;
	
	EditorCommand*							getCurrentCommand() const;
	
private:
	std::vector<EditorCommand*>				mCommandQueue;
	std::vector<EditorCommand*>::iterator	mCurrentCommand;
	
};

}