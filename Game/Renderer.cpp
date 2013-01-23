#include "Renderer.h"
#include "GlslInclude.h"
#include "AssetManager.h"

#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ip/Hdr.h"

#include <sstream>
#include <vector>

using namespace ci;
using namespace ci::app;

using namespace ly;

const int TEX_UNIT_DEPTH		= 0;
const int TEX_UNIT_DIFFUSE		= 1;
const int TEX_UNIT_NORMAL		= 2;
const int TEX_UNIT_DEPTH_MAP	= 3;


ly::Renderer* ly::Renderer::sInstance = NULL;

bool ly::Renderer::debugDrawEnabled = true;
DebugDrawSettings ly::Renderer::debugDraw;

ly::Renderer* ly::Renderer::get()
{
	if ( !sInstance ) sInstance = new ly::Renderer();
	return sInstance;
}

ly::Renderer::Renderer() : mMainCamera( NULL )
{
	setup();
}

void ly::Renderer::setup()
{
	//mFogColor				= ColorA( .2, .2, .4, 1 );
	mFogColor				= ColorA( 0, 0, 0, 0 );
	
	mShadowCastingLight = NULL;
	
	mShaders[ Sprite ]		= GlslInclude::compileShader( "shaders/sprite.vert",		"shaders/sprite.frag" );
	mShaders[ SpriteUnlit ]	= GlslInclude::compileShader( "shaders/passthru_mvp.vert",	"shaders/sprite_unlit.frag" );
	mShaders[ Debug ]		= GlslInclude::compileShader( "shaders/simple.vert",		"shaders/simple.frag");
	mShaders[ Mega ]		= GlslInclude::compileShader( "shaders/megashader.vert",	"shaders/megashader.frag" );
	mShaders[ DepthMap ]	= GlslInclude::compileShader( "shaders/depthmap.vert",		"shaders/depthmap.frag");
    mShaders[ DepthMap2 ]	= GlslInclude::compileShader( "shaders/depthmap.vert",		"shaders/depthmap2.frag" );
	
	mDefaultBlack			= *AssetManager::get()->getTexture( "textures/default_black.png" );
	mDefaultGray			= *AssetManager::get()->getTexture( "textures/default_gray.png" );
	mDefaultWhite			= *AssetManager::get()->getTexture( "textures/default_white.png" );
	
	gl::Fbo::Format format;
	mSceneDepthMap			= new gl::Fbo( getWindowWidth(), getWindowHeight(), format);
	mShadowDepthMap			= createShadowDepthMap();
}

void ly::Renderer::draw()
{
	//ColorA fogColor = mFogColor * 0.3f + mFogColor * 0.7f * mLights[0]->color();
	//fogColor.a = 1.0f;
	gl::clear( mFogColor );
	
	std::vector<Node*> sprites;
	std::vector<Node*> objects;
	
	/*std::vector<Node*>::const_iterator iter;
	for( iter = mNodes.begin(); iter != mNodes.end(); iter++ ) {
		if ( (*iter)->nodeType() == NodeTypeSprite ) sprites.push_back( *iter );
		else if ( (*iter)->isObject() ) objects.push_back( *iter );
	}*/
	
	drawObjects( mNodes );
	//drawSprites( sprites );
	
	if ( debugDrawEnabled ) drawDebug();
}


ci::gl::Fbo* ly::Renderer::createDepthMap()
{
	return createDepthMap( getWindowWidth(), getWindowHeight() );
}


ci::gl::Fbo* ly::Renderer::createShadowDepthMap()
{
	return createDepthMap( kShadowMapResolution, kShadowMapResolution );
}

ci::gl::Fbo* ly::Renderer::createDepthMap( int width, int height )
{
	gl::Fbo::Format format;
	format.setColorInternalFormat( GL_R32F );
	gl::Fbo* fbo = new gl::Fbo(width, height, format);
	fbo->bindFramebuffer();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	fbo->unbindFramebuffer();
	return fbo;
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

void ly::Renderer::addLight( ly::Light* light )
{
	if ( mLights.size() >= 4 ) return;
	std::vector<ly::Light*>::iterator existing = find( mLights.begin(), mLights.end(), light );
	if ( existing != mLights.end() ) return;
	mLights.push_back( light );
	if ( mLights.size() == 1 )
		setShadowCastingLight( light );
}

void ly::Renderer::removeLight( ly::Light* light )
{
	std::vector<ly::Light*>::iterator existing = find( mLights.begin(), mLights.end(), light );
	mLights.erase( existing );
}

void ly::Renderer::setShadowCastingLight( ly::Light* light )
{
}

void ly::Renderer::update( const float deltaTime )
{
}

void ly::Renderer::drawObjects( std::vector<Node*>& nodes )
{
}

void ly::Renderer::drawObjects( std::vector<Node*>& nodes, gl::GlslProg* shader )
{
}

void ly::Renderer::drawSprites( std::vector<Node*>& nodes )
{
}

void ly::Renderer::drawDebug()
{
}

