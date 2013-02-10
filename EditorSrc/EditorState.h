#pragma once

#include "EditorTexturePaint.h"

namespace ly {

typedef enum {
	MODE_NONE = -1,
	MODE_SELECTION_OBJECT,
	MODE_PAINT_ELEVATION,
	MODE_PAINT_TEXTURE,
	MODE_COUNT
} EditorMode_t;
	
typedef enum {
	SELECTION_NONE = 1,
	SELECTION_POINT,
	SELECTION_FACE,
	SELECTION_COUNT
} SelectionMode_t;

typedef enum {
	PAINT_NONE,
	PAINT_SPECIFIC,
	PAINT_STACK,
	PAINT_ADDITIVE,
	PAINT_COUNT
} ElevationPaintMode_t;

template <class T>
class EditorValue {
public:
	EditorValue() {}
	EditorValue( T value, T min, T max ) : mValue( mValue ), mMin( min ), mMax( max ) {}
	EditorValue( const EditorValue<T>& rhs ) : mValue( rhs ) {}
	operator const T&() const { return mValue; }
	const T&	operator()() const { return mValue; }
	T&			operator()() { return mValue; }
	
	EditorValue<T>& operator=( const EditorValue& rhs ) { mValue = clamp( rhs.mValue ); return *this; }
	EditorValue<T>& operator=( const T& rhs ) { mValue = clamp( rhs ); return *this; }
	bool			operator==( const T& value ) { return mValue == value; }
	bool			operator!=( const T& value ) { return mValue != value; }
	bool			operator==( const EditorValue& eValue ) { return eValue.mValue == mValue; }
	bool			operator!=( const EditorValue& eValue ) { return eValue.mValue != mValue; }
	
	void		operator++() { mValue = clamp( mValue + (T)1 );	}
	void		operator--() { mValue = clamp( mValue - (T)1 );	}
	
	const T		clamp( const T& v) const { return v < mMin ? mMin : v > mMax ? mMax : v; }
	
private:
	T			mValue;
	T			mMax;
	T			mMin;
};

class EditorState {
public:
	EditorState();
	
	//EditorValue<float> testFloat;
	//EditorValue<EditorMode_t> testType;
	//EditorValue<int> testInt;
	
	void					resetDefaults();
	void					modifySelection( int amount );
	void					modifyValue( float amount );
	
	int						elevationRange;
	int						elevationHeight;
	int						elevationSelection;
	TexturePaintState		texturePaint;
	bool					showGrid;
	EditorMode_t			mode;
};

}