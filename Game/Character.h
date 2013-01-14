#pragma once

#include "Node.h"
#include "Tile.h"

#include "cinder/Vector.h"

namespace ly {

class Character {
public:
	Character();
	virtual ~Character();
	
	virtual void update( const float deltaTime );
	void move( const ci::Vec2f movement );
	
	Node& node() { return *mNode; }
	void onEnterTile( const Tile& tile );
	ci::Vec3f focalPoint() const { return mFocalPoint; }
	
private:
	ci::Vec3f mFocalPoint;
	ci::Vec2f mMovement;
	Node* mNode;
};

}