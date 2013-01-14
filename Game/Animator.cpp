#include "Animator.h"

#include "cinder/Easing.h"
#include "cinder/app/App.h"
#include "cinder/CinderMath.h"

#include <algorithm>

using namespace ly;

using namespace ci;
using namespace ci::app;


Animation::Animation( float* value, float duration, float from, float to ) : position( 0.0f ), mOnComplete( NULL ), mOnUpdate( NULL )
{
	this->value = value;
	this->duration = duration;
	this->start = from;
	this->end = to;
	animationId = animationIdCounter++;
}

Animation::Animation( float duration, boost::function<void()> onComplete ) : mOnComplete( NULL ), mOnUpdate( NULL )
{
	this->mOnComplete = onComplete;
	this->duration = duration;
	this->delayValue = 0.0f;
	this->value = &delayValue;
	animationId = animationIdCounter++;
}

bool Animation::isComplete() const
{
	return position >= duration;
}

void Animation::update( const float deltaTime )
{
	position += deltaTime;
	if ( isComplete() ) position = duration;
	*value = start + (end - start) * ci::easeInOutQuad( ci::math<float>::clamp( position / duration, 0.0f, 1.0f ) );
	if ( mOnUpdate != NULL ) mOnUpdate();
	if ( isComplete() && mOnComplete != NULL ) mOnComplete();
}

Animator* Animator::sInstance = NULL;

Animator* Animator::get()
{
	if ( !sInstance ) sInstance = new Animator();
	return sInstance;
}

void Animator::unschedule( int animationId )
{
	Animator* a = Animator::get();
	for( std::vector<Animation*>::iterator iter = a->mAnimations.begin(); iter != a->mAnimations.end(); iter++)
		if ( (*iter)->animationId == animationId )
			a->mDeletionQueue.push_back( *iter );
	a->drainDeletionQueue();
}

Animation& Animator::animate( float* value, float from, float to, float duration )
{
	Animation* animation = new Animation( value, duration, from, to);
	Animator::get()->mAnimations.push_back( animation );
	return *animation;
}

int Animator::schedule( float delay, boost::function<void()> onComplete )
{
	Animation* animation = new Animation( delay, onComplete );
	Animator::get()->mAnimations.push_back( animation );
	return animation->animationId;
}

void Animator::drainDeletionQueue()
{
	Animator* a = Animator::get();
	for( std::vector<Animation*>::iterator iter = mDeletionQueue.begin(); iter != mDeletionQueue.end(); iter++) {
		std::vector<Animation*>::iterator match = std::find( a->mAnimations.begin(), a->mAnimations.end(), *iter );
		if ( match != mDeletionQueue.end() ) {
			a->mAnimations.erase( match );
			delete *iter;
		}
	}
	a->mDeletionQueue.clear();
}

void Animator::update( const float deltaTime )
{
	Animator* a = Animator::get();
	std::vector<Animation*> mDeletionQueue;
	for( std::vector<Animation*>::iterator iter = a->mAnimations.begin(); iter != a->mAnimations.end(); iter++) {
		Animation* anim = *iter;
		anim->update( deltaTime );
		if ( anim->isComplete() ) a->mDeletionQueue.push_back( anim );
	}
	a->drainDeletionQueue();
}