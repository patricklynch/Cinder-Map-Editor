#include "Node.h"
#include "Camera.h"

using namespace ly;

using namespace ci;

static int nodeIdCounter = 0;

Node::Node( NodeType type ) : parent( NULL ), mIsUnlit( false ), mShouldFaceForward( false ), inverted( false ), mTextureUnitSize( 1.0f )
{
	mNodeType = type;
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
	switch( mNodeType )
	{
		case NodeTypeSphere:
			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );
			gl::drawSphere( Vec3f::zero(), size.x );
			glDisable( GL_CULL_FACE );
			break;
			
		case NodeTypeCube: {
			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );
			glMatrixMode( GL_TEXTURE );
			glPushMatrix();
			glScalef( mTextureUnitSize, mTextureUnitSize, 1.0f );
			glTranslatef( textureLoc.x, textureLoc.y, 0.0f );
			gl::drawCube( Vec3f::zero() + mAnchoredPosition, size * Vec3f::one() );
			glPopMatrix();
			glDisable( GL_CULL_FACE );
		}
			break;
			
		case NodeTypeSprite:
		case NodeTypePlane: {
			float x = (0.0f - anchor.x) * size.x;
			float y = (0.0f - anchor.y) * size.y;
			float w = size.x;
			float h = size.y;
			gl::drawSolidRect( Rectf( x, y, x+w, y+h ) );
		}
			break;
			
		default:break;
	}
}

void Node::faceForwardTarget()
{
	Vec3f forward = mForwardTarget - globalPosition();
	mTransform *= ci::Matrix44f::alignZAxisWithTarget( forward, ci::Vec3f::yAxis() );
	mTransform.rotate( Vec3f::zAxis(), toRadians( spriteRotation ) );
	mLastTransform = mTransform;
}

void Node::update( const float deltaTime )
{
	if ( mNodeType == NodeTypeSprite ) {
		mShouldFaceForward = true;
		mForwardTarget = ly::Camera::get()->globalPosition();
	}
	
	float min = 0.01f;
	if ( size.x <= min ) size.x = min;
	if ( size.y <= min ) size.y = min;
	if ( size.z <= min ) size.z = min;
	
	mTransform = Matrix44f::identity();
	if ( parent != NULL )
		mTransform = parent->transform();
	mTransform.translate( position );
	mTransform.rotate( rotation * M_PI / 180.0f );
	mTransform.scale( scale );
	mAnchoredPosition = ( Vec3f( anchor.x, 1.0f-anchor.y, anchor.z ) - Vec3f( 0.5f, 0.5f, 0.5f ) ) * size;
	mLastTransform = mTransform;
	
	if ( mShouldFaceForward ) faceForwardTarget();
}

void Node::setTexture( gl::Texture* texture )
{
	textures[ MaterialDiffuse ] = texture;
	mTextureUnitSize = (float) kTileTextureSize / textures[ MaterialDiffuse ]->getWidth();
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