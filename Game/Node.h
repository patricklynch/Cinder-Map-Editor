#pragma once

#include <map>

#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "cinder/gl/Texture.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"

namespace ly {

class PostFilter;
class Renderer;
	
typedef enum {
	MaterialDiffuse,
	MaterialNormal,
	MaterialSpecular,
	MaterialSelfIlllum,
} MaterialProperty;

class Node {
public:
	Node();
	virtual ~Node();
	
	void update( const float deltaTime );
	void draw();
	
	ci::Vec3f position;
	ci::Vec3f rotation;
	ci::Vec3f scale;
	ci::Vec3f size;
	ci::Vec3f anchor;
	ci::Vec2i textureLoc;
	float spriteRotation;
	
	ci::gl::VboMesh* mVboMesh;
	ci::TriMesh* mMesh;
	
	std::map<MaterialProperty, ci::gl::Texture*> textures;
	std::map<MaterialProperty, ci::ColorA> colors;
	
	Node* parent;
	
	ci::Matrix44f transform() const { return mLastTransform; }
	virtual ci::Vec3f globalPosition();
	
	/** Convenience methods making it easier to set/get the diffuse color/texture */
	void setTexture( ci::gl::Texture* texture );
	void setColor( ci::ColorA color );
	ci::gl::Texture* texture();
	ci::ColorA color();
	
	/** Used by the renderer to select a lit or unlit shader */
	bool isUnlit() const { return mIsUnlit; }
	void setIsUnlit( bool inValue ) { mIsUnlit = inValue; }
	
	/** For comparing nodes */
	bool operator==( Node* node ) const { return mNodeId == node->nodeId(); }
	int nodeId() const { return mNodeId; }
	
	void setFaceForward( bool value ) { mShouldFaceForward = value; }
	
	/** Draws with normals reversed */
	bool inverted;
	
protected:
	
	/** Performs calcualtions necessary to face to camera, mostly for sprite billboards */
	void faceCamera();
	
	ci::Vec3f mAnchoredPosition;
	bool mShouldFaceForward;
	ly::Renderer* mRenderer;
	bool mIsUnlit;
	int mNodeId;
	ci::Matrix44f mLastTransform;
	ci::Matrix44f mTransform;
	
};

}