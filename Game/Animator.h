#pragma once

#include <vector>
#include <boost/function.hpp>

#include "cinder/Vector.h"

namespace ly {

static int animationIdCounter = 0;
	
class Animation {
public:
	Animation( float* value, float duration, float from, float to );
	Animation( float duration, boost::function<void()> onComplete );
	
	int animationId;
	bool isComplete() const;
	void update( const float deltaTime );
	bool operator==( Animation* a ) { return a->animationId == animationId; }
	
	void onComplete( boost::function<void()> onComplete ) { mOnComplete = onComplete; }
	void onUpdate( boost::function<void()> onUpdate ) { mOnUpdate = onUpdate; }
	
private:
	float start;
	float end;
	float* value;
	float duration;
	float position;
	float delayValue;
	boost::function<void()> mOnUpdate;
	boost::function<void()> mOnComplete;
};

class Animator {
public:
	static Animation& animate( float* value, float from, float to, float duration );
	
	static int schedule( float delay, boost::function<void()> onComplete );
	
	static void unschedule( int animationId );
	
	void drainDeletionQueue();
	
	static void update( const float deltaTime );
	static Animator* get();
	
private:
	Animator() {}
	std::vector<Animation*> mAnimations;
	std::vector<Animation*> mDeletionQueue;
	static Animator* sInstance;
};

}