#include "casein.windows.hpp"

#include <exception>
#include <stdexcept>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

import casein;

static constexpr const auto window_class = "m4c0-window-class";
static constexpr const auto timer_id = 0xb16b00b5;

extern "C" void casein_handle(const casein::event & e);

static casein::keys wp2c(WPARAM wp) {
  switch (wp) {
  case VK_LEFT:
    return casein::K_LEFT;
  case VK_RIGHT:
    return casein::K_RIGHT;
  case VK_UP:
    return casein::K_UP;
  case VK_DOWN:
    return casein::K_DOWN;
  case VK_SPACE:
    return casein::K_SPACE;
  default:
    if (wp >= 0x41 && wp <= 0x5A) {
      return static_cast<casein::keys>(casein::K_A + (wp - 0x41));
    }
    return casein::K_NULL;
  }
}
static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
  switch (msg) {
  case WM_CREATE:
    casein_handle(casein::events::create_window { hwnd });
    return 0;
  case WM_DESTROY:
    casein_handle(casein::events::quit {});
    PostQuitMessage(0);
    return 0;
  case WM_KEYDOWN:
    casein_handle(casein::events::key_down { wp2c(w_param) });
    return 0;
  case WM_KEYUP:
    casein_handle(casein::events::key_up { wp2c(w_param) });
    return 0;
  case WM_LBUTTONDOWN: {
    auto x = GET_X_LPARAM(l_param);
    auto y = GET_Y_LPARAM(l_param);
    casein_handle(casein::events::mouse_down { { static_cast<int>(x), static_cast<int>(y), casein::M_LEFT } });
    return 0;
  }
  case WM_LBUTTONUP: {
    auto x = GET_X_LPARAM(l_param);
    auto y = GET_Y_LPARAM(l_param);
    casein_handle(casein::events::mouse_up { { static_cast<int>(x), static_cast<int>(y), casein::M_LEFT } });
    return 0;
  }
  case WM_MOUSEMOVE:
    casein_handle(casein::events::mouse_move { { GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) } });
    return 0;
  case WM_PAINT:
    casein_handle(casein::events::repaint {});
    return DefWindowProc(hwnd, msg, w_param, l_param);
  case WM_RBUTTONDOWN: {
    auto x = GET_X_LPARAM(l_param);
    auto y = GET_Y_LPARAM(l_param);
    casein_handle(casein::events::mouse_down { { static_cast<int>(x), static_cast<int>(y), casein::M_RIGHT } });
    return 0;
  }
  case WM_RBUTTONUP: {
    auto x = GET_X_LPARAM(l_param);
    auto y = GET_Y_LPARAM(l_param);
    casein_handle(casein::events::mouse_up { { static_cast<int>(x), static_cast<int>(y), casein::M_RIGHT } });
    return 0;
  }
  case WM_SIZE: {
    auto w = LOWORD(l_param);
    auto h = HIWORD(l_param);
    casein_handle(casein::events::resize_window { { w, h, 1.0f, false } });
    return 0;
  }
  case WM_TIMER:
    if (w_param == timer_id) casein_handle(casein::events::timer {});
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
  static constexpr const auto ms_per_tick = 1000 / 20;

  MSG msg;

  SetTimer(hwnd, timer_id, ms_per_tick, nullptr);
  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  KillTimer(hwnd, timer_id);
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
