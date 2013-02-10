#include "EditorTexturePaint.h"
#include "Editor.h"
#include "EditorState.h"
#include "AssetManager.h"
#include "EditorConstants.h"

#include "cinder/app/App.h"
#include "cinder/CinderMath.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

EditorTexturePaint::EditorTexturePaint( TexturePaintState defaultState ) : mIsApplying( false )
{
	mPosition = kTexturePaintNullPosition;
	setState( defaultState );
	
	mFlattenedOutputFbo = createFbo();
	mFlattenedHistoryFbo = createFbo();
	
	AssetManager* assetManager = AssetManager::get();
	mBrushTextures.push_back( assetManager->getTexture( "brushes/rough.png" ) );
	mBrushTextures.push_back( assetManager->getTexture( "brushes/scatter.png" ) );
	mBrushTextures.push_back( assetManager->getTexture( "brushes/soft.png" ) );
	mBrushTextures.push_back( assetManager->getTexture( "brushes/stroke.png" ) );
}

EditorTexturePaint::~EditorTexturePaint()
{
	delete mFlattenedHistoryFbo;
	delete mFlattenedOutputFbo;
}

ci::gl::Fbo* EditorTexturePaint::createFbo()
{
	gl::Fbo::Format format;
	format.setWrap( GL_REPEAT, GL_REPEAT );
	gl::Fbo* fbo = new gl::Fbo( kTexturePaintMapResolution, kTexturePaintMapResolution, format );
	fbo->bindFramebuffer();
	gl::clear( ColorA( 0, 0, 0, 0 ) );
	fbo->unbindFramebuffer();
	return fbo;
}

ci::gl::Texture* EditorTexturePaint::paintMask()
{
	return &mFlattenedOutputFbo->getTexture();
}

void EditorTexturePaint::update( const float deltaTime ) {}

void EditorTexturePaint::resetPosition()
{
	mPosition = kTexturePaintNullPosition;
}

ci::gl::Fbo* EditorTexturePaint::currentLayer() const
{
	if ( mPaintLayers.size() > 0 ) {
		return *(mPaintLayers.end()-1);
	}
	return NULL;
}

void EditorTexturePaint::setTarget( ci::Vec3f position )
{
	mPosition = position;
}

void EditorTexturePaint::apply( ci::Vec3f position )
{
	if ( mPosition == kTexturePaintNullPosition ) return;
	
	if ( position != kTexturePaintNullPosition ) {
		mPosition = position;
	}
	
	ci::ColorA layerColor( mState.textureIndex == 1 ? 1 : 0,
						   mState.textureIndex == 2 ? 1 : 0,
						   mState.textureIndex == 3 ? 1 : 0,
						   mState.brushStrength );
	
	if ( !mIsApplying ) {
		mIsApplying = true;
		mPaintLayers.push_back( createFbo() );
	}
	
	ci::gl::Fbo* currentFbo = currentLayer();
	if ( currentFbo ) {
		gl::setMatricesWindow( kTexturePaintMapResolution, kTexturePaintMapResolution );
		float n = kDefaultMaxVisibleTileRadius + 0.5f;
		currentFbo->bindFramebuffer();
		gl::enableAlphaBlending( true );
		gl::color( layerColor );
		Vec2f brushPos = Vec2f( 0.5f, 0.5f ) + ( Vec2f( position.x, position.z ) / n ) * 0.5f;
		brushPos *= (float) kTexturePaintMapResolution;
		float radius = mState.brushSize / n * (float) kTexturePaintMapResolution * 0.5f;
		Rectf rect( brushPos.x - radius, brushPos.y - radius, brushPos.x + radius, brushPos.y + radius );
		gl::draw( *mBrushTextures[ mState.brushIndex ], rect );
		currentFbo->unbindFramebuffer();
	}
	
	if ( mPaintLayers.size() >= kMaxTexturePaintUndo ) {
		gl::setMatricesWindow( kTexturePaintMapResolution, kTexturePaintMapResolution );
		mFlattenedHistoryFbo->bindFramebuffer();
		gl::enableAlphaBlending();
		gl::color( 1, 1, 1, 1 );
		gl::draw( (*mPaintLayers.begin())->getTexture() );
		mFlattenedHistoryFbo->unbindFramebuffer();
		
		delete *mPaintLayers.begin();
		mPaintLayers.erase( mPaintLayers.begin() );
	}
	
	drawOutputLayers();
}

bool EditorTexturePaint::popPaintLayer()
{
	if ( mPaintLayers.size() > 0 ) {
		delete *(mPaintLayers.end()-1);
		mPaintLayers.pop_back();
		drawOutputLayers();
		return true;
	}
	return false;
}

void EditorTexturePaint::drawOutputLayers()
{
	// flatten for output as a signle texture
	gl::setMatricesWindow( kTexturePaintMapResolution, kTexturePaintMapResolution );
	mFlattenedOutputFbo->bindFramebuffer();
	gl::clear( ColorA( 0, 0, 0, 0 ) );
	gl::color( 1, 1, 1, 1 );
	
	glPushMatrix();
	glScalef( 1.0f, -1.0f, 1.0f );
	glTranslatef( 0.0f, (float)-mFlattenedHistoryFbo->getWidth(), 0.0f );
	gl::enableAlphaBlending( true );
	gl::draw( mFlattenedHistoryFbo->getTexture() );
	glPopMatrix();
	
	gl::enableAlphaBlending();
	
	for( std::vector<gl::Fbo*>::const_iterator iter = mPaintLayers.begin(); iter != mPaintLayers.end(); iter++ ) {
		glPushMatrix();
		gl::draw( (*iter)->getTexture() );
		glPopMatrix();
	}
	mFlattenedOutputFbo->unbindFramebuffer();
}

void EditorTexturePaint::editingComplete()
{
	mIsApplying = false;
}

void EditorTexturePaint::draw()
{
	glPushMatrix();
	gl::translate( Vec3f( 0, 1.05f, 0 ) );
	gl::rotate( Vec3f( 90.0f, 0, 0 ) );
	glPopMatrix();
	
	glPushMatrix();
	gl::enableAlphaBlending();
	gl::translate( mPosition + Vec3f::one() * 0.1f );
	gl::rotate( Vec3f( 90.0f, 0, 0 ) );
	gl::color( 0, 1, 0, 0.05f );
	gl::drawSolidCircle( Vec2f::zero(), mState.brushSize, 20 );
	gl::color( 0, 1, 0, 1 );
	gl::drawStrokedCircle( Vec2f::zero(), mState.brushSize, 20 );
	glPopMatrix();
}

void EditorTexturePaint::debugDraw()
{
	Rectf r = Rectf( 0, 0, 200, 200 );
	
	gl::color( 1, 1, 1, 1 );
	gl::disableAlphaBlending();
	glPushMatrix();
	glTranslatef( 300.0f, 0.0f, 0.0f );
	gl::draw( mFlattenedOutputFbo->getTexture(), r );
	gl::drawStrokedRect( r );
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( 500.0f, 0.0f, 0.0f );
	gl::draw( mFlattenedHistoryFbo->getTexture(), r );
	gl::drawStrokedRect( r );
	glPopMatrix();
}

