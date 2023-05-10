#include "pareja-mainwnd-xaml.hpp"
#include "pareja-clrdlg-xaml.hpp"

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::Foundation;

using namespace Pareja;

InitXamlEngine ixe{};
MainWndXaml mainwnd_xaml{};

// Choose one impl:
// ClrDlgXamlPopup clrdlg_xaml{};
 ClrDlgXamlChild clrdlg_xaml{};

HWND CreateMainWnd(HINSTANCE, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK wWinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow)
{
    mainwnd_xaml.RegisterClrDlg(clrdlg_xaml);
    clrdlg_xaml.RegisterMainWnd(mainwnd_xaml);

    auto main_wnd = CreateMainWnd(hinst, nCmdShow);
    assert(main_wnd != NULL);
    mainwnd_xaml.AttachTo(main_wnd);

    clrdlg_xaml.CreateHostAndAttach(hinst, main_wnd);

    ShowWindow(main_wnd, nCmdShow);
    UpdateWindow(main_wnd);

    MSG msg{};
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

HWND CreateMainWnd(HINSTANCE hinst, int nCmdShow)
{
    const wchar_t wc_name[] = L"Pareja::MainWnd";

    WNDCLASSEXW wc{};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = MainWndProc;
    //wc.hInstance = hinst;
    wc.lpszClassName = wc_name;
    wc.hbrBackground = HBRUSH(COLOR_ACTIVECAPTION + 1);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    //wc.hIconSm = LoadIcon(windowClass.hInstance, IDI_APPLICATION);

    RegisterClassExW(&wc);

    return CreateWindowExW(
        WS_EX_DLGMODALFRAME, // this is to remove the small icon from the title bar
        wc_name,
        L": : Pareja",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        0,//hinst,
        NULL
    );
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    /*case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        wchar_t greeting[] = L"Hello World in Win32!";

        hdc = BeginPaint(hwnd, &ps);
        TextOut(hdc, 300, 500, greeting, (int)wcslen(greeting));
        EndPaint(hwnd, &ps);

        break;
    }*/

    case WM_DESTROY:
    
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
    {
        mainwnd_xaml.OnHostWndResize();
        clrdlg_xaml.OnMainWndResize();

        return 0;
    }

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    return 0;
}

