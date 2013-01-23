#include "Block.h"

#include "cinder/app/App.h"

#include "MeshHelper.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

Block::Block()
{
	mNode = new Node();
}

Block::~Block() {}

void Block::update( const float deltaTime )
{
	mNode->update( deltaTime );
}