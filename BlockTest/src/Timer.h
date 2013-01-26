#pragma once

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace ly {

class Timer {
public:
	Timer( boost::function<void(void)> callback, float limit, bool loop=false) : mCallback( callback ), mLimit( limit ), mLoop( loop ), mActive( false ), mCurrentTime( 0.0f ), mHasCallback( true ) {}
	
	Timer() : mHasCallback( false ) {}
	
	virtual ~Timer() {}
	
	boost::function<void (void)> mCallback;
	
	void activate( bool andReset = false)
	{
		mActive = true;
		if( andReset ) reset();
	}
	
	void deactivate( bool andReset = false)
	{
		mActive = false;
		if( andReset ) reset();
	}
	
	void reset() { mCurrentTime = 0; }
	
	void update( const float deltaTime ) {
		if ( !mHasCallback ) return;
		
		if (mActive) {
			mCurrentTime += deltaTime;
			if (mCurrentTime >= mLimit) {
				mCurrentTime = 0;
				if ( !mLoop ) mActive = false;
				mCallback();
			}
		}
	}
	
	bool loops() const { return mLoop; }
	float currentTime() const { return mCurrentTime; }
	float limit() const { return mLimit; }
	void setLimit( const float limit ) { mLimit = limit; }
	void setLoops( const bool loops ) { mLoop = loops; }
	
private:
	bool mHasCallback;
	bool mActive;
	bool mLoop;
	float mCurrentTime;
	float mLimit;
};
}