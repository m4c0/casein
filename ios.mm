@import MetalKit;
@import UIKit;

#include "CASView.h"
#include "externc.h"

@interface CASViewController : UIViewController
@end

@implementation CASViewController
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_mouse_down(p.x, [self view].frame.size.height - p.y, t.tapCount);
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_mouse_up(p.x, self.view.frame.size.height - p.y, t.tapCount);
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  CGPoint p = [[touches anyObject] locationInView:self.view];
  casein_mouse_move(p.x, self.view.frame.size.height - p.y);
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_mouse_up(p.x, self.view.frame.size.height - p.y, t.tapCount);
}
@end

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property (nonatomic, strong) UIWindow * window;
@end

@implementation CASAppDelegate
- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {

  UIViewController * vc = [CASViewController new];
  vc.view = [UIView new];

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
