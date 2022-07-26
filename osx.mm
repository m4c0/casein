#include "casein.hpp"

#import <AppKit/AppKit.h>

@interface CASAppDelegate : NSObject<NSApplicationDelegate>
- (void)redraw:(NSTimer *)sender;
@end

@implementation CASAppDelegate
- (void)applicationWillTerminate:(NSApplication *)app {
  casein_event(casein::events::quit {});
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
  return YES;
}
- (void)redraw:(NSTimer *)sender {
  casein_event(casein::events::repaint {});
}
@end

@interface CASWindow : NSWindow
@end

@implementation CASWindow
- (void)keyDown:(NSEvent *)event {
  if (!event.ARepeat) casein_event(casein::events::key_down { 0 });
}
- (void)keyUp:(NSEvent *)event {
  casein_event(casein::events::key_up { 0 });
}
- (void)mouseDown:(NSEvent *)event {
  casein_event(casein::events::mouse_down { static_cast<int>(event.buttonNumber) });
}
- (void)mouseDragged:(NSEvent *)event {
  [self mouseMoved:event]; 
}
- (void)mouseMoved:(NSEvent *)event {
  int lx = static_cast<int>(event.locationInWindow.x);
  int ly = static_cast<int>(self.frame.size.height - event.locationInWindow.y);
  casein_event(casein::events::mouse_move { lx, ly });
}
- (void)mouseUp:(NSEvent *)event {
  casein_event(casein::events::mouse_up { static_cast<int>(event.buttonNumber) });
}
@end

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

static NSWindow * create_key_window(NSString * title) {
  NSWindow * wnd = [CASWindow new];
  wnd.acceptsMouseMovedEvents = true;
  wnd.collectionBehavior = NSWindowCollectionBehaviorFullScreenPrimary;
  wnd.contentView = [NSView new];
  wnd.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable
                | NSWindowStyleMaskResizable;
  wnd.title = title;

  [wnd setFrame:NSMakeRect(0, 0, 800, 600) display:YES];
  [wnd center];
  [wnd makeKeyAndOrderFront:wnd];

  casein_event(casein::events::create_window { (__bridge void *)wnd });
  return wnd;
}

static void setupTimer(CASAppDelegate * a) {
  [NSTimer scheduledTimerWithTimeInterval:1.0 / 30.0 target:a selector:@selector(redraw:) userInfo:nil repeats:YES];
}

int main(int argc, char ** argv) {
  @autoreleasepool {
    NSString * title = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
    if (title == nil) {
      title = @"App";
    }

    CASAppDelegate * app_d = [CASAppDelegate new];

    NSApplication * app = [NSApplication sharedApplication];
    app.delegate = app_d;
    app.mainMenu = setup_apple_menu(title);
    create_key_window(title);
    setupTimer(app_d);
    [app activateIgnoringOtherApps:YES];
    [app run];
    return 0;
  }
}
