@import UIKit;
#import "CASView.h"
#import "externc.hpp"

@interface CASViewController : UIViewController
@end

@implementation CASViewController
- (BOOL)canBecomeFirstResponder {
  return YES;
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
  casein_call_g(casein::GESTURE, casein::G_SHAKE);
}

// - (casein::touch)touchOfLocatable:(id)t loong:(bool)l {
//   CGPoint p = [t locationInView:[self view]];
//   return casein::touch { static_cast<int>(p.x), static_cast<int>(p.y), l };
// }
// - (casein::touch)touchOfTouches:(NSSet *)touches {
//   UITouch * t = [touches anyObject];
//   return [self touchOfLocatable:t loong:false];
// }
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  // casein_handle(casein::events::touch_down { [self touchOfTouches:touches] });
  casein_call(casein::TOUCH_DOWN);
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  // casein_handle(casein::events::touch_cancel { [self touchOfTouches:touches] });
  casein_call(casein::TOUCH_CANCEL);
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  // casein_handle(casein::events::touch_move { [self touchOfTouches:touches] });
  casein_call(casein::TOUCH_MOVE);
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  // casein_handle(casein::events::touch_up { [self touchOfTouches:touches] });
  casein_call(casein::TOUCH_UP);
}
- (void)press:(UIGestureRecognizer *)gr {
  // TODO: store "T" into a register
  // casein::touch t = [self touchOfLocatable:gr loong:true];

  switch (gr.state) {
  case UIGestureRecognizerStateBegan:
    casein_call(casein::TOUCH_DOWN);
    break;
  case UIGestureRecognizerStateChanged:
    casein_call(casein::TOUCH_MOVE);
    break;
  case UIGestureRecognizerStateCancelled:
    casein_call(casein::TOUCH_CANCEL);
    break;
  case UIGestureRecognizerStateEnded:
    casein_call(casein::TOUCH_UP);
    break;
  default:
    break;
  }
}
@end

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property (nonatomic, strong) UIWindow * window;
@end

@implementation CASAppDelegate
- (void)swipeLeft {
  casein_call_g(casein::GESTURE, casein::G_SWIPE_LEFT);
}
- (void)swipeRight {
  casein_call_g(casein::GESTURE, casein::G_SWIPE_RIGHT);
}
- (void)swipeTop {
  casein_call_g(casein::GESTURE, casein::G_SWIPE_UP);
}
- (void)swipeBottom {
  casein_call_g(casein::GESTURE, casein::G_SWIPE_DOWN);
}

- (void)tap {
  casein_call_g(casein::GESTURE, casein::G_TAP_1);
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
  [press addTarget:vc action:@selector(press:)];
  [vc.view addGestureRecognizer:press];

  self.window = [UIWindow new];
  self.window.frame = [UIScreen mainScreen].bounds;
  self.window.rootViewController = vc;
  [self.window makeKeyAndVisible];
  return YES;
}
- (void)applicationWillTerminate:(UIApplication *)app {
  casein_call(casein::QUIT);
}
@end

extern "C" void casein_exit(int code) {
  abort();
}
int main(int argc, char ** argv) {
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, @"CASAppDelegate");
  }
}
