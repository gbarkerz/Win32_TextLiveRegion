// Win32_TextLiveRegion.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Win32_TextLiveRegion.h"

// This app is the default Win32 app created in VS 2019, plus the code snippets shown at
// https://docs.microsoft.com/en-us/accessibility-tools-docs/items/Win32/Text_LiveSetting.

#include <initguid.h>
#include "objbase.h"
#include "uiautomation.h"
IAccPropServices* _pAccPropServices = NULL;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32TEXTLIVEREGION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32TEXTLIVEREGION));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TEXTLIVEREGION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32TEXTLIVEREGION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HRESULT hr;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:

        // Run when the UI is created.
        hr = CoCreateInstance(
            CLSID_AccPropServices,
            nullptr,
            CLSCTX_INPROC,
            IID_PPV_ARGS(&_pAccPropServices));
        if (SUCCEEDED(hr))
        {
            // Set up the status label to be an assertive LiveRegion. The assertive property 
            // is used to request that the screen reader announces the update immediately. 
            // And alternative setting of polite requests that the screen reader completes 
            // any in-progress announcement before announcing the LiveRegion update.

            VARIANT var;
            var.vt = VT_I4;
            var.lVal = Assertive;

            hr = _pAccPropServices->SetHwndProp(
                GetDlgItem(hDlg, IDC_LABEL_STATUS),
                OBJID_CLIENT,
                CHILDID_SELF,
                LiveSetting_Property_GUID,
                var);
        }

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON_CHECKSTATUS)
        {
            SetDlgItemText(hDlg, IDC_LABEL_STATUS, L"Woohoo! Status is just fine.");

            // Run when the text on the label changes. Raise a UIA LiveRegionChanged 
            // event so that a screen reader is made aware of a change to the LiveRegion.
            // Make sure the updated text is set on the label before making this call.
            NotifyWinEvent(
                EVENT_OBJECT_LIVEREGIONCHANGED,
                GetDlgItem(hDlg, IDC_LABEL_STATUS),
                OBJID_CLIENT,
                CHILDID_SELF);
        }

        break;

    case WM_DESTROY:

        // Run when the UI is destroyed.
        if (_pAccPropServices != nullptr)
        {
            // We only added the one property to the hwnd.
            MSAAPROPID props[] = { LiveSetting_Property_GUID };
            hr = _pAccPropServices->ClearHwndProps(
                GetDlgItem(hDlg, IDC_LABEL_STATUS),
                OBJID_CLIENT,
                CHILDID_SELF,
                props,
                ARRAYSIZE(props));

            _pAccPropServices->Release();
            _pAccPropServices = NULL;
        }
    }
    return (INT_PTR)FALSE;
}
