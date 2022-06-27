#include "casein.hpp"

#import <UIKit/UIKit.h>

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@end

@implementation CASAppDelegate
- (void)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
  casein_event(casein::create_window { (__bridge void *)app }); // TODO: pass window? view?
}
- (void)applicationWillTerminate:(UIApplication *)app {
  casein_event(casein::quit {});
}
@end

int main(int argc, char ** argv) {
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, @"CASAppDelegate");
  }
}
