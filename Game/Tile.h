#pragma once

#include "Node.h"

#include "cinder/Vector.h"

namespace ly {

class Tile {
public:
	Tile( int startX, int startY );
	virtual ~Tile();
	
	virtual void update( const float deltaTime );
	static ci::Vec2i tilePosition( ci::Vec3f realPosition );
	static ci::Vec3f realPosition( ci::Vec2i tilePosition );
	
	int x;
	int y;
	
	Node& node() { return *mNode; }
	float elevation() const { return mNode->size.z; }
	
private:
	Node* mNode;
};

}