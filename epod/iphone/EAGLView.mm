#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/glext.h>

#import "EAGLView.h"

#include <epod.h>


@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;

- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;

@end


/////////////////////////////////////////////////////////////////////////////////////////


@implementation EAGLView

@synthesize context;


// You must implement this
+ (Class)layerClass
{
  return [CAEAGLLayer class];
}


- (id)initWithFrame:(CGRect)frame
{
  self = [super initWithFrame:frame];

  for (int i=0; i<MAX_TOUCHES_COUNT; ++i)
    curTouches[i] = NULL;

  if (self) 
  {
    self.opaque = YES;
    self.multipleTouchEnabled = YES;

    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    //if (os_is_retina_display())
    //  eaglLayer.contentsScale = 2;
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
      [NSNumber numberWithBool: NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

    context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES1];

    if (!context || ![EAGLContext setCurrentContext:context]) 
    {
      [self release];
      return nil;
    }
  }

  if (!ep::init_epod())
  {
    NSLog(@"ERROR: Couldn't create/init epod device!");
    [self release];
    return nil;
  }

  ep::get_root_scene()->init();

  return self;
}


- (void)drawView
{
  if (!viewRenderbuffer)
    return;

  static int timeLast = ep::get_time();

  [EAGLContext setCurrentContext:context];

  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);

  int time = ep::get_time();
  ep::IScene *root = ep::get_root_scene();
  root->update(time - timeLast);
  root->render(0);

  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [context presentRenderbuffer:GL_RENDERBUFFER_OES];

  timeLast = time;
}


- (void)layoutSubviews
{
  [EAGLContext setCurrentContext:context];
  [self destroyFramebuffer];
  [self createFramebuffer];
  [self drawView];
}


- (BOOL)createFramebuffer
{
  glGenFramebuffersOES(1, &viewFramebuffer);
  glGenRenderbuffersOES(1, &viewRenderbuffer);

  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);

  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

  if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
  {
    NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    return NO;
  }

  return YES;
}


- (void)destroyFramebuffer
{
  glDeleteFramebuffersOES(1, &viewFramebuffer);
  viewFramebuffer = 0;
  glDeleteRenderbuffersOES(1, &viewRenderbuffer);
  viewRenderbuffer = 0;
}


- (void)dealloc
{
  ep::free_epod();

  if ([EAGLContext currentContext] == context)
  {
    [EAGLContext setCurrentContext:nil];
  }

  [context release];
  [super dealloc];
}


///////////////////////////////////////////////////////////////////////////////


int get_touch_ind(UITouch **touches, UITouch *touch)
{
  for (int i=0; i<MAX_TOUCHES_COUNT; ++i)
    if (touch == touches[i])
      return i;

  return -1;
}

int get_touch_empty_ind(UITouch **touches)
{
  for (int i=0; i<MAX_TOUCHES_COUNT; ++i)
    if (!touches[i])
      return i;

  return -1;
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  //ep::log("touch begin (%d)",[[event allTouches] count]);

  for (UITouch *touch in touches)
  {
    int touchInd = get_touch_empty_ind(curTouches);
    if (touchInd == -1)
      break;

    curTouches[touchInd] = touch;

    CGPoint pt =[touch locationInView:self];

    ep::IScene *root = ep::get_root_scene();
    if (root)
      root->onPointerDown(touchInd, pt.x, pt.y);
  }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  //ep::log("touch end (%d)",[[event allTouches] count]);

  for (UITouch *touch in touches)
  {
    int touchInd = get_touch_ind(curTouches, touch);
    if (touchInd == -1)
      continue;

    CGPoint pt =[touch locationInView:self];

    ep::IScene *root = ep::get_root_scene();
    if (root)
      root->onPointerUp(touchInd, pt.x, pt.y);

    curTouches[touchInd] = NULL;
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
  //ep::log("touch moved (%d)",[[event allTouches] count]);

  for (UITouch *touch in touches)
  {
    int touchInd = get_touch_ind(curTouches, touch);
    if (touchInd == -1)
      continue;

    CGPoint pt =[touch locationInView:self];

    ep::IScene *root = ep::get_root_scene();
    if (root)
      root->onPointerMove(touchInd, pt.x, pt.y);
  }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event 
{
  [self touchesEnded:touches withEvent:event];
}


@end
