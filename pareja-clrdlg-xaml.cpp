#include "pareja-mainwnd-xaml.hpp"
#include "pareja-clrdlg-xaml.hpp"

using namespace Pareja;

void ClrDlgXamlBase::DoInitXamlContent()
{
    constexpr auto
        wnd_width = 400.0, wnd_height = 350.0,
        bu_width = 77.0,
        bu_margin = 10.0,
        bu_interval = 9.0;

    sp_buttons.Orientation(Orientation::Horizontal);
    sp_buttons.HorizontalAlignment(HorizontalAlignment::Left);
    sp_buttons.VerticalAlignment(VerticalAlignment::Top);
    sp_buttons.Width(wnd_width);
    sp_buttons.Height(wnd_height);

    //SolidColorBrush br;
    //br.Color(Colors::Azure());
    //sp_buttons.Background(br);

    auto init_bu = [&, this](Button& bu, const wchar_t* c, double lm) // can be static - doesn't matter
    {
        Thickness margin = { lm, 0.0, 0.0, bu_margin };
        //bu.HorizontalAlignment(HorizontalAlignment::Center);
        bu.VerticalAlignment(VerticalAlignment::Bottom);
        bu.Margin(margin);
        bu.Content(box_value(c));
        bu.Width(bu_width);
        bu.CornerRadius(corner_radius);
        //bu.Style(BuAccentedStyle());
        sp_buttons.Children().Append(bu);
    };

    init_bu(bu_ok, L"OK", wnd_width - bu_margin - 2 * bu_width - bu_interval);
    init_bu(bu_cancel, L"Cancel", bu_interval);

    auto on_bu_ok_clicked = [&](IInspectable const& sender, RoutedEventArgs const& args)
    {
        ShowHost(false);
    };

    auto on_bu_cancel_clicked = [&](IInspectable const& sender, RoutedEventArgs const& args)
    {
        ShowHost(false);
    };

    bu_ok.Click(on_bu_ok_clicked);
    bu_cancel.Click(on_bu_cancel_clicked);

    auto on_xaml_root_changed = [=](XamlRoot const& s, XamlRootChangedEventArgs const&)
    {
        AdjustWndSize();
    };
        
    assert(sp_buttons.XamlRoot());
    sp_buttons.XamlRoot().Changed(on_xaml_root_changed);
}

LRESULT CALLBACK ClrDlgXamlBase::ClrDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {

    case WM_CREATE:
    {
        SetWindowLongPtrW(hwnd, GWLP_USERDATA,
            LONG_PTR(LPCREATESTRUCTW(lParam)->lpCreateParams));

        return 0;
    }

    case WM_CLOSE:
    {
        ClrDlgXamlBase* This = (ClrDlgXamlBase*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
        assert(This != nullptr);
        assert(This->HwndHost() != NULL);

        EnableWindow(This->HwndHost(), TRUE);

        return 0;
    }

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

void ClrDlgXamlPopup::AdjustWndSize() const
{
    auto xr = DoTopXamlContainer().XamlRoot();

    if (xr)
    {
        const auto
            factor = RasterizationScale(),
            width = sp_buttons.Width(),
            height = sp_buttons.Height();

        SetWindowPos(
            HwndXamlIsland(), 0,
            0, 0, 
            int(width * factor),
            int(height * factor),
            SWP_SHOWWINDOW);

        RECT rc_host_cli{}, rc_host_wnd{}, rc_xi{};
        GetClientRect(HwndHost(), &rc_host_cli);
        GetWindowRect(HwndHost(), &rc_host_wnd);
        GetWindowRect(HwndXamlIsland(), &rc_xi);

        SetWindowPos(
            HwndHost(), 0,
            0, 0,
            rc_xi.right - rc_xi.left + 
                rc_host_wnd.right - rc_host_wnd.left - rc_host_cli.right + rc_host_cli.left,
            rc_xi.bottom - rc_xi.top +
                rc_host_wnd.bottom - rc_host_wnd.top - rc_host_cli.bottom + rc_host_cli.top,
            SWP_NOACTIVATE | SWP_NOMOVE);
    }
}

void ClrDlgXamlPopup::ShowHost(bool show) const
{
    if (show) 
    {
        EnableWindow(main_wnd.HwndHost(), FALSE);

        RECT rc{};
        GetWindowRect(main_wnd.HwndHost(), &rc);
        auto dlgx = (rc.left + rc.right) / 2, dlgy = rc.top + (rc.bottom - rc.top) / 3;

        SetWindowPos(HwndHost(), NULL, dlgx, dlgy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else 
    {
        EnableWindow(main_wnd.HwndHost(), TRUE);
        ShowWindow(HwndHost(), SW_HIDE);
    }
}

void ClrDlgXamlPopup::CreateHostAndAttach(HINSTANCE hinst, HWND parent)
{
    const wchar_t wc_name[] = L"Pareja::ClrDlgPopup";

    WNDCLASSEXW wc{};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = ClrDlgProc;
    wc.hInstance = hinst;
    wc.lpszClassName = wc_name;
    //wc.hbrBackground = HBRUSH(COLOR_ACTIVECAPTION + 1);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    RegisterClassExW(&wc);

    const auto clr_dlg = CreateWindowExW(
        0,
        wc_name,
        L": : Pareja : : Choose colors",
        WS_OVERLAPPED,
        CW_USEDEFAULT, CW_USEDEFAULT, 10, 10,
        parent, // NULL == no flashing animation
        NULL,
        hinst,
        this
    );

    assert(clr_dlg != NULL);

    AttachTo(clr_dlg);
}


void ClrDlgXamlChild::AdjustWndSize() const
{
    auto xr = DoTopXamlContainer().XamlRoot();

    if (xr)
    {
        auto
            factor = RasterizationScale(),
            width = sp_buttons.Width(),
            height = sp_buttons.Height();

        RECT rct{};
        main_wnd.GetTargetRect(rct);

        SetWindowPos(
            HwndXamlIsland(), 0,
            0, 0,
            int(width * factor),
            rct.bottom - rct.top,
            SWP_SHOWWINDOW);

        SetWindowPos(
            HwndHost(), 0,
            0, rct.top,
            int(width * factor),
            rct.bottom - rct.top,
            SWP_NOACTIVATE);
    }
}

void ClrDlgXamlChild::ShowHost(bool show) const
{
    if (show)
    {
        main_wnd.EnableControls(false);
        AnimateWindow(HwndHost(), 200, AW_HOR_POSITIVE);
    }
    else
    {
        main_wnd.EnableControls(true);
        AnimateWindow(HwndHost(), 125, AW_HIDE | AW_HOR_NEGATIVE);
    }
}

void ClrDlgXamlChild::CreateHostAndAttach(HINSTANCE hinst, HWND parent)
{
    const wchar_t wc_name[] = L"Pareja::ClrDlgChild";

    WNDCLASSEXW wc{};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = ClrDlgProc;
    //wc.hInstance = hinst;
    wc.lpszClassName = wc_name;
    wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    RegisterClassExW(&wc);

    auto clr_dlg = CreateWindowExW(
        0,
        wc_name,
        nullptr,
        WS_CHILD,
        0, 0, 10, 10,
        parent,
        NULL,
        0,//hinst,
        this
    );

    assert(clr_dlg != NULL);

    AttachTo(clr_dlg);
}
