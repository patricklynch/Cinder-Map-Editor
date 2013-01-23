#pragma once

#include "Node.h"

#include "cinder/Vector.h"

#include <vector>

namespace ly {

class Block {
public:
	Block( ci::Vec3i blockPos );
	virtual ~Block();
	virtual void update( const float deltaTime );
	
private:
	Node* mRoot;
	std::vector<Node*> mNodes;
};

}