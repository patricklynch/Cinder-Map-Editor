#pragma once

namespace ly {

class CharacterModel {
public:
	CharacterModel();
	virtual ~CharacterModel();
	
	virtual void draw();
	virtual void update( const float deltaTime );
	
protected:
	
private:
	
};

}