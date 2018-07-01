#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <TCHAR.h>

#include <gles/egl.h>
#include <gles/gl.h>

#include <epod.h>

#pragma comment(lib, "libgles_cm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "openal32.lib")


// Windows class name to register
const char *WINDOW_CLASS = "EPodWin32Class";


void save_window_pos(HWND hWnd)
{
  RECT rect;
  if (GetWindowRect(hWnd, &rect))
  {
    HKEY key = 0;
    if (RegCreateKey(HKEY_CURRENT_USER, "EPod", &key) == ERROR_SUCCESS)
    {
      RegSetValueEx(key, "WindowPosX", 0, REG_DWORD, (const BYTE *)&rect.left, sizeof(rect.left));
      RegSetValueEx(key, "WindowPosY", 0, REG_DWORD, (const BYTE *)&rect.top, sizeof(rect.top));

      RegCloseKey(key);
    }
  }
}

void load_window_pos(int &x, int &y)
{
  HKEY key = 0;
  if (RegOpenKey(HKEY_CURRENT_USER, "EPod", &key) == ERROR_SUCCESS)
  {
    int sizex = sizeof(x);
    RegQueryValueEx(key, "WindowPosX", 0, NULL, (LPBYTE)&x, (LPDWORD)&sizex);
    int sizey = sizeof(y);
    RegQueryValueEx(key, "WindowPosY", 0, NULL, (LPBYTE)&y, (LPDWORD)&sizey);

    RegCloseKey(key);
  }
}


bool g_bAppDone = false;
bool g_bMouseDown = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_CLOSE:
      g_bAppDone = true;
      PostQuitMessage(0);

      save_window_pos(hWnd);
      return 1;

    case WM_ACTIVATE:
      {
        int activate = (int)(short)LOWORD(wParam);
        ep::IScene *scene = ep::get_root_scene();
        if (scene)
          (activate == WA_INACTIVE) ? scene->onPause() : scene->onResume();
      }
      return 0;

    case WM_LBUTTONDOWN:
      g_bMouseDown = true;
      SetCapture(hWnd);
      {
        ep::IScene *scene = ep::get_root_scene();
        if (scene)
          scene->onPointerDown(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      }
      return 0;

    case WM_LBUTTONUP:
      g_bMouseDown = false;
      ReleaseCapture();
      {
        ep::IScene *scene = ep::get_root_scene();
        if (scene)
          scene->onPointerUp(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      }
      return 0;

    case WM_MOUSEMOVE:
      if (g_bMouseDown)
      {
        ep::IScene *scene = ep::get_root_scene();
        if (scene)
          scene->onPointerMove(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      }
      return 0;

    case WM_KEYDOWN:
      {
        ep::IScene *scene = ep::get_root_scene();
        if (scene)
          scene->onKeyDown((int)wParam);
      }
      return 0;

    case WM_KEYUP:
      {
        ep::IScene *scene = ep::get_root_scene();
        if (scene)
          scene->onKeyUp((int)wParam);
      }
      return 0;

    default:
      break;
  }

  // Calls the default window procedure for messages we did not handle
  return DefWindowProc(hWnd, message, wParam, lParam);
}


bool TestEGLError(HWND hWnd, char* pszLocation)
{
  // eglGetError returns the last error that has happened using egl,
  // not the status of the last called function. The user has to
  // check after every single egl call or at least once every frame.

  EGLint iErr = eglGetError();
  if (iErr != EGL_SUCCESS)
  {
    TCHAR pszStr[256];
    _stprintf_s(pszStr, 256, _T("%s failed (%d).\n"), pszLocation, iErr);
    MessageBox(hWnd, pszStr, _T("Error"), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  return true;
}


bool start_window(HINSTANCE hInstance, HWND &hWnd, HDC &hDC, EGLDisplay &eglDisplay)
{
  // EGL variables
  EGLConfig     eglConfig = 0;
  EGLSurface    eglSurface = 0;
  EGLContext    eglContext = 0;
  NativeWindowType eglWindow = 0;
  EGLint        pi32ConfigAttribs[128];

  if (!ep::init_epod())
  {
    MessageBox(0, _T("init_epod() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  // Step 0 - Create a NativeWindowType that we can use for OpenGL ES output

  // Register the windows class
  WNDCLASS sWC;
  sWC.style = CS_HREDRAW | CS_VREDRAW;
  sWC.lpfnWndProc = WndProc;
  sWC.cbClsExtra = 0;
  sWC.cbWndExtra = 0;
  sWC.hInstance = hInstance;
  sWC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  sWC.hCursor = LoadCursor(NULL, IDC_ARROW);
  sWC.lpszMenuName = 0;
  sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  sWC.lpszClassName = WINDOW_CLASS;

  ATOM registerClass = RegisterClass(&sWC);
  if (!registerClass)
  {
    MessageBox(0, _T("Failed to register the window class"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  int posx = CW_USEDEFAULT;
  int posy = CW_USEDEFAULT;

  load_window_pos(posx, posy);

  // Create the eglWindow
  RECT sRect;
  SetRect(&sRect, 0, 0, ep::get_width(), ep::get_height());
  AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);

  hWnd = CreateWindow(WINDOW_CLASS, ep::get_app_name(), WS_VISIBLE | WS_SYSMENU, posx, posy,
    sRect.right - sRect.left, sRect.bottom - sRect.top, NULL, NULL, hInstance, NULL);
  eglWindow = hWnd;

  // Get the associated device context
  hDC = GetDC(hWnd);
  if (!hDC)
  {
    MessageBox(0, _T("Failed to create the device context"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  // Step 1 - Get the default display.
  // EGL uses the concept of a "display" which in most environments
  // corresponds to a single physical screen. Since we usually want
  // to draw to the main screen or only have a single screen to begin
  // with, we let EGL pick the default display.
  // Querying other displays is platform specific.

  eglDisplay = eglGetDisplay((NativeDisplayType)hDC);

  if(eglDisplay == EGL_NO_DISPLAY)
    eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);

  // Step 2 - Initialize EGL.
  // EGL has to be initialized with the display obtained in the
  // previous step. We cannot use other EGL functions except
  // eglGetDisplay and eglGetError before eglInitialize has been
  // called.
  // If we're not interested in the EGL version number we can just
  // pass NULL for the second and third parameters.

  EGLint iMajorVersion, iMinorVersion;
  if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
  {
    MessageBox(0, _T("eglInitialize() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  // Step 3 - Specify the required configuration attributes.
  // An EGL "configuration" describes the pixel format and type of
  // surfaces that can be used for drawing.
  // For now we just want to use a 16 bit RGB surface that is a
  // Window surface, i.e. it will be visible on screen. The list
  // has to contain key/value pairs, terminated with EGL_NONE.

  int i = 0;
  pi32ConfigAttribs[i++] = EGL_RED_SIZE;
  pi32ConfigAttribs[i++] = 5;
  pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
  pi32ConfigAttribs[i++] = 6;
  pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
  pi32ConfigAttribs[i++] = 5;
  pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
  pi32ConfigAttribs[i++] = 0;
  pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
  pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
  pi32ConfigAttribs[i++] = EGL_NONE;

  // Step 4 - Find a config that matches all requirements.
  // eglChooseConfig provides a list of all available configurations
  // that meet or exceed the requirements given as the second
  // argument. In most cases we just want the first config that meets
  // all criteria, so we can limit the number of configs returned to 1.

  EGLint iConfigs;
  if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
  {
    MessageBox(0, _T("eglChooseConfig() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  // Step 5 - Create a surface to draw to.
  // Use the config picked in the previous step and the native window
  // handle when available to create a window surface. A window surface
  // is one that will be visible on screen inside the native display (or
  // fullscreen if there is no windowing system).
  // Pixmaps and pbuffers are surfaces which only exist in off-screen
  // memory.

  eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

  if(eglSurface == EGL_NO_SURFACE)
  {
    eglGetError(); // Clear error
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
  }

  if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
    return false;

  // Step 6 - Create a context.
  // EGL has to create a context for OpenGL ES. Our OpenGL ES resources
  // like textures will only be valid inside this context
  // (or shared contexts)

  eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
  if (!TestEGLError(hWnd, "eglCreateContext"))
    return false;

  // Step 7 - Bind the context to the current thread and use our
  // window surface for drawing and reading.
  // Contexts are bound to a thread. This means you don't have to
  // worry about other threads and processes interfering with your
  // OpenGL ES application.
  // We need to specify a surface that will be the target of all
  // subsequent drawing operations, and one that will be the source
  // of read operations. They can be the same surface.

  eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
  if (!TestEGLError(hWnd, "eglMakeCurrent"))
    return false;

  // Step 8 - Draw something with OpenGL ES.
  // At this point everything is initialized and we're ready to use
  // OpenGL ES to draw something on the screen.

  ep::IScene *root = ep::get_root_scene();
  root->init();

  int timeLast = ep::get_time();

  while(1)
  {
    if (g_bAppDone)
      break;

    int time = ep::get_time();

    root->update(time - timeLast);
    root->render(0);

    // Swap Buffers.
    // Brings to the native display the current render surface.
    eglSwapBuffers(eglDisplay, eglSurface);
    if (!TestEGLError(hWnd, "eglSwapBuffers"))
      return false;

    // Managing the window messages
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    timeLast = time;
  }

  return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nCmdShow)
{
  HWND hWnd = 0;
  HDC  hDC = 0;

  EGLDisplay eglDisplay = 0;

  ep::set_loc(lpCmdLine);

  start_window(hInstance, hWnd, hDC, eglDisplay);

  ep::free_epod();

  // Step 9 - Terminate OpenGL ES and destroy the window (if present).
  // eglTerminate takes care of destroying any context or surface created
  // with this display, so we don't need to call eglDestroySurface or
  // eglDestroyContext here.

  eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglTerminate(eglDisplay);

  // Step 10 - Destroy the eglWindow.
  // Again, this is platform specific and delegated to a separate function.

  // Release the device context
  if (hDC) ReleaseDC(hWnd, hDC);

  // Destroy the eglWindow
  if (hWnd) DestroyWindow(hWnd);

  return 0;
}
