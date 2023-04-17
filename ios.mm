@import MetalKit;
@import UIKit;

using casein_native_handle = CAMetalLayer;

#include "CASView.h"
#include "common.hpp"

@interface CASViewController : UIViewController
- (void)tap:(UIGestureRecognizer *)gr;
@end

@implementation CASViewController
- (BOOL)canBecomeFirstResponder {
  return YES;
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
  casein_handle(casein::events::gesture { casein::G_SHAKE });
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_handle(casein::events::touch_down {
      { static_cast<int>(p.x), static_cast<int>(p.y), static_cast<int>(t.tapCount) - 1 } });
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_handle(casein::events::touch_cancel {
      { static_cast<int>(p.x), static_cast<int>(p.y), static_cast<int>(t.tapCount) - 1 } });
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  CGPoint p = [[touches anyObject] locationInView:self.view];
  casein_handle(casein::events::touch_move { { static_cast<int>(p.x), static_cast<int>(p.y) } });
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_handle(
      casein::events::touch_up { { static_cast<int>(p.x), static_cast<int>(p.y), static_cast<int>(t.tapCount) - 1 } });
}

- (void)tap:(UITapGestureRecognizer *)gr {
  CGPoint p = [gr locationInView:[self view]];
  casein_handle(casein::events::touch_move { { static_cast<int>(p.x), static_cast<int>(p.y) } });
  if (gr.numberOfTapsRequired == 1) {
    casein_handle(casein::events::gesture { casein::G_TAP_1 });
  } else {
    casein_handle(casein::events::gesture { casein::G_TAP_2 });
  }
}
@end

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property (nonatomic, strong) UIWindow * window;
@end

@implementation CASAppDelegate
- (void)swipeLeft {
  casein_handle(casein::events::gesture { casein::G_SWIPE_LEFT });
}
- (void)swipeRight {
  casein_handle(casein::events::gesture { casein::G_SWIPE_RIGHT });
}
- (void)swipeTop {
  casein_handle(casein::events::gesture { casein::G_SWIPE_UP });
}
- (void)swipeBottom {
  casein_handle(casein::events::gesture { casein::G_SWIPE_DOWN });
}

- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
  app.applicationSupportsShakeToEdit = YES;

  UIViewController * vc = [CASViewController new];
  vc.view = [CASView new];

  UISwipeGestureRecognizer * left = [UISwipeGestureRecognizer new];
  left.direction = UISwipeGestureRecognizerDirectionLeft;
  [left addTarget:self action:@selector(swipeLeft)];
  [vc.view addGestureRecognizer:left];

  UISwipeGestureRecognizer * right = [UISwipeGestureRecognizer new];
  right.direction = UISwipeGestureRecognizerDirectionRight;
  [right addTarget:self action:@selector(swipeRight)];
  [vc.view addGestureRecognizer:right];

  UISwipeGestureRecognizer * top = [UISwipeGestureRecognizer new];
  top.direction = UISwipeGestureRecognizerDirectionUp;
  [top addTarget:self action:@selector(swipeTop)];
  [vc.view addGestureRecognizer:top];

  UISwipeGestureRecognizer * bottom = [UISwipeGestureRecognizer new];
  bottom.direction = UISwipeGestureRecognizerDirectionDown;
  [bottom addTarget:self action:@selector(swipeBottom)];
  [vc.view addGestureRecognizer:bottom];

  UITapGestureRecognizer * dblTap = [UITapGestureRecognizer new];
  dblTap.numberOfTapsRequired = 2;
  dblTap.numberOfTouchesRequired = 1;
  [dblTap addTarget:vc action:@selector(tap:)];
  [vc.view addGestureRecognizer:dblTap];

  UITapGestureRecognizer * tap = [UITapGestureRecognizer new];
  tap.numberOfTapsRequired = 1;
  tap.numberOfTouchesRequired = 1;
  [tap requireGestureRecognizerToFail:dblTap];
  [tap addTarget:vc action:@selector(tap:)];
  [vc.view addGestureRecognizer:tap];

  self.window = [UIWindow new];
  self.window.frame = [UIScreen mainScreen].bounds;
  self.window.rootViewController = vc;
  [self.window makeKeyAndVisible];
  return YES;
}
- (void)applicationWillTerminate:(UIApplication *)app {
  casein_handle(casein::events::quit {});
}
@end

int main(int argc, char ** argv) {
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, @"CASAppDelegate");
  }
}
