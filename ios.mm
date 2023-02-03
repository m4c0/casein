@import MetalKit;
@import UIKit;

#include "externc.h"

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@end

@implementation CASAppDelegate
- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
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
