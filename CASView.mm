using casein_native_handle = void;
#include "CASView.h"

#include "common.hpp"

@import MetalKit;

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
    casein_handle(casein::events::repaint {});
  } else {
    casein_handle(casein::events::create_window { (__bridge void *)self.layer });
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
  return self.window.contentScaleFactor;
#endif
}

#if TARGET_OS_IPHONE
- (bool)inLiveResize {
  return NO;
}
#endif

- (void)sendResizeEvent {
  casein_handle(casein::events::resize_window { {
      static_cast<int>(self.frame.size.width),
      static_cast<int>(self.frame.size.height),
      [self backingScaleFactor],
      [self inLiveResize] == YES,
  } });
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
  if (self.prepared) {
    [self sendResizeEvent];
  }
}

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
