#pragma once

#include "cinder/Vector.h"
#include "cinder/Rect.h"

namespace ly {

class LoadSequence {
public:
	LoadSequence();
	virtual ~LoadSequence();
	
	virtual void draw();
	virtual void update( const float deltaTime );
	void setProgress( float progress ) { mProgress = progress; }

private:
	ci::Rectf mLocalRect;
	float mProgress;
};

}