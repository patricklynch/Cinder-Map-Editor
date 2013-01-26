#include "Block.h"
#include "GameConstants.h"
#include "Terrain.h"

#include "cinder/app/App.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

Block::Block()
{
	mNode = new Node();
}

Block::~Block()
{
	delete mNode;
}

void Block::update( const float deltaTime )
{
	mNode->update( deltaTime );
}