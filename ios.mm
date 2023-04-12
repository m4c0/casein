@import MetalKit;
@import UIKit;

#include "CASView.h"
#include "externc.h"

@interface CASViewController : UIViewController
@end

@implementation CASViewController
- (BOOL)canBecomeFirstResponder {
  return YES;
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
  casein_gesture(G_SHAKE);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_mouse_down(p.x,  p.y, t.tapCount);
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_mouse_up(p.x,  p.y, t.tapCount);
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  CGPoint p = [[touches anyObject] locationInView:self.view];
  casein_mouse_move(p.x,  p.y);
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_mouse_up(p.x,  p.y, t.tapCount);
}
@end

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property (nonatomic, strong) UIWindow * window;
@end

@implementation CASAppDelegate
- (void)swipeLeft {
  casein_gesture(G_SWIPE_LEFT);
}
- (void)swipeRight {
  casein_gesture(G_SWIPE_RIGHT);
}
- (void)swipeTop {
  casein_gesture(G_SWIPE_UP);
}
- (void)swipeBottom {
  casein_gesture(G_SWIPE_DOWN);
}
- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
  app.applicationSupportsShakeToEdit = YES;

  UIViewController * vc = [CASViewController new];
  vc.view = [CASView new];

  UISwipeGestureRecognizer * left = [UISwipeGestureRecognizer new];
  left.direction = UISwipeGestureRecognizerDirectionLeft;
  [left addTarget:self action:@selector(swipeLeft)];
  [vc.view addGestureRecognizer:left];

  UISwipeGestureRecognizer * right = [UISwipeGestureRecognizer new];
  right.direction = UISwipeGestureRecognizerDirectionRight;
  [right addTarget:self action:@selector(swipeRight)];
  [vc.view addGestureRecognizer:right];

  UISwipeGestureRecognizer * top = [UISwipeGestureRecognizer new];
  top.direction = UISwipeGestureRecognizerDirectionUp;
  [top addTarget:self action:@selector(swipeTop)];
  [vc.view addGestureRecognizer:top];

  UISwipeGestureRecognizer * bottom = [UISwipeGestureRecognizer new];
  bottom.direction = UISwipeGestureRecognizerDirectionDown;
  [bottom addTarget:self action:@selector(swipeBottom)];
  [vc.view addGestureRecognizer:bottom];

  self.window = [UIWindow new];
  self.window.frame = [UIScreen mainScreen].bounds;
  self.window.rootViewController = vc;
  [self.window makeKeyAndVisible];
  return YES;
}
- (void)applicationWillTerminate:(UIApplication *)app {
  casein_quit();
}
@end

int main(int argc, char ** argv) {
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, @"CASAppDelegate");
  }
}
