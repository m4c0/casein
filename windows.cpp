module;
#include "casein.windows.hpp"

#include <exception>
#include <stdexcept>
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

// Forcing this after <windows.h>
#include <hidusage.h>

// https://handmade.network/forums/t/2011-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645565(v=vs.85).aspx
// https://docs.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
// https://stackoverflow.com/questions/2382464/win32-full-screen-and-hiding-taskbar
// https://devblogs.microsoft.com/oldnewthing/20050505-04/?p=35703
// https://gamedev.net/forums/topic/418397-problems-with-changedisplaysettingsex-solved/

module casein;

using namespace casein;

static constexpr const auto window_class = "m4c0-window-class";
static constexpr const auto timer_id = 0xb16b00b5;

static void handle_raw_mouse(RAWMOUSE & mouse) noexcept {
  if ((mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == 0) {
    auto x = casein::mouse_rel.x = mouse.lLastX;
    auto y = casein::mouse_rel.y = mouse.lLastY;
    if (x != 0 || y != 0) {
      casein_call(MOUSE_MOVE_REL);
    }
  }

  if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
    casein_call_m(MOUSE_DOWN, M_LEFT);
  } else if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
    casein_call_m(MOUSE_UP, M_LEFT);
  } else if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
    casein_call_m(MOUSE_DOWN, M_RIGHT);
  } else if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
    casein_call_m(MOUSE_UP, M_RIGHT);
  }
}

static void handle_raw_input(WPARAM wp, LPARAM lp) {
  RAWINPUT raw {};
  UINT raw_size = sizeof(raw);
  if (-1 == GetRawInputData((HRAWINPUT)lp, RID_INPUT, &raw, &raw_size, sizeof(RAWINPUTHEADER)))
    // TODO: log? abort?
    return;

  switch (raw.header.dwType) {
  case RIM_TYPEMOUSE:
    handle_raw_mouse(raw.data.mouse);
    break;
  }
}

static casein::keys wp2c(WPARAM wp) {
  switch (wp) {
  case VK_ESCAPE:
    return casein::K_ESCAPE;
  case VK_LEFT:
    return casein::K_LEFT;
  case VK_RIGHT:
    return casein::K_RIGHT;
  case VK_UP:
    return casein::K_UP;
  case VK_DOWN:
    return casein::K_DOWN;
  case VK_RETURN:
    return casein::K_ENTER;
  case VK_SPACE:
    return casein::K_SPACE;
  default:
    if (wp >= VK_F1 && wp <= VK_F12) {
      return static_cast<casein::keys>(casein::K_F1 + (wp - VK_F1));
    }
    if (wp >= 0x30 && wp <= 0x39) {
      return static_cast<casein::keys>(casein::K_0 + (wp - 0x30));
    }
    if (wp >= 0x41 && wp <= 0x5A) {
      return static_cast<casein::keys>(casein::K_A + (wp - 0x41));
    }
    return casein::K_NULL;
  }
}
static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
  switch (msg) {
  case WM_CREATE:
    native_ptr = hwnd;
    casein_call(CREATE_WINDOW);
    return 0;
  case WM_CLOSE:
    // Required to enable another thread sending "plz exit" messages
    DestroyWindow(hwnd);
    return 0;
  case WM_DESTROY:
    casein_call(QUIT);
    PostQuitMessage(0);
    return 0;
  case WM_EXITSIZEMOVE: {
    // WINDOWINFO wi {};
    // wi.cbSize = sizeof(WINDOWINFO);
    // GetWindowInfo(hwnd, &wi);

    // auto [l, t, r, b] = wi.rcClient;
    // auto w = r - l;
    // auto h = b - t;

    // casein_handle(casein::events::resize_window { { w, h, 1.0f, false } });
    casein_call(RESIZE_WINDOW);
    return 0;
  }
  case WM_INPUT:
    handle_raw_input(w_param, l_param);
    return 0;
  case WM_KEYDOWN:
    casein::keydown_repeating = (l_param & 0xFFFF) != 0;
    casein_call_k(KEY_DOWN, wp2c(w_param));
    return 0;
  case WM_KEYUP:
    casein_call_k(KEY_UP, wp2c(w_param));
    return 0;
  case WM_MOUSEMOVE: {
    casein::mouse_pos.x = GET_X_LPARAM(l_param);
    casein::mouse_pos.y = GET_Y_LPARAM(l_param);
    casein_call(MOUSE_MOVE);
    return 0;
  }
  case WM_PAINT:
    // From ValidateRect docs: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-validaterect
    // "The system continues to generate WM_PAINT messages until the current update region is validated."
    casein_call(REPAINT);
    return DefWindowProc(hwnd, msg, w_param, l_param);
  case WM_SIZE: {
    auto w = LOWORD(l_param);
    auto h = HIWORD(l_param);
    casein::window_size = { w, h };
    casein_call(RESIZE_WINDOW);
    return 0;
  }
  case WM_TIMER:
    if (w_param == timer_id) casein_call(TIMER);
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
      casein::base_width,
      casein::base_height,
      NULL,
      NULL,
      h_instance,
      NULL);
  if (!hwnd) throw std::runtime_error("Failed to create window");

  ShowWindow(hwnd, show);
  UpdateWindow(hwnd);

  return hwnd;
}

static void setup_raw_input() {
  RAWINPUTDEVICE rids[1] {};

  rids[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
  rids[0].usUsage = HID_USAGE_GENERIC_MOUSE;

  if (!RegisterRawInputDevices(rids, 1, sizeof(rids[0])))
    throw std::runtime_error("Failed to register raw input devices");
}

static HWND g_hwnd;
extern "C" void casein_exit(int code) {
  // This is the only way to properly programatically exit an app from any thread. Other attempts froze the app or kept
  // it as a "background app".
  SendMessage(g_hwnd, WM_CLOSE, 0, 0);
}

extern "C" void casein_set_title(const char * title) {
  SetWindowText(g_hwnd, title);
}

static RECT g_old_hwnd_rect;
static void set_window_rect(RECT rect) {
  auto [l, t, r, b] = rect;
  SetWindowPos(g_hwnd, nullptr, l, t, r - l, b - t, 0);
}
extern "C" void casein_enter_fullscreen() {
  if (!GetWindowRect(g_hwnd, &g_old_hwnd_rect)) return;

  HMONITOR hmon = MonitorFromWindow(g_hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFOEX mi {};
  mi.cbSize = sizeof(mi);
  if (!GetMonitorInfo(hmon, &mi)) return;

  SetWindowLongPtr(g_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

  // TODO: provide data about display w/h/refresh using EnumDisplaySettings

  DEVMODE dm {};
  EnumDisplaySettings(mi.szDevice, 0, &dm);
  dm.dmPelsWidth = casein::base_width;
  dm.dmPelsHeight = casein::base_height;
  dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
  if (ChangeDisplaySettingsEx(mi.szDevice, &dm, nullptr, CDS_FULLSCREEN, nullptr) == DISP_CHANGE_SUCCESSFUL) {
    ShowWindow(g_hwnd, SW_MAXIMIZE);
    return;
  }

  // Fallback to maximize
  set_window_rect(mi.rcMonitor);
}
extern "C" void casein_leave_fullscreen() {
  SetWindowLongPtr(g_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  set_window_rect(g_old_hwnd_rect);
  ChangeDisplaySettings(nullptr, CDS_RESET);
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

extern "C" int CALLBACK WinMain(
    _In_ HINSTANCE h_instance,
    _In_opt_ HINSTANCE /* unused */,
    _In_ LPSTR /* command line */,
    _In_ int cmd_show) try {
  register_class(h_instance);
  auto hwnd = g_hwnd = create_window(h_instance, cmd_show);
  setup_raw_input();
  return main_loop(hwnd);
} catch (const std::exception & e) {
  MessageBox(NULL, _T(e.what()), _T("Unhandled error"), NULL);
  return 1;
}
