@import AppKit;
#import "CASView.h"

@interface CASAppDelegate : NSObject<NSApplicationDelegate>
@end

@implementation CASAppDelegate
- (void)applicationWillTerminate:(NSApplication *)app {
  casein_handle(casein::events::quit {});
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
  return YES;
}
@end

@interface CASWindow : NSWindow
@end

@implementation CASWindow
- (casein::keys)codeForEvent:(NSEvent *)event {
  NSString * arrow = event.charactersIgnoringModifiers;
  switch (arrow.length) {
  case 1:
    switch (unichar c = [arrow characterAtIndex:0]) {
    case NSLeftArrowFunctionKey:
      return casein::K_LEFT;
    case NSRightArrowFunctionKey:
      return casein::K_RIGHT;
    case NSUpArrowFunctionKey:
      return casein::K_UP;
    case NSDownArrowFunctionKey:
      return casein::K_DOWN;
    default:
      if (c >= 32 && c <= 127) return static_cast<casein::keys>(c);
    }
  }
  return casein::K_NULL;
}
- (void)keyDown:(NSEvent *)event {
  if (event.ARepeat) return;

  casein_handle(casein::events::key_down { [self codeForEvent:event] });
}
- (void)keyUp:(NSEvent *)event {
  casein_handle(casein::events::key_up { [self codeForEvent:event] });
}
- (NSPoint)translateMousePosition:(NSEvent *)event {
  NSPoint p = [self.contentView convertPoint:event.locationInWindow fromView:nil];
  p.y = self.contentView.frame.size.height - p.y;
  return p;
}

- (void)mouseDown:(NSEvent *)event {
  [self mouseMoved:event];
  casein_handle(casein::events::mouse_down { casein::M_LEFT });
}
- (void)mouseDragged:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)mouseMoved:(NSEvent *)event {
  NSPoint p = [self translateMousePosition:event];
  casein_handle(casein::events::mouse_move { { static_cast<int>(p.x), static_cast<int>(p.y) } });
  casein_handle(casein::events::mouse_move_rel { { static_cast<int>(event.deltaX), static_cast<int>(event.deltaY) } });
}
- (void)mouseUp:(NSEvent *)event {
  [self mouseMoved:event];
  casein_handle(casein::events::mouse_up { casein::M_LEFT });
}

- (void)rightMouseDown:(NSEvent *)event {
  [self mouseMoved:event];
  casein_handle(casein::events::mouse_down { casein::M_RIGHT });
}
- (void)rightMouseDragged:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)rightMouseMoved:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)rightMouseUp:(NSEvent *)event {
  [self mouseMoved:event];
  casein_handle(casein::events::mouse_up { casein::M_RIGHT });
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

  int w = casein::base_width;
  int h = casein::base_height;
  [wnd setFrame:NSMakeRect(0, 0, w, h) display:YES];
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
