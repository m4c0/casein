@import MetalKit;
@import UIKit;

#include "externc.h"

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property (nonatomic, strong) UIWindow * window;
@end

@implementation CASAppDelegate
- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
  MTKView * v = [MTKView new];
  // TODO: Add delegate

  UIViewController * vc = [UIViewController new];
  vc.view = v;

  self.window = [UIWindow new];
  self.window.frame = [UIScreen mainScreen].bounds;
  self.window.rootViewController = vc;
  [self.window makeKeyAndVisible];

  casein_create_window((__bridge void *)app); // TODO: pass CAMetalLayer
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
