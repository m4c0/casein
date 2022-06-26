#import <UIKit/UIKit.h>

@interface CASAppDelegate : NSObject<UIAppDelegate>
@end

@implementation CASAppDelegate
- (void)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
}
- (void)applicationWillTerminate:(UIApplication *)app {
}
@end

int main(int argc, char ** argv) {
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, @"CASAppDelegate");
  }
}
