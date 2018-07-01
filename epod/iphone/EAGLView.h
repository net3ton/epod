#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>

enum
{
  MAX_TOUCHES_COUNT = 3,
};

@interface EAGLView : UIView
{
@private
  // The pixel dimensions of the backbuffer
  GLint backingWidth;
  GLint backingHeight;

  EAGLContext *context;

  // OpenGL names for the renderbuffer and framebuffers used to render to this view
  GLuint viewRenderbuffer, viewFramebuffer;

  // Touches
  UITouch *curTouches[MAX_TOUCHES_COUNT];
}

- (void)drawView;

@end
