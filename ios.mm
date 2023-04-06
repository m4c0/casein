@import MetalKit;
@import UIKit;

#include "CASView.h"
#include "externc.h"

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property (nonatomic, strong) UIWindow * window;
@end

@implementation CASAppDelegate
- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {

  UIViewController * vc = [UIViewController new];
  vc.view = [CASView new];

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
