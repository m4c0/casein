@import AppKit;
@import MetalKit;

#include "CASView.h"
#include "externc.h"

@interface CASAppDelegate : NSObject<NSApplicationDelegate>
@end

@implementation CASAppDelegate
- (void)applicationWillTerminate:(NSApplication *)app {
  casein_quit();
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
  return YES;
}
@end

@interface CASWindow : NSWindow
@end

@implementation CASWindow
- (keys)codeForEvent:(NSEvent *)event {
  NSString * arrow = event.charactersIgnoringModifiers;
  switch (arrow.length) {
  case 1:
    switch (unichar c = [arrow characterAtIndex:0]) {
    case NSLeftArrowFunctionKey:
      return K_LEFT;
    case NSRightArrowFunctionKey:
      return K_RIGHT;
    case NSUpArrowFunctionKey:
      return K_UP;
    case NSDownArrowFunctionKey:
      return K_DOWN;
    default:
      if (c >= 32 && c <= 127) return K_SPACE;
    }
  }
  return K_NULL;
}
- (void)keyDown:(NSEvent *)event {
  if (event.ARepeat) return;

  casein_key_down([self codeForEvent:event]);
}
- (void)keyUp:(NSEvent *)event {
  casein_key_up([self codeForEvent:event]);
}
- (NSPoint)translateMousePosition:(NSEvent *)event {
  NSPoint p = [self.contentView convertPoint:event.locationInWindow fromView:nil];
  p.y = self.contentView.frame.size.height - p.y;
  return p;
}
- (void)mouseDown:(NSEvent *)event {
  NSPoint p = [self translateMousePosition:event];
  casein_mouse_down(static_cast<int>(p.x), static_cast<int>(p.y), static_cast<int>(event.buttonNumber));
}
- (void)mouseDragged:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)mouseMoved:(NSEvent *)event {
  NSPoint p = [self translateMousePosition:event];
  casein_mouse_move(static_cast<int>(p.x), static_cast<int>(p.y));
}
- (void)mouseUp:(NSEvent *)event {
  NSPoint p = [self translateMousePosition:event];
  casein_mouse_up(static_cast<int>(p.x), static_cast<int>(p.y), static_cast<int>(event.buttonNumber));
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
  wnd.contentView = [CASView new];
  wnd.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable
                | NSWindowStyleMaskResizable;
  wnd.title = title;

  [wnd setFrame:NSMakeRect(0, 0, 800, 600) display:YES];
  [wnd center];
  [wnd makeKeyAndOrderFront:wnd];
  return wnd;
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
    [app activateIgnoringOtherApps:YES];
    [app run];
    return 0;
  }
}
