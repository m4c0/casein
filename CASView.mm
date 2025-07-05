#include "CASView.h"

#include "externc.hpp"

extern void ** casein_native_ptr;

@implementation CASView
- (instancetype)init {
  self = [super init];
  if (self) {
    self.device = MTLCreateSystemDefaultDevice();
    self.delegate = self;
  }
  return self;
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (void)drawInMTKView:(MTKView *)view {
  if (self.prepared) {
    casein_call(casein::REPAINT);
  } else {
    *casein_native_ptr = (__bridge void *)self.layer;
    casein_call(casein::CREATE_WINDOW);

    [self sendResizeEvent];
    self.prepared = YES;
  }
}

- (float)backingScaleFactor {
#if TARGET_OS_OSX
  NSWindow * window = [self window];
  if (!window) window = [NSApp mainWindow];
  if (!window) return 1.0f;
  return window.backingScaleFactor;
#else
  return self.contentScaleFactor;
#endif
}

#if TARGET_OS_IPHONE
- (bool)inLiveResize {
  return NO;
}
#else
- (void)viewDidEndLiveResize {
  [self sendResizeEvent];
}
#endif

#if TARGET_OS_OSX
- (void)mouseEntered:(NSEvent *)event { casein_call(casein::MOUSE_ENTER); }
- (void)mouseExited:(NSEvent *)event  { casein_call(casein::MOUSE_LEAVE); }
- (void)updateTrackingAreas {
  [super updateTrackingAreas];

  if ([self.trackingAreas count]) [self removeTrackingArea:self.trackingAreas[0]];

  NSTrackingAreaOptions opts = NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways;
  NSTrackingArea * area = [[NSTrackingArea alloc] initWithRect:self.bounds options:opts owner:self userInfo:nil];
  [self addTrackingArea:area];
}
#endif

- (void)sendResizeEvent {
  *casein_window_size = {
    static_cast<float>(self.frame.size.width),
    static_cast<float>(self.frame.size.height),
  };
  *casein_screen_scale_factor = [self backingScaleFactor];
  *casein_window_live_resize = [self inLiveResize] == YES;
  casein_call(casein::RESIZE_WINDOW);
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
  if (self.prepared) {
    [self sendResizeEvent];
  }
}

#if TARGET_OS_OSX
- (void)scrollWheel:(NSEvent *)e {
  *casein_mouse_rel = {
    static_cast<float>(e.deltaX),
    static_cast<float>(e.deltaY),
  };
  casein_call_m(casein::MOUSE_MOVE, casein::M_WHEEL);
}
#endif

// http://smokyonion.github.io/blog/2012/11/11/how-to-make-your-mac-apps-retina-ready/
- (void)scaleDidChange:(NSNotification *)n {
  if ([self.layer respondsToSelector:@selector(contentsScale)]) {
    [self.layer setContentsScale:[self backingScaleFactor]];
  }
}

- (void)viewDidMoveToWindow {
  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(scaleDidChange:)
                                               name:@"NSWindowDidChangeBackingPropertiesNotification"
                                             object:[self window]];
  [self scaleDidChange:nil];
}

- (void)removeFromSuperview {
  [super removeFromSuperview];
  [[NSNotificationCenter defaultCenter] removeObserver:self
                                                  name:@"NSWindowDidChangeBackingPropertiesNotification"
                                                object:[self window]];
}
@end
