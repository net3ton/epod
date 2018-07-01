#import <UIKit/UIKit.h>

@class EAGLView;

@interface EPodAppDelegate : NSObject <UIApplicationDelegate, UIAccelerometerDelegate>
{
  UIWindow *window;
  EAGLView *view;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *view;

+(EPodAppDelegate*)sharedAppDelegate;

-(void)accelerometer: (UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;

@end
