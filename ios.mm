@import UIKit;
#import "CASView.h"
#import "externc.hpp"

extern "C" void casein_enable_filedrop(bool) {}

extern "C" void casein_interrupt(casein::interrupts irq) {
  // These are here as a reminder, if we ever got an IRQ tha makes sense in iOS
  switch (irq) {
    case casein::IRQ_FULLSCREEN:
    case casein::IRQ_QUIT:
    case casein::IRQ_WINDOW_SIZE:
    case casein::IRQ_WINDOW_TITLE: break;
  }
}

@interface CASViewController : UIViewController
@end

@implementation CASViewController
- (BOOL)canBecomeFirstResponder {
  return YES;
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
  casein_call_g(casein::GESTURE, casein::G_SHAKE);
}

- (void)updateMousePosFromLocatable:(id)t {
  CGPoint p = [t locationInView:[self view]];
  *casein_mouse_pos = { static_cast<float>(p.x), static_cast<float>(p.y) };
}
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  [self updateMousePosFromLocatable:[touches anyObject]];
  casein_call(casein::TOUCH_DOWN);
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  [self updateMousePosFromLocatable:[touches anyObject]];
  casein_call(casein::TOUCH_CANCEL);
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  [self updateMousePosFromLocatable:[touches anyObject]];
  casein_call(casein::TOUCH_MOVE);
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [self updateMousePosFromLocatable:[touches anyObject]];
  casein_call(casein::TOUCH_UP);
}
@end

@interface CASAppDelegate : NSObject<UIApplicationDelegate>
@property(nonatomic, strong) UIWindow * window;
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
- (void)press {
  casein_call_g(casein::GESTURE, casein::G_LONG_PRESS);
}
- (void)tap {
  casein_call_g(casein::GESTURE, casein::G_TAP_1);
}

- (void)applicationWillResignActive:(UIApplication *)application {
  casein_call(casein::ENTER_BACKGROUND);
}
- (void)applicationDidBecomeActive:(UIApplication *)application {
  casein_call(casein::LEAVE_BACKGROUND);
}

- (BOOL)application:(UIApplication *)app didFinishLaunchingWithOptions:(id)options {
  app.applicationSupportsShakeToEdit = YES;

  UIViewController * vc = [CASViewController new];
  vc.view = [CASView new];

  UISwipeGestureRecognizer * left = [UISwipeGestureRecognizer new];
  left.direction = UISwipeGestureRecognizerDirectionLeft;
  left.cancelsTouchesInView = NO;
  [left addTarget:self action:@selector(swipeLeft)];
  [vc.view addGestureRecognizer:left];

  UISwipeGestureRecognizer * right = [UISwipeGestureRecognizer new];
  right.direction = UISwipeGestureRecognizerDirectionRight;
  right.cancelsTouchesInView = NO;
  [right addTarget:self action:@selector(swipeRight)];
  [vc.view addGestureRecognizer:right];

  UISwipeGestureRecognizer * top = [UISwipeGestureRecognizer new];
  top.direction = UISwipeGestureRecognizerDirectionUp;
  top.cancelsTouchesInView = NO;
  [top addTarget:self action:@selector(swipeTop)];
  [vc.view addGestureRecognizer:top];

  UISwipeGestureRecognizer * bottom = [UISwipeGestureRecognizer new];
  bottom.direction = UISwipeGestureRecognizerDirectionDown;
  bottom.cancelsTouchesInView = NO;
  [bottom addTarget:self action:@selector(swipeBottom)];
  [vc.view addGestureRecognizer:bottom];

  UITapGestureRecognizer * tap = [UITapGestureRecognizer new];
  tap.numberOfTapsRequired = 1;
  tap.numberOfTouchesRequired = 1;
  tap.cancelsTouchesInView = NO;
  [tap addTarget:self action:@selector(tap)];
  [vc.view addGestureRecognizer:tap];

  UILongPressGestureRecognizer * press = [UILongPressGestureRecognizer new];
  press.cancelsTouchesInView = NO;
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
  casein_call(casein::QUIT);
}
@end

extern "C" void casein_init();
extern "C" void casein_exit(int code) { abort(); }
int main(int argc, char ** argv) {
  casein_init();
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, @"CASAppDelegate");
  }
}
