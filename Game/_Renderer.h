#pragma once

#include "cinder/gl/Texture.h"

#include "Node.h"
#include "Camera.h"

#include <vector>

namespace ly {
	
class Renderer {
public:
	static Renderer* get();
	virtual ~Renderer() {}
	void draw();
	void addNode( Node* node );
	void addNodes( std::vector<Node*>& node );
	void removeNode( Node* node );
	
private:
	static Renderer* sInstance;
	Renderer();
	
	ly::Camera* mCamera;
	std::vector<Node*> mNodes;
};

}