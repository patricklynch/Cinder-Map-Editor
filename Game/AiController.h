#pragma once

namespace ly {

class AiController {
public:
	AiController();
	virtual ~AiController();
	
	virtual void draw();
	virtual void update( const float deltaTime );
	
protected:
	
private:
	
};

}