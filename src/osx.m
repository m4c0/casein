#import <AppKit/AppKit.h>

static NSMenu * setup_apple_menu(NSString * title) {
  NSMenu * bar = [NSMenu new];
  NSMenuItem * app_item = [NSMenuItem new];
  NSMenu * app_menu = [NSMenu new];

  [app_menu addItem:[[NSMenuItem alloc] initWithTitle:[NSString stringWithFormat:@"Quit %@", title]
                                               action:@selector(terminate:)
                                        keyEquivalent:@"q"]];

  app_item.submenu = app_menu;
  [bar addItem:app_item];
  return bar;
}

int main(int argc, char ** argv) {
  @autoreleasepool {
    NSString * title = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
    if (title == nil) {
      title = @"App";
    }

    NSApplication * app = [NSApplication sharedApplication];
    app.mainMenu = setup_apple_menu(title);
    [app activateIgnoringOtherApps:YES];
    [app run];
    return 0;
  }
}
