#pragma once

namespace ly {

class EditorMode {
public:
	EditorMode();
	virtual ~EditorMode();
	
	virtual void draw();
	virtual void update( const float deltaTime );
	
protected:
	
private:
	
};

}