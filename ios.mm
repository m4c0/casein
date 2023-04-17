@import MetalKit;
@import UIKit;

using casein_native_handle = CAMetalLayer;

#include "CASView.h"
#include "common.hpp"

@interface CASViewController : UIViewController
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
  casein_handle(casein::events::touch_down { { static_cast<int>(p.x), static_cast<int>(p.y) } });
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_handle(casein::events::touch_cancel { { static_cast<int>(p.x), static_cast<int>(p.y) } });
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  CGPoint p = [[touches anyObject] locationInView:self.view];
  casein_handle(casein::events::touch_move { { static_cast<int>(p.x), static_cast<int>(p.y) } });
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch * t = [touches anyObject];
  CGPoint p = [t locationInView:[self view]];
  casein_handle(casein::events::touch_up { { static_cast<int>(p.x), static_cast<int>(p.y) } });
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

- (void)tap {
  casein_handle(casein::events::gesture { casein::G_TAP_1 });
}
- (void)press {
  casein_handle(casein::events::gesture { casein::G_LONG_PRESS });
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

  UITapGestureRecognizer * tap = [UITapGestureRecognizer new];
  tap.numberOfTapsRequired = 1;
  tap.numberOfTouchesRequired = 1;
  [tap addTarget:self action:@selector(tap)];
  [vc.view addGestureRecognizer:tap];

  UILongPressGestureRecognizer * press = [UILongPressGestureRecognizer new];
  [press requireGestureRecognizerToFail:tap];
  [press addTarget:self action:@selector(press)];
  [vc.view addGestureRecognizer:press];

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
