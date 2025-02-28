@import AppKit;
#import "CASView.h"
#import "externc.hpp"

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
    case NSLeftArrowFunctionKey: return casein::K_LEFT;
    case NSRightArrowFunctionKey: return casein::K_RIGHT;
    case NSUpArrowFunctionKey: return casein::K_UP;
    case NSDownArrowFunctionKey: return casein::K_DOWN;
    case 13: return casein::K_ENTER;
    case 27: return casein::K_ESCAPE;
    default:
      if (c >= 32 && c <= 127) return static_cast<casein::keys>(c);
    }
    break;
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

- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender {
  NSDragOperation mask = [sender draggingSourceOperationMask];
  NSPasteboard * pboard = [sender draggingPasteboard];

  if ([[pboard types] containsObject:NSPasteboardTypeFileURL]) {
    // "Copy" gives a better visual feedback
    if (mask & NSDragOperationCopy) {
      return NSDragOperationCopy;
    }
  }
  return NSDragOperationNone;
}
- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender {
  NSDragOperation mask = [sender draggingSourceOperationMask];
  NSPasteboard * pboard = [sender draggingPasteboard];

  if ([[pboard types] containsObject:NSPasteboardTypeFileURL]) {
    if (mask & NSDragOperationCopy) {
      NSArray * files = [pboard readObjectsForClasses:@[ [NSURL class] ] options:@{}];

      casein_clear_drops();
      for (NSURL * url in files) {
        NSString * str = [url path];
        casein_add_drop([str UTF8String], [str length]);
      }
      casein_call(casein::FILES_DROP);
      return YES;
    }
  }
  return NO;
}
@end

static NSMenu * setup_apple_menu() {
  NSMenu * bar = [NSMenu new];
  NSMenuItem * app_item = [NSMenuItem new];
  NSMenu * app_menu = [NSMenu new];

  // TODO: remove exe name from top menu
  [app_menu addItem:[[NSMenuItem alloc] initWithTitle:[NSString stringWithFormat:@"Quit %@", @"App"]
                                               action:@selector(terminate:)
                                        keyEquivalent:@"q"]];

  app_item.submenu = app_menu;
  [bar addItem:app_item];
  return bar;
}

static NSWindow * g_window;
static bool g_drop_enabled;

extern "C" void casein_enable_filedrop(bool en) {
  g_drop_enabled = en;
  if (!g_window) return;

  if (en) {
    NSArray * types = [NSArray arrayWithObjects:NSPasteboardTypeFileURL, nil];
    [g_window registerForDraggedTypes:types];
  } else {
    [g_window unregisterDraggedTypes];
  }
}

static void set_window_title() {
  // TODO: this is not working
  // TODO: check if we can rename on apple menu as well
  const char * chars;
  unsigned len;
  casein_window_title(&chars, &len);
  g_window.title = [[NSString alloc] initWithBytes:chars length:len encoding:NSUTF8StringEncoding];
}

static void resize_window() {
  NSRect crect = [g_window contentRectForFrameRect:g_window.frame];
  crect.size = NSMakeSize(casein_window_size->x, casein_window_size->y);

  // TODO: align this shite from top-left
  NSRect frect = [g_window frameRectForContentRect:crect];
  [g_window setFrame:frect display:YES animate:YES];
}

static void enter_fullscreen() {
  if (g_window.styleMask & NSWindowStyleMaskFullScreen) return;
  [g_window toggleFullScreen:nil];
}
static void leave_fullscreen() {
  if (g_window.styleMask & NSWindowStyleMaskFullScreen) [g_window toggleFullScreen:nil];
}
static NSWindow * create_key_window() {
  NSWindow * wnd = g_window = [CASWindow new];
  wnd.acceptsMouseMovedEvents = true;
  wnd.collectionBehavior = NSWindowCollectionBehaviorFullScreenPrimary;
  wnd.contentView = [CASView new];
  wnd.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable
                | NSWindowStyleMaskResizable;
  set_window_title();

  int w = casein_window_size->x;
  int h = casein_window_size->y;
  NSRect crect = NSMakeRect(0, 0, w, h);
  NSRect frect = [g_window frameRectForContentRect:crect];
  [wnd setFrame:frect display:YES];
  [wnd center];
  [wnd makeKeyAndOrderFront:wnd];

  casein_enable_filedrop(g_drop_enabled);
  if (*casein_fullscreen) enter_fullscreen();
  return wnd;
}

extern "C" void casein_interrupt(casein::interrupts irq) {
  switch (irq) {
  case casein::IRQ_CURSOR: *casein_cursor_visible ? [NSCursor unhide] : [NSCursor hide]; break;
  case casein::IRQ_FULLSCREEN: *casein_fullscreen ? enter_fullscreen() : leave_fullscreen(); break;
  case casein::IRQ_QUIT: [NSApp terminate:nil]; break;
  case casein::IRQ_WINDOW_SIZE: resize_window(); break;
  case casein::IRQ_WINDOW_TITLE: set_window_title(); break;
  }
}

int main(int argc, char ** argv) {
  @autoreleasepool {
    CASAppDelegate * app_d = [CASAppDelegate new];

    NSApplication * app = [NSApplication sharedApplication];
    app.delegate = app_d;
    create_key_window();
    app.mainMenu = setup_apple_menu();
    [app activateIgnoringOtherApps:YES];
    [app run];
    return 0;
  }
}
