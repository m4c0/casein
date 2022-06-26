#include "casein.hpp"
#include "glue/android_native_app_glue.h"

static void handle_command(android_app * app, int32_t cmd) {
  switch (cmd) {
  case APP_CMD_INIT_WINDOW:
    casein_event(casein::events::start {});
    break;
  case APP_CMD_TERM_WINDOW:
    casein_event(casein::events::stop {});
    break;
  case APP_CMD_STOP:
    casein_event(casein::events::quit {});
    ANativeActivity_finish(app->activity);
    break;
  default:
    break;
  }
}

static void process_command(android_app * app) {
  int events;
  android_poll_source * source;
  if (ALooper_pollAll(0, nullptr, &events, (void **)&source) >= 0) {
    if (source != nullptr) {
      source->process(app, source);
    }
  }
}

void android_main(android_app * app) {
  app->onAppCmd = handle_command;

  do {
    process_command(app);
  } while (app->destroyRequested == 0);
}
