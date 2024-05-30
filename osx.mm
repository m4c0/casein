@import AppKit;
#import "CASView.h"

@interface CASAppDelegate : NSObject<NSApplicationDelegate>
@end

@implementation CASAppDelegate
- (void)applicationWillTerminate:(NSApplication *)app {
  casein_call(casein::QUIT);
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
  return YES;
}
@end

@interface CASWindow : NSWindow
@end

extern casein::point * casein_mouse_pos;
extern casein::point * casein_mouse_rel;
extern bool * casein_keydown_repeating;

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
    case 13:
      return casein::K_ENTER;
    case 27:
      return casein::K_ESCAPE;
    default:
      if (c >= 32 && c <= 127) return static_cast<casein::keys>(c);
    }
  }
  return casein::K_NULL;
}
- (void)keyDown:(NSEvent *)event {
  *casein_keydown_repeating = event.ARepeat;
  casein_call_k(casein::KEY_DOWN, [self codeForEvent:event]);
}
- (void)keyUp:(NSEvent *)event {
  casein_call_k(casein::KEY_UP, [self codeForEvent:event]);
}
- (NSPoint)translateMousePosition:(NSEvent *)event {
  NSPoint p = [self.contentView convertPoint:event.locationInWindow fromView:nil];
  p.y = self.contentView.frame.size.height - p.y;
  return p;
}

- (void)mouseDown:(NSEvent *)event {
  [self mouseMoved:event];
  casein_call_m(casein::MOUSE_DOWN, casein::M_LEFT);
}
- (void)mouseDragged:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)mouseMoved:(NSEvent *)event {
  NSPoint p = [self translateMousePosition:event];
  casein_mouse_pos->x = p.x;
  casein_mouse_pos->y = p.y;
  casein_mouse_rel->x = event.deltaX;
  casein_mouse_rel->y = event.deltaY;
  casein_call(casein::MOUSE_MOVE);
  casein_call(casein::MOUSE_MOVE_REL);
}
- (void)mouseUp:(NSEvent *)event {
  [self mouseMoved:event];
  casein_call_m(casein::MOUSE_UP, casein::M_LEFT);
}

- (void)rightMouseDown:(NSEvent *)event {
  [self mouseMoved:event];
  casein_call_m(casein::MOUSE_DOWN, casein::M_RIGHT);
}
- (void)rightMouseDragged:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)rightMouseMoved:(NSEvent *)event {
  [self mouseMoved:event];
}
- (void)rightMouseUp:(NSEvent *)event {
  [self mouseMoved:event];
  casein_call_m(casein::MOUSE_UP, casein::M_RIGHT);
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

static NSWindow * g_window;
extern "C" void casein_set_title(const char * title) {
  // TODO: this is not working
  // TODO: check if we can rename on apple menu as well
  g_window.title = [NSString stringWithUTF8String:title];
}
extern "C" void casein_enter_fullscreen() {
  if (g_window.styleMask & NSWindowStyleMaskFullScreen) return;
  [g_window toggleFullScreen:nil];
}
extern "C" void casein_leave_fullscreen() {
  if (g_window.styleMask & NSWindowStyleMaskFullScreen) [g_window toggleFullScreen:nil];
}
static NSWindow * create_key_window(NSString * title) {
  NSWindow * wnd = g_window = [CASWindow new];
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

int exit_code { 0 };
extern "C" void casein_exit(int code) {
  exit_code = code;
  [NSApp terminate:nil];
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
    return exit_code;
  }
}
