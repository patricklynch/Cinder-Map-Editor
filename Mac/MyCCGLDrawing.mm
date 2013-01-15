#import "MyCCGLDrawing.h"
#import "MyController.h"

@implementation MyCCGLDrawing

- (void) setup
{
	[super setup];
	
	mGame = ly::Game::create( self.frame.size.width, self.frame.size.height, 30 );
	mEditor = new ly::EditorMode( mGame );
	
	mUpdateRate = 1.0f / 60.0f;
	mLastDate = [[NSDate date] retain];
	[NSTimer scheduledTimerWithTimeInterval:mUpdateRate target:self selector:@selector(update) userInfo:nil repeats:YES];
	
	[controller setEditor:mEditor];
}


-(void) update
{
	NSDate* date = [NSDate date];
    float deltaTime = (float)[date timeIntervalSinceDate:mLastDate];
	[mLastDate autorelease];
	mLastDate = [date retain];
	
	mGame->update( deltaTime );
	mEditor->update( deltaTime );
}

- (void) draw
{
	gl::clear( Color( 0.9f, 0.9f, 0.9f ), true );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	mGame->draw();
	mEditor->draw();
}

- (void)reshape
{
	[super reshape];
}

- (void)mouseDown:(NSEvent*)theEvent
{
	[self mouseDown:theEvent initiator:cinder::app::MouseEvent::LEFT_DOWN];
}

- (void)rightMouseDown:(NSEvent*)theEvent
{
	[self mouseDown:theEvent initiator:cinder::app::MouseEvent::RIGHT_DOWN];
}

- (void)mouseUp:(NSEvent*)theEvent
{
	[self mouseUp:theEvent initiator:cinder::app::MouseEvent::LEFT_DOWN];
}

- (void)rightMouseUp:(NSEvent*)theEvent
{
	[self mouseUp:theEvent initiator:cinder::app::MouseEvent::RIGHT_DOWN];
}

-(void) mouseDragged:(NSEvent *)theEvent
{
	[self mouseDragged:theEvent initiator:cinder::app::MouseEvent::LEFT_DOWN];
}

-(void) rightMouseDragged:(NSEvent *)theEvent
{
	[self mouseDragged:theEvent initiator:cinder::app::MouseEvent::RIGHT_DOWN];
}

-(void) mouseMoved:(NSEvent *)theEvent
{
	[self mouseMoved:theEvent initiator:cinder::app::MouseEvent::LEFT_DOWN];
}

-(void) rightMouseMoved:(NSEvent *)theEvent
{
	[self mouseMoved:theEvent initiator:cinder::app::MouseEvent::RIGHT_DOWN];
}

- (void)mouseUp:(NSEvent*)theEvent initiator:(int) initiator
{
	NSPoint curPoint		= [self convertPoint:[theEvent locationInWindow] fromView:nil];
	int y					= self.frame.size.height - curPoint.y;
	int mods				= [self prepMouseEventModifiers:theEvent];
	
	mods |= initiator;
 	mEditor->mouseUp( cinder::app::MouseEvent( initiator, curPoint.x, y, mods, 0.0f, [theEvent modifierFlags] ) );
}

- (void)mouseDown:(NSEvent*)theEvent initiator:(int) initiator
{
	NSPoint curPoint		= [self convertPoint:[theEvent locationInWindow] fromView:nil];
	int y					= self.frame.size.height - curPoint.y;
	int mods				= [self prepMouseEventModifiers:theEvent];
	
	mods |= initiator;
 	mEditor->mouseDown( cinder::app::MouseEvent( initiator, curPoint.x, y, mods, 0.0f, [theEvent modifierFlags] ) );
}

- (void)mouseDragged:(NSEvent*)theEvent initiator:(int) initiator
{
	NSPoint curPoint		= [self convertPoint:[theEvent locationInWindow] fromView:nil];
	int y					= self.frame.size.height - curPoint.y;
	int mods				= [self prepMouseEventModifiers:theEvent];
	
	mods |= initiator;
 	mEditor->mouseDrag( cinder::app::MouseEvent( initiator, curPoint.x, y, mods, 0.0f, [theEvent modifierFlags] ) );
}

- (void)mouseMoved:(NSEvent*)theEvent initiator:(int) initiator
{
	NSPoint curPoint		= [self convertPoint:[theEvent locationInWindow] fromView:nil];
	int y					= self.frame.size.height - curPoint.y;
	int mods				= [self prepMouseEventModifiers:theEvent];
	
	mods |= initiator;
 	mEditor->mouseMove( cinder::app::MouseEvent( initiator, curPoint.x, y, mods, 0.0f, [theEvent modifierFlags] ) );
}

- (void)keyDown:(NSEvent*)theEvent
{
	char c		= [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
	int code	= [theEvent keyCode];
	int mods	= [self prepKeyEventModifiers:theEvent];
	
	cinder::app::KeyEvent k = cinder::app::KeyEvent (cinder::app::KeyEvent::translateNativeKeyCode( code ), (char)c, mods, code);
	mEditor->keyDown( k );
}

- (void)keyUp:(NSEvent*)theEvent
{
	char c		= [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
	int code	= [theEvent keyCode];
	int mods	= [self prepKeyEventModifiers:theEvent];
	
	mEditor->keyDown( cinder::app::KeyEvent (cinder::app::KeyEvent::translateNativeKeyCode( code ), (char)c, mods, code) );
}

- (int)prepKeyEventModifiers:(NSEvent *)evt
{
	unsigned int result = 0;
	
	if( [evt modifierFlags] & NSShiftKeyMask ) result |= cinder::app::KeyEvent::SHIFT_DOWN;
	if( [evt modifierFlags] & NSAlternateKeyMask ) result |= cinder::app::KeyEvent::ALT_DOWN;
	if( [evt modifierFlags] & NSCommandKeyMask ) result |= cinder::app::KeyEvent::META_DOWN;
	if( [evt modifierFlags] & NSControlKeyMask ) result |= cinder::app::KeyEvent::CTRL_DOWN;
	
	return result;
}

- (int)prepMouseEventModifiers:(NSEvent *)evt
{
	unsigned int result = 0;
	if( [evt modifierFlags] & NSControlKeyMask ) result |= cinder::app::MouseEvent::CTRL_DOWN;
	if( [evt modifierFlags] & NSShiftKeyMask ) result |= cinder::app::MouseEvent::SHIFT_DOWN;
	if( [evt modifierFlags] & NSAlternateKeyMask ) result |= cinder::app::MouseEvent::ALT_DOWN;
	if( [evt modifierFlags] & NSCommandKeyMask ) result |= cinder::app::MouseEvent::META_DOWN;
	
	return result;
}

@end
