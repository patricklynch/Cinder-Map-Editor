#pragma once

#include "EditorSelection.h"
#include "Terrain.h"
#include "EditorSelection.h"

#include "cinder/Vector.h"

#include <vector>

namespace ly {

class EditorCommand {
public:
	EditorCommand() {}
	virtual bool					execute() = 0;
	virtual bool					undo() = 0;
};
	
class Editor;

class EditorCommandModifyElevation : public EditorCommand {
public:
	EditorCommandModifyElevation() {}
	virtual bool					execute();
	virtual bool					undo();
	std::vector<EditorSelection*>	activeSelections;
	int								amount;
	Editor*							editor;
private:
	std::vector<int>				previousAmounts;
};

}