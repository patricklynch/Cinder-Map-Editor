#pragma once

#include "EditorSelection.h"
#include "Terrain.h"

#include "cinder/Vector.h"

namespace ly {

class EditorCommand {
public:
	EditorCommand() {}
	virtual bool execute() = 0;
	virtual bool undo() = 0;
};
	
}