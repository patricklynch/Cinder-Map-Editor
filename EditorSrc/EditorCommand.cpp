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
		previousAmounts.push_back( activeSelection->tilePosition.y );
		activeSelection->editingStarted();
	}
	for( iter = activeSelections.begin(); iter != activeSelections.end(); iter++ ) {
		EditorSelection* activeSelection = *iter;
		if ( editor->currentElevationTarget != activeSelection->tilePosition.y ) {
			editor->setElevation( activeSelection, amount );
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
		editor->setElevation( activeSelection, previousAmounts[i++] );
	}
	return true;
}