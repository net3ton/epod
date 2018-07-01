#import <QuartzCore/CADisplayLink.h>
#import "epod_delegate.h"
#import "EAGLView.h"

#include <epod.h>


@implementation EPodAppDelegate

@synthesize window;
@synthesize view;


+(EPodAppDelegate*)sharedAppDelegate
{
  return (EPodAppDelegate*)[[UIApplication sharedApplication] delegate];
}


UIInterfaceOrientation get_iface_orientation(int orient)
{
  if (orient == ep::SCREEN_ORIENT_LANDSCAPE_RIGHT)
    return UIInterfaceOrientationLandscapeRight;
  if (orient == ep::SCREEN_ORIENT_LANDSCAPE_LEFT)
    return UIInterfaceOrientationLandscapeLeft;
  if (orient == ep::SCREEN_ORIENT_PORTRAIT_DOWN)
    return UIInterfaceOrientationPortraitUpsideDown;

  return UIInterfaceOrientationPortrait;
}


//-(void)mainLoop
//{
//  while(1)
//  {
//    while(CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, TRUE) == kCFRunLoopRunHandledSource);
//
//    [view drawView];
//  }
//}

-(void)frameFunc
{
  [view drawView];
}


-(void)applicationDidFinishLaunching:(UIApplication *)application
{
  [UIApplication sharedApplication].statusBarHidden = YES;

  UIInterfaceOrientation orient = ::get_iface_orientation(ep::get_screen_orientation());
  [[UIApplication sharedApplication] setStatusBarOrientation: orient animated: NO];

  [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationDidChange)
    name:@"UIDeviceOrientationDidChangeNotification" object:nil];

  ////

  window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  //window.backgroundColor = [UIColor whiteColor];

  // Override point for customization after application launch  
  [window makeKeyAndVisible];

  view = [[EAGLView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];

  //if (os_is_retina_display())
  //  view.contentScaleFactor = 2.0f;

  [window addSubview: view];

  //[NSTimer scheduledTimerWithTimeInterval:(0) target:self selector:@selector(mainLoop) userInfo:nil repeats:NO];

  CADisplayLink *displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(frameFunc)];
  [displayLink setFrameInterval:1];
  [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}


int get_ep_orientation(UIDeviceOrientation orient)
{
  if (orient == UIDeviceOrientationLandscapeRight)
    return ep::SCREEN_ORIENT_LANDSCAPE_RIGHT;
  if (orient == UIDeviceOrientationLandscapeLeft)
    return ep::SCREEN_ORIENT_LANDSCAPE_LEFT;
  if (orient == UIDeviceOrientationPortrait)
    return ep::SCREEN_ORIENT_PORTRAIT_TOP;
  if (orient == UIDeviceOrientationPortraitUpsideDown)
    return ep::SCREEN_ORIENT_PORTRAIT_DOWN;

  return ep::SCREEN_ORIENT_INVALID;
}


-(void)orientationDidChange
{
  UIDeviceOrientation orient = [[UIDevice currentDevice] orientation];
  ep::set_screen_orientation(::get_ep_orientation(orient));
}


-(void)applicationWillResignActive:(UIApplication *)application
{
  ep::IScene *root = ep::get_root_scene();
  if (root)
    root->onPause();
}


-(void)applicationDidBecomeActive:(UIApplication *)application
{
  ep::IScene *root = ep::get_root_scene();
  if (root)
    root->onResume();
}


-(void)accelerometer: (UIAccelerometer *)accelerometer didAccelerate: (UIAcceleration *)acceleration
{
  // acceleration.x;
  // acceleration.y;
  // acceleration.z;
}


- (void)dealloc
{
  [window release];
  [view release];
  [super dealloc];
}

@end
