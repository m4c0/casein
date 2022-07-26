#include "casein.hpp"
#include "casein.windows.hpp"

#include <exception>
#include <stdexcept>
#include <tchar.h>
#include <windows.h>

static constexpr const auto window_class = "m4c0-window-class";
static constexpr const auto repaint_timer_id = 0xb16b00b5;

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
  switch (msg) {
  case WM_CREATE:
    casein_event(casein::events::create_window { hwnd });
    return 0;
  case WM_DESTROY:
    casein_event(casein::events::quit {});
    PostQuitMessage(0);
    return 0;
  case WM_KEYDOWN:
    casein_event(casein::events::key_down { 0 });
    return 0;
  case WM_KEYUP:
    casein_event(casein::events::key_up { 0 });
    return 0;
  case WM_LBUTTONDOWN:
    casein_event(casein::events::mouse_down { 0 });
    return 0;
  case WM_LBUTTONUP:
    casein_event(casein::events::mouse_up { 0 });
    return 0;
  case WM_MOUSEMOVE:
    casein_event(casein::events::mouse_move { GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) });
    return 0;
  case WM_TIMER:
    if (w_param == repaint_timer_id) casein_event(casein::events::repaint {});
    return 0;
  default:
    return DefWindowProc(hwnd, msg, w_param, l_param);
  }
}

static void register_class(HINSTANCE h_instance) {
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = window_proc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = h_instance;
  wcex.hIcon = LoadIcon(h_instance, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = _T(window_class);
  wcex.hIconSm = LoadIcon(h_instance, IDI_APPLICATION);

  if (!RegisterClassEx(&wcex)) {
    throw std::runtime_error("Failed to register class");
  }
}

static auto create_window(HINSTANCE h_instance, int show) {
  constexpr const auto title_max_len = 256;
  TCHAR title[title_max_len];
  int size = LoadString(h_instance, IDS_CASEIN_APP_TITLE, title, title_max_len);

  auto hwnd = CreateWindow(
      _T(window_class),
      size > 0 ? static_cast<LPCTSTR>(title) : _T("App"),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      800,
      600,
      NULL,
      NULL,
      h_instance,
      NULL);
  if (!hwnd) throw std::runtime_error("Failed to create window");

  ShowWindow(hwnd, show);
  UpdateWindow(hwnd);

  return hwnd;
}

static int main_loop(HWND hwnd) {
  static constexpr const auto sixty_fps = 1000 / 30;

  MSG msg;

  SetTimer(hwnd, repaint_timer_id, sixty_fps, nullptr);
  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  KillTimer(hwnd, repaint_timer_id);
  return static_cast<int>(msg.wParam);
}

int CALLBACK WinMain(
    _In_ HINSTANCE h_instance,
    _In_opt_ HINSTANCE /* unused */,
    _In_ LPSTR /* command line */,
    _In_ int cmd_show) try {
  register_class(h_instance);
  auto hwnd = create_window(h_instance, cmd_show);
  return main_loop(hwnd);
} catch (const std::exception & e) {
  MessageBox(NULL, _T(e.what()), _T("Unhandled error"), NULL);
  return 1;
}
