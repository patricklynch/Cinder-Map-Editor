#pragma once

namespace ly {

enum Buttons {
	GameButtonLowKick,
	GameButtonLowPunch,
	GameButtonHighPunch,
	GameButtonHighKick,
	GameButtonJump,
	GameButtonBlock,
	EditorSave = 100,
	EditorOpen,
	EditorReset
};
	
class Touch {
public:
	ci::Vec2i position;
	ci::Vec2i prevPosition;
};

}