/******************************************************************************/
/*!
\file           main.cpp
\author         Team Popplio
\author         Ng Jun Heng Shawn
\contribution   Shawn - 100%
\par            Course : CSD2401 / CSD2400
\par            Section : A
\date           2024/11/06
\brief
    This is the source file for the main application executable entry point.
    Loads the main DLL and runs the application.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include "resource.h" // icons

#include <windows.h>
#include <tchar.h>

#define PRE_DLL // without cppsharp binds
//#define POST_DLL // with cppsharp binds

typedef int(*AppFunc)(); // pointer to app func

#ifdef _DEBUG // use main() if debug editor is used

/*
*   Entry point for the application.
*   Uses main() if the editor is used. (debug)
*   Uses WinMain() if using the release editor / game build
*   @returns int | result code 0 if no errors
*/
int main()

#else // use WinMain() if is game release / editor release

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // signal that the application should terminate
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        static_cast<void>(hdc);
        // perform custom painting here
        EndPaint(hwnd, &ps);
    }
    return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#endif // _DEBUG
{
#ifndef _DEBUG
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"PopplioWindowClass";
    static_cast<void>(hPrevInstance);
    static_cast<void>(lpCmdLine);
    static_cast<void>(nCmdShow);

    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(GLFW_ICON));  // Large icon
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(GLFW_ICON)); // Small icon

    RegisterClassEx(&wc);
#endif

    // Specify the path to the DLL
#ifdef PRE_DLL
    HMODULE dll = LoadLibrary(TEXT("Team_Popplio_PreDLL.dll")); // without cppsharp binds
#elif defined POST_DLL
    HMODULE dll = LoadLibrary(TEXT("Team_Popplio_EngineDLL.dll")); // with cppsharp binds
#endif // PRE_DLL

    if (!dll) return -1; // error out

    // Get the function pointer
    AppFunc app = reinterpret_cast<AppFunc>(GetProcAddress(dll, "App"));

    int result = app(); // run the app

    FreeLibrary(dll); // free the dll

    return result;
}