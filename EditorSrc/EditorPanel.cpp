#include "EditorPanel.h"
#include "EditorConstants.h"

#include <boost/bind.hpp>

using namespace ly;
using namespace ci;

EditorPanel::EditorPanel()
{
	mParams = params::InterfaceGl( "EDITOR", Vec2i( kParamsWidth, kParamsHeight ) );
	mParams.addSeparator();
	std::vector<std::string> toolStrings;
	toolStrings.push_back( "Selection" );
	toolStrings.push_back( "Texture" );
	toolStrings.push_back( "Elevation" );
	toolStrings.push_back( "Object" );
	mCurrentTool = EditorPaintSelection2;
	mParams.addParam( "CURRENT TOOL", toolStrings, &mCurrentTool );
	mParams.addSeparator();
	mTextureIndex = 0;
	mParams.addParam( "TEXTURE INDEX", &mTextureIndex, "", true );
	mElevation = 0.0f;
	mParams.addParam( "ELEVATION", &mElevation, "max=1.0 min=-1.0 step=0.01" );
	mParams.addSeparator();
	mParams.addButton( "UNDO", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "REDO", boost::bind( &EditorPanel::test, this ) );
	mParams.addSeparator();
	mParams.addButton( "NEW", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "OPEN", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "SAVE", boost::bind( &EditorPanel::test, this ) );
	mParams.addButton( "EXIT", boost::bind( &EditorPanel::test, this ) );
	
	/*mParams.addParam( "Mix Red", &mMixColorRed, "min=-1.0 max=1.0 step=0.01 keyIncr=r keyDecr=R" );
	 mParams.addParam( "Mix Green", &mMixColorGreen, "min=-1.0 max=1.0 step=0.01 keyIncr=g keyDecr=G" );
	 mParams.addParam( "Mix Blue", &mMixColorBlue, "min=-1.0 max=1.0 step=0.01 keyIncr=b keyDecr=B" );*/
}

EditorPanel::~EditorPanel() {}


void EditorPanel::test()
{
	std::cout << "TEST" << std::endl;
}

void EditorPanel::draw()
{
	params::InterfaceGl::draw();
}

void EditorPanel::update( const float deltaTime )
{
	
}