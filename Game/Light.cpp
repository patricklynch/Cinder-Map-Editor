#include "Light.h"
#include "Renderer.h"

#include "cinder/app/App.h"

using namespace ly;

using namespace ci;

ly::Light::Light()
{
	mNode = new Node();
	
	mSpriteNode = new Node();
	mSpriteNode->setIsUnlit( true );
	mSpriteNode->setColor( ColorA::white() );
	mSpriteNode->parent = mNode;
	
	mLight = new gl::Light( gl::Light::POINT, 0 );
	mLight->setShadowParams( 40.0f, 500.0f, 4000.0f );
	mLight->enable();
}

ly::Light::~Light()
{
	delete mNode;
	delete mSpriteNode;
	delete mLight;
}

void ly::Light::update( const float deltaTime )
{
	mNode->update( deltaTime );
	mSpriteNode->update( deltaTime );
	updateCinderLight();
}

void ly::Light::updateCinderLight()
{
	ci::Vec3f pos = mNode->globalPosition();
	mLight->setPosition( pos );
	mLight->setDirection( (target - pos).normalized() );
}