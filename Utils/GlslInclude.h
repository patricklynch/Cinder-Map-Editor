#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/GlslProg.h"

#include <string>

namespace ly {

class GlslInclude {
private:
	static std::string loadFileToString( const std::string filepath )
	{
		ci::DataSourceRef data = ci::app::loadAsset( filepath );
		ci::Buffer& buffer = data->getBuffer();
		const char* text = (const char*) buffer.getData();
		std::string output = std::string( text );
		return output.substr( 0, buffer.getDataSize() );
	}
	
public:
	static ci::gl::GlslProg compileShader( const std::string pathVert, const std::string pathFrag )
	{
		ci::gl::GlslProg output;
		try {
			output = ci::gl::GlslProg( parse( pathVert ).c_str(), parse( pathFrag ).c_str() );
		} catch ( ci::gl::GlslProgCompileExc& exc ) {
			ci::app::console() << "GlslInclude :: Failed to compile shader: " << exc.what() << std::endl;
			exit(1);
		} catch ( ci::Exception& exc ) {
			ci::app::console() << "GlslInclude :: Failed to load shader: " << exc.what() << std::endl;
			exit(1);
		}
		return output;
	}
	
	static std::string parse( const std::string shaderFilepath )
	{
		std::string string = loadFileToString( shaderFilepath );
		int inc = 0;
		while( inc != std::string::npos ) {
			inc = string.find( "#include" );
			if ( inc != std::string::npos ) {
				int q1 = string.find( "\"", inc+1 );
				int q2 = string.find( "\"", q1+1 );
				std::string filename = string.substr( q1+1, q2-1-q1 );
				std::string fileString = loadFileToString( filename );
				string = string.replace( inc, q2-inc+1, fileString );
			}
		}
		return string;
	}
};

}