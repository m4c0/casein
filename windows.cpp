#pragma leco add_library shell32
module;
#include "casein.windows.hpp"
#include "externc.hpp"

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

// See Remarks of this link for how to get the name of a key
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawkeyboard

// TODO: consider use Windows scan codes to give an experience based on keyboard position
// https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input#scan-codes

module casein;

using namespace casein;

static constexpr const auto window_class = "m4c0-window-class";

static constexpr const auto dw_style = WS_OVERLAPPEDWINDOW;

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
  case RIM_TYPEMOUSE: handle_raw_mouse(raw.data.mouse); break;
  }
}

static casein::keys wp2c(WPARAM wp) {
  switch (wp) {
  case VK_TAB: return casein::K_TAB;
  case VK_ESCAPE: return casein::K_ESCAPE;
  case VK_LEFT: return casein::K_LEFT;
  case VK_RIGHT: return casein::K_RIGHT;
  case VK_UP: return casein::K_UP;
  case VK_DOWN: return casein::K_DOWN;
  case VK_RETURN: return casein::K_ENTER;
  case VK_SPACE: return casein::K_SPACE;

  // TODO: deal with these internationally
  case VK_OEM_COMMA: return casein::K_COMMA;
  case VK_OEM_MINUS: return casein::K_MINUS;
  case VK_OEM_PERIOD: return casein::K_DOT;
  case VK_OEM_PLUS: return casein::K_EQUAL;
  case VK_OEM_2: return casein::K_SLASH;
  case VK_OEM_5: return casein::K_BACKSLASH;
  case VK_OEM_4: return casein::K_LBRACKET;
  case VK_OEM_6: return casein::K_RBRACKET;

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
  case WM_DROPFILES: {
    auto h_drop = reinterpret_cast<HDROP>(w_param);
    casein_clear_drops();

    auto count = DragQueryFile(h_drop, 0xFFFFFFFF, nullptr, 0);
    for (auto i = 0; i < count; i++) {
      char buffer[10240];
      auto sz = DragQueryFile(h_drop, i, buffer, sizeof(buffer));
      casein_add_drop(buffer, sz);
    }
    DragFinish(h_drop);
    casein_call(FILES_DROP);
    return 0;
  }
  case WM_ENTERSIZEMOVE:
    casein::window_live_resize = true;
    casein_call(RESIZE_WINDOW);
    return 0;
  case WM_EXITSIZEMOVE:
    casein::window_live_resize = false;
    casein_call(RESIZE_WINDOW);
    return 0;
  case WM_INPUT: handle_raw_input(w_param, l_param); return 0;
  case WM_KEYDOWN: {
    auto key_flags = HIWORD(l_param);
    casein::keydown_repeating = (key_flags & KF_REPEAT) == KF_REPEAT;
    casein_call_k(KEY_DOWN, wp2c(LOWORD(w_param)));
    return 0;
  }
  case WM_KEYUP: casein_call_k(KEY_UP, wp2c(LOWORD(w_param))); return 0;
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
    ValidateRect(hwnd, nullptr);
    return 0;
  case WM_SIZE: {
    auto w = LOWORD(l_param);
    auto h = HIWORD(l_param);
    casein::window_size = { w, h };
    casein_call(RESIZE_WINDOW);
    return 0;
  }
  default: return DefWindowProc(hwnd, msg, w_param, l_param);
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
  wcex.hIcon = LoadIcon(h_instance, "IDI_CASEIN_ICON");
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = _T(window_class);
  wcex.hIconSm = LoadIcon(h_instance, "IDI_CASEIN_ICON");

  if (!RegisterClassEx(&wcex)) {
    throw std::runtime_error("Failed to register class");
  }
}

static auto get_client_size() {
  struct {
    int x, y;
  } res;

  RECT cli_rect { 0, 0, static_cast<long>(casein::window_size.x), static_cast<long>(casein::window_size.y) };
  AdjustWindowRect(&cli_rect, dw_style, false);

  res.x = cli_rect.right - cli_rect.left;
  res.y = cli_rect.bottom - cli_rect.top;
  return res;
}

static auto create_window(HINSTANCE h_instance, int show) {
  constexpr const auto title_max_len = 256;
  TCHAR title[title_max_len];
  int size = LoadString(h_instance, IDS_CASEIN_APP_TITLE, title, title_max_len);

  auto [w, h] = get_client_size();
  auto hwnd =
      CreateWindow(_T(window_class), size > 0 ? static_cast<LPCTSTR>(title) : _T(casein::window_title.cstr().begin()),
                   dw_style, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, h_instance, NULL);
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

static bool g_drop_enabled;
extern "C" void casein_enable_filedrop(bool en) {
  g_drop_enabled = en;
  if (!g_hwnd) return;

  DragAcceptFiles(g_hwnd, en);
}

static RECT g_old_hwnd_rect;
static void set_window_rect(RECT rect) {
  auto [l, t, r, b] = rect;
  SetWindowPos(g_hwnd, nullptr, l, t, r - l, b - t, 0);
}
static void enter_fullscreen() {
  if (!GetWindowRect(g_hwnd, &g_old_hwnd_rect)) return;

  HMONITOR hmon = MonitorFromWindow(g_hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFOEX mi {};
  mi.cbSize = sizeof(mi);
  if (!GetMonitorInfo(hmon, &mi)) return;

  SetWindowLongPtr(g_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

  // TODO: provide data about display w/h/refresh using EnumDisplaySettings

  //DEVMODE dm {};
  //EnumDisplaySettings(mi.szDevice, 0, &dm);
  //dm.dmPelsWidth = casein::window_size.x;
  //dm.dmPelsHeight = casein::window_size.y;
  //dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
  //if (ChangeDisplaySettingsEx(mi.szDevice, &dm, nullptr, CDS_FULLSCREEN, nullptr) == DISP_CHANGE_SUCCESSFUL) {
  //  ShowWindow(g_hwnd, SW_MAXIMIZE);
  //  return;
  //}

  // Fallback to maximize
  set_window_rect(mi.rcMonitor);
}
static void leave_fullscreen() {
  SetWindowLongPtr(g_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  set_window_rect(g_old_hwnd_rect);
  //ChangeDisplaySettings(nullptr, CDS_RESET);
}

static void resize_window() {
  RECT rect {};
  GetWindowRect(g_hwnd, &rect);

  auto [w, h] = get_client_size();
  MoveWindow(g_hwnd, rect.left, rect.top, w, h, true);
}

static void set_cursor_pos() {
  // Avoids warping if user is on another app
  if (GetForegroundWindow() != g_hwnd) return;

  RECT rect {};
  GetWindowRect(g_hwnd, &rect);

  auto x = casein::mouse_pos.x + rect.left;
  auto y = casein::mouse_pos.y + rect.top;
  SetCursorPos(x, y);
}

static void set_cursor_visibility() {
  static HCURSOR shown  = LoadCursor(NULL, IDC_ARROW);
  static HCURSOR hidden = [] {
    BYTE CursorMaskAND[] = { 0xFF };
    BYTE CursorMaskXOR[] = { 0x00 };
    return CreateCursor(NULL, 0, 0, 1, 1, CursorMaskAND, CursorMaskXOR);
  }();
  // The only "sane" way to change cursor is via the window class.
  // Other changes affect the global cursor.
  auto h = casein::cursor_visible ? shown : hidden;
  SetClassLongPtr(g_hwnd, GCLP_HCURSOR, (LONG_PTR)h);
}

static void set_fullscreen() {
  casein::fullscreen ? enter_fullscreen() : leave_fullscreen();
}

static void quit() {
  // This is the only way to properly programatically exit an app from any
  // thread. Other attempts froze the app or kept it as a "background app".
  // i.e. We use WM_CLOSE instead of WM_QUIT (or PostQuitMessage etc) and we
  // need to use SendNotifyMessage instead of SendMessage.
  SendNotifyMessage(g_hwnd, WM_CLOSE, 0, 0);
}

static void set_window_title() {
  SetWindowText(g_hwnd, casein::window_title.cstr().begin());
}

static void set_window_size() {
  // TODO: set fullscreen resolution
  if (casein::fullscreen) return;

  resize_window();
}

void casein::interrupt(casein::interrupts irq) {
  if (!g_hwnd) return;

  switch (irq) {
    case IRQ_CURSOR:       set_cursor_visibility(); break;
    case IRQ_FULLSCREEN:   set_fullscreen();        break;
    case IRQ_MOUSE_POS:    set_cursor_pos();        break;
    case IRQ_QUIT:         quit();                  break;
    case IRQ_WINDOW_TITLE: set_window_title();      break;
    case IRQ_WINDOW_SIZE:  set_window_size();       break;
  }
}

static int main_loop(HWND hwnd) {
  MSG msg;

  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return static_cast<int>(msg.wParam);
}

extern "C" void casein_init();
extern "C" int CALLBACK WinMain(_In_ HINSTANCE h_instance, _In_opt_ HINSTANCE /* unused */,
                                _In_ LPSTR /* command line */, _In_ int cmd_show) try {
  casein_init();

  register_class(h_instance);
  auto hwnd = g_hwnd = create_window(h_instance, cmd_show);
  casein_enable_filedrop(g_drop_enabled);
  set_cursor_visibility();
  if (casein::fullscreen) enter_fullscreen();
  setup_raw_input();
  return main_loop(hwnd);
} catch (const std::exception & e) {
  MessageBox(NULL, _T(e.what()), _T("Unhandled error"), NULL);
  return 1;
} catch (...) {
  MessageBox(NULL, _T("Check the application logs for more details"), _T("Unhandled error"), NULL);
  return 1;
}
