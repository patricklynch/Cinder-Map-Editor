#include "Collision.h"
#include "Animator.h"
#include "Renderer.h"

#include <boost/bind.hpp>

using namespace ly;

using namespace ci;

Strike::Strike() : lifetime( 0.0f ), mIsActive( false ), duration(0.1f) {}

void Strike::activate()
{
	mIsActive = true;
	lifetime = 0.0f;
	strikePosition = 0.0f;
	Animator::animate( &strikePosition, 0.0f, 1.0f, duration ).onComplete( boost::bind( &Strike::onComplete, this ) );
}

void Strike::deactivate()
{
	mIsActive = false;
	lifetime = 0.0f;
}

void Strike::onComplete()
{
	strikePosition = 0.0f;
}

void Strike::update( const float deltaTime )
{
	node->update( deltaTime );
	
	node->position.x = 1.0f + strikePosition * 2.0f;
	
	if ( mIsActive ) {
		lifetime += deltaTime;
		if ( lifetime >= duration ) {
			deactivate();
		}
	}
	
	if ( mIsActive ) node->setColor( ColorA( 1, 1, 0, 1 ) );
	else node->setColor( ColorA( 0, 1, 0, 1 ) );
}


AttackZone::AttackZone() : lifetime( 0.0f ), mIsHit( false ), duration(0.1f) {}

void AttackZone::activate()
{
	mIsHit = true;
	lifetime = 0.0f;
}

void AttackZone::update( const float deltaTime )
{
	node->update( deltaTime );
	
	if ( mIsHit ) {
		lifetime += deltaTime;
		if ( lifetime >= duration ) {
			mIsHit = false;
			lifetime = 0.0f;
		}
	}
	
	if ( mIsHit ) node->setColor( ColorA( 1, 0, 0, 1 ) );
	else node->setColor( ColorA( 1, 1, 1, 1 ) );
}

Hit::Hit( ci::Vec3f position, float duration )
{
	mNode = new Node( NodeTypeSprite );
	mNode->position = position;
	mNode->position.z  = 0.5f;
	mNode->size = Vec3f::one();
	ly::Renderer::get()->addNode( mNode );
	Animator::animate( &mAlpha, 1.0f, 0.0f, duration ).onUpdate( boost::bind( &Hit::animationUpdate, this ) );
}

Hit::~Hit()
{
	ly::Renderer::get()->removeNode( mNode );
}

void Hit::update( const float deltaTime )
{
	mNode->update( deltaTime );
}

void Hit::animationUpdate()
{
	mNode->colors[ MaterialDiffuse ].a = mAlpha;
}