#include "Renderer.h"

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

ly::Renderer* ly::Renderer::sInstance = NULL;

ly::Renderer* ly::Renderer::get()
{
	if ( !sInstance ) sInstance = new ly::Renderer();
	return sInstance;
}

ly::Renderer::Renderer()
{
	mCamera = ly::Camera::get();
}

void ly::Renderer::addNode( Node* node )
{
	mNodes.push_back( node );
}

void ly::Renderer::removeNode( Node* node )
{
	std::vector<Node*>::iterator match = find( mNodes.begin(), mNodes.end(), node );
	if ( match != mNodes.end() ) mNodes.erase( match );
}

void ly::Renderer::addNodes( std::vector<Node*>& nodes )
{
	std::vector<Node*>::const_iterator iter;
	for( iter = nodes.begin(); iter != nodes.end(); iter++ )
		addNode( *iter );
}

void ly::Renderer::draw()	
{
	if ( mCamera == NULL ) return;
	
	gl::clear( Color( 0.0f, 0.0f, 0.0f ), true );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    gl::setMatrices( mCamera->cinderCamera() );
	
	glEnable( GL_LIGHTING );
	glEnable (GL_COLOR_MATERIAL );
	glEnable( GL_LIGHT0 );
	GLfloat light_position[] = { 10.0f, 100.0f, -50.0f, 0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
	
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv( GL_FRONT, GL_DIFFUSE,	mat_diffuse );
	GLfloat mat_ambient[] = { 2.0, 2.0, 2.0, 3.0 };
	glMaterialfv( GL_FRONT, GL_AMBIENT,	mat_ambient );
	
	for( std::vector<Node*>::iterator iter = mNodes.begin(); iter != mNodes.end(); iter++) {
		Node* node = *iter;
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		{
			glMultMatrixf( node->transform() );
			
			glMatrixMode( GL_TEXTURE );
			glPushMatrix();
			{
				glScalef( 1.0f, -1.0f, 1.0f );
				glTranslatef( 0.0f, -1.0f, 0.0f );
				
				gl::color( node->color() );
				if (node->texture()) node->texture()->enableAndBind();
				node->draw();
				if (node->texture()) node->texture()->unbind();
			}
			glPopMatrix();
		}
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
	}
	glDisable( GL_BLEND );
	
	glDisable( GL_LIGHTING );
	glDisable( GL_COLOR_MATERIAL );
	glDisable( GL_LIGHT0 );
	
	gl::disableDepthRead();
	
	float length = 5.0f;
	gl::color( 1, 0, 0, 1 );
	gl::drawVector( Vec3f::zero(), Vec3f::xAxis() * length );
	gl::color( 0, 1, 0, 1 );
	gl::drawVector( Vec3f::zero(), Vec3f::yAxis() * length );
	gl::color( 0, 0, 1, 1 );
	gl::drawVector( Vec3f::zero(), Vec3f::zAxis() * length );
}