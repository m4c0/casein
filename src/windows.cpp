#include <stdexcept>
#include <tchar.h>
#include <windows.h>

int CALLBACK WinMain(
    _In_ HINSTANCE h_instance,
    _In_opt_ HINSTANCE /* unused */,
    _In_ LPSTR /* command line */,
    _In_ int cmd_show) try {
  return 0;
} catch (const std::exception & e) {
  MessageBox(NULL, _T(e.what()), _T("Unhandled error"), NULL);
  return 1;
}
