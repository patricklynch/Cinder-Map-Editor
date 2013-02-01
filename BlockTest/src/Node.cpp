#include "Node.h"
#include "Camera.h"

using namespace ly;

using namespace ci;

static int nodeIdCounter = 0;

Node::Node() : parent( NULL ), mMesh( NULL ), mVboMesh( NULL ), mIsUnlit( false ), mShouldFaceForward( false ), inverted( false )
{
	mNodeId = nodeIdCounter++;
	mTransform = mLastTransform = Matrix44f::identity();
	scale = Vec3f::one();
	
	size = Vec3f::one();
	
	// Set default colors
	colors[ MaterialDiffuse ]  = ColorA::white();
	colors[ MaterialSpecular ]  = ColorA::white();
	
	anchor = Vec3f( 0.5f, 0.5f, 0.5f);
	
	textureLoc.set( 0, 0 );
}

Node::~Node() {}

ci::Vec3f Node::globalPosition()
{
	return Vec3f( transform().getTranslate().xyz() );
}

void Node::draw()
{
	if ( mMesh ) gl::draw( *mMesh );
}

void Node::faceCamera()
{
	Vec3f target = ly::Camera::get()->globalPosition();
	Vec3f forward = target - globalPosition();
	mTransform *= ci::Matrix44f::alignZAxisWithTarget( forward, ci::Vec3f::yAxis() );
	mTransform.rotate( Vec3f::zAxis(), toRadians( spriteRotation ) );
	mLastTransform = mTransform;
}

void Node::update( const float deltaTime )
{
	float min = 0.01f;
	if ( size.x <= min ) size.x = min;
	if ( size.y <= min ) size.y = min;
	if ( size.z <= min ) size.z = min;
	
	if ( parent != NULL ) {
		mTransform = parent->transform();
	} else {
		mTransform = Matrix44f::identity();
	}
	mTransform.translate( position );
	mTransform.rotate( rotation * M_PI / 180.0f );
	mTransform.scale( scale );
	//mAnchoredPosition = ( Vec3f( anchor.x, 1.0f-anchor.y, anchor.z ) - Vec3f( 0.5f, 0.5f, 0.5f ) ) * size;
	mLastTransform = mTransform;
	
	if ( mShouldFaceForward ) faceCamera();
}

void Node::setTexture( gl::Texture* texture )
{
	textures[ MaterialDiffuse ] = texture;
}

ci::gl::Texture* Node::texture()
{
	return textures[ MaterialDiffuse ];
}

void Node::setColor( ci::ColorA color )
{
	colors[ MaterialDiffuse ] = color;
}

ci::ColorA Node::color()
{
	return colors[ MaterialDiffuse ];
}