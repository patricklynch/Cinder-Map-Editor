#include "EditorPanel.h"
#include "EditorConstants.h"
#include "Editor.h"
#include "AssetManager.h"
#include "Block.h"

#include "cinder/Area.h"

#include <boost/bind.hpp>

using namespace ly;
using namespace ci;

EditorPanel::EditorPanel( Editor* editor ) : mEditor( editor )
{
	Vec2i paramsPanelSize = Vec2i( 300, 400 );
	mParams = params::InterfaceGl( "EDITOR", paramsPanelSize );
	
	/*std::vector<std::string> toolStrings;
	toolStrings.push_back( "Selection" );
	toolStrings.push_back( "Texture" );
	toolStrings.push_back( "Elevation" );
	toolStrings.push_back( "Object" );
	mCurrentTool = EditorPaintSelection2;
	 mParams.addParam( "CURRENT TOOL", toolStrings, &mCurrentTool );*/
	
	
	mParams.addSeparator(); // elevation options
	mParams.addParam( "Elevation Range ",		&mEditor->mState.elevationRange, "max=10 min=2 step=1" );
	mParams.addParam( "Elevation Height",		&mEditor->mState.elevationHeight, "max=10 min=0 step=1" );
	
	mParams.addSeparator(); // texture paint options
	mParams.addParam( "Texture Brush Size",		&mEditor->mState.texturePaint.brushSize, "max=10 min=0 step=1" );
	mParams.addParam( "Texture",				&mEditor->mState.texturePaint.textureIndex, "max=10 min=0 step=1" );
	mParams.addParam( "Texture Brush",			&mEditor->mState.texturePaint.brushIndex, "max=10 min=0 step=1" );
	mParams.addParam( "Texture Strength",		&mEditor->mState.texturePaint.brushStrength, "max=1.0 min=0.0 step=0.01" );
	
	mParams.addSeparator(); // View options
	mParams.addParam( "Show Grid",				&mEditor->mState.showGrid );
	
	mParams.addSeparator(); // Undo/redo
	mParams.addButton( "UNDO",					boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "REDO",					boost::bind( &EditorPanel::test, this ) );
	
	mParams.addSeparator(); // File options
	mParams.addButton( "NEW",					boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "OPEN",					boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "SAVE",					boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "RESET",					boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "COPY",					boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "EXIT",					boost::bind( &EditorPanel::test, this ) );
	
	
	EditorPalette* p;
	std::vector<gl::Texture*> textures;
	AssetManager* assetManager = AssetManager::get();
	
	textures.clear();
	textures.push_back( assetManager->getTexture( "textures/grass.png" ) );
	textures.push_back( assetManager->getTexture( "textures/stone.png" ) );
	textures.push_back( assetManager->getTexture( "textures/concrete.png" ) );
	textures.push_back( assetManager->getTexture( "textures/brick.png" ) );
	
	p = new EditorPalette( textures, &mEditor->mState.texturePaint.textureIndex );
	p->mId = TEXTURE_PICKER;
	p->mDelegate = this;
	mPalettes[ TEXTURE_PICKER ] = p;
	
	textures.clear();
	textures.push_back( assetManager->getTexture( "brushes/rough.png" ) );
	textures.push_back( assetManager->getTexture( "brushes/scatter.png" ) );
	textures.push_back( assetManager->getTexture( "brushes/soft.png" ) );
	textures.push_back( assetManager->getTexture( "brushes/stroke.png" ) );
	
	p = new EditorPalette( textures, &mEditor->mState.texturePaint.brushIndex );
	p->mId = BRUSH_PICKER;
	p->mDelegate = this;
	mPalettes[ BRUSH_PICKER ] = p;
	
	textures.clear();
	
	
	int w = 100;
	int h = 100;
	ci::CameraPersp camera( w, h, 10.0f, 1.0f, 2000.0f );
	glViewport( 0, 0, w, h );
	camera.setWorldUp( -Vec3f::yAxis() );
	camera.setEyePoint( Vec3f::one() * 20.0f );
	camera.setCenterOfInterestPoint( Vec3f( -1.0f, 2.0f, 0.0f) );
	gl::Fbo fbo = gl::Fbo( w, h );
	fbo.bindFramebuffer();
	gl::clear( ColorA( 1, 0, 0, 0 ) );
	std::vector<Block*> blocks;
	int numBlockTypes = 1;
	for( int i = 0; i < numBlockTypes; i++ ) {
		Block* block = new Block();
		block->mNode->scale = Vec3f::one() * 4.0f;
		block->setMeshType( BlockMeshEdge, 180.0f );
		block->update( 0.0f );
		blocks.push_back( block );
	}
	mEditor->mGame->drawBlocks( blocks, camera );
	fbo.unbindFramebuffer();
	mBlockThumbs.push_back( new gl::Texture( fbo.getTexture() ) );
	
	for( std::vector<gl::Texture*>::iterator iter = mBlockThumbs.begin(); iter != mBlockThumbs.end(); iter++ ) {
		textures.push_back( *iter );
	}
	
	p = new EditorPalette( textures, &mEditor->mState.elevationSelection );
	p->mId = MODEL_PICKER;
	p->mDelegate = this;
	mPalettes[ MODEL_PICKER ] = p;
	
	/*mParams.addParam( "Mix Red", &mMixColorRed, "min=-1.0 max=1.0 step=0.01 keyIncr=r keyDecr=R" );
	 mParams.addParam( "Mix Green", &mMixColorGreen, "min=-1.0 max=1.0 step=0.01 keyIncr=g keyDecr=G" );
	 mParams.addParam( "Mix Blue", &mMixColorBlue, "min=-1.0 max=1.0 step=0.01 keyIncr=b keyDecr=B" );*/
	
	// Set positions of palettes
	
	int space = 110;
	int left = 16;
	mPalettes[ TEXTURE_PICKER ]->position	= Vec2i( left, paramsPanelSize.y + 50 );
	mPalettes[ BRUSH_PICKER ]->position		= Vec2i( left, mPalettes[ TEXTURE_PICKER ]->position.y + space );
	mPalettes[ MODEL_PICKER ]->position		= Vec2i( left, mPalettes[ BRUSH_PICKER ]->position.y + space );
}

EditorPanel::~EditorPanel()
{
	for( std::map<int, EditorPalette*>::iterator iter = mPalettes.begin(); iter != mPalettes.end(); iter++ ) {
		delete iter->second;
	}
	mPalettes.clear();
}

void EditorPanel::didUpdateSelection( EditorPalette* palette, int index )
{
	switch( palette->mId ) {

		case TEXTURE_PICKER:
			mEditor->mState.texturePaint.textureIndex = index;
			break;
			
		case BRUSH_PICKER:
			mEditor->mState.texturePaint.brushIndex = index;
			break;
			
	}
}

void EditorPanel::test()
{
	std::cout << "TEST" << std::endl;
}

void EditorPanel::draw()
{
	params::InterfaceGl::draw();
	
	gl::enableAlphaBlending();
	gl::disableDepthRead();
	
	for( std::map<int, EditorPalette*>::iterator iter = mPalettes.begin(); iter != mPalettes.end(); iter++ ) {
		iter->second->draw();
	}
}

void EditorPanel::update( const float deltaTime )
{
	for( std::map<int, EditorPalette*>::iterator iter = mPalettes.begin(); iter != mPalettes.end(); iter++ ) {
		iter->second->update( deltaTime );
	}
}