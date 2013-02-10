#pragma once

#include "Node.h"

#include "cinder/gl/Light.h"

namespace ly {
	
class Renderer;

class Light {
public:
	Light();
	virtual ~Light();
	virtual void update( const float deltaTime );
	
	ci::Vec3f target;
	ci::gl::Light* cinderLight() const { return mLight; }
	Node& spriteNode() const { return *mSpriteNode; }
	Node& node() const { return *mNode; }
	
	bool operator==( ly::Light& light ) const { return &light.node() == &node(); }
	
protected:
	Node* mNode;
	Node* mSpriteNode;
	ci::gl::Light* mLight;
	virtual void updateCinderLight();
};

}