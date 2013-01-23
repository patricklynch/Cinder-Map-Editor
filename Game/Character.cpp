#include "Character.h"
#include "Tile.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "GameConstants.h"

#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"
#include "cinder/CinderMath.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Character::Character()
{
	/*const gl::Texture* texture1 = AssetManager::get()->getTexture( "char_1.png" );
					
	mNode = new Node();
	mNode->rotation.x = -90.0f;
	mNode->scale = Vec3f( kTileSize, kTileSize, kTileSize ) * 1.0f;
	mNode->position.y = 2.0f;
	mNode->setTexture( &texture1 );
	ly::Renderer::get()->addNode( mNode );*/
}

void Character::onEnterTile( const Tile& tile )
{
	mNode->position.y = tile.elevation() + 0.1f;
}

Character::~Character()
{
	ly::Renderer::get()->removeNode( mNode );
}

void Character::move( const ci::Vec2f movement )
{
	if ( movement == Vec2i::zero() ) return;
	
	mMovement = movement;
	float tan = ci::math<float>::atan2( mMovement.x, mMovement.y );
	mNode->rotation.y = toDegrees( tan );
	mNode->position += Vec3f( mMovement.x, 0.0f, mMovement.y ) * 0.05f;
}

void Character::update( const float deltaTime )
{
	Vec2f dir = mMovement.normalized();
	mFocalPoint = mNode->position + Vec3f( dir.x, 0.0f, dir.y ) * 0.1f;
	mFocalPoint = mNode->position;
	mNode->update( deltaTime );
}