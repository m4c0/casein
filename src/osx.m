#import <AppKit/AppKit.h>

int main(int argc, char ** argv) {
  @autoreleasepool {
    NSString * title = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
    if (!title) {
      title = @"App";
    }

    NSApplication * app = [NSApplication sharedApplication];
    [app activateIgnoringOtherApps:YES];
    [app run];
    return 0;
  }
}
