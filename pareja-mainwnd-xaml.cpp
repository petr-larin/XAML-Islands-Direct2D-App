#include "pareja-mainwnd-xaml.hpp"
#include "pareja-clrdlg-xaml.hpp"

namespace Pareja
{

template<typename ClrDlgT>
void MainWndXaml<ClrDlgT>::DoInitXamlContent()
{
    // ----------------- [ The Grid - top level container ] ----------------- //

    //SolidColorBrush br;
    //br.Color(Colors::LightGray());
    //grid.Background(br);
    //grid.RequestedTheme(ElementTheme::Dark);

    constexpr auto
        num_columns = 6, num_rows = 4;

    constexpr auto
        column_width_odd = 120.0, column_width_even = 150.0;

    constexpr double

        column_def[num_columns] = {
            column_width_odd, column_width_even,
            column_width_odd, column_width_even,
            column_width_odd, column_width_even + 9.0 },// + 5.0},

        row_def[num_rows] = { 30.0, 30.0, 30.0, 50.0 };

    double grid_height = 0.0;

    ColumnDefinition cd[num_columns]{};
    RowDefinition rd[num_rows]{};

    for (auto ind = 0; ind < num_columns; ++ind)
    {
        cd[ind].Width(GridLength{ column_def[ind], GridUnitType::Pixel });
        grid.ColumnDefinitions().Append(cd[ind]);
    }

    for (auto ind = 0; ind < num_rows; ++ind)
    {
        rd[ind].Height(GridLength{ row_def[ind], GridUnitType::Pixel });
        grid.RowDefinitions().Append(rd[ind]);
        grid_height += row_def[ind];
    }

    grid.Height(grid_height);
    grid.HorizontalAlignment(HorizontalAlignment::Left);
    grid.VerticalAlignment(VerticalAlignment::Top);

    auto on_xaml_root_changed = [=](XamlRoot const& s, XamlRootChangedEventArgs const&)
    {
        AdjustWndSize();
    };

    assert(grid.XamlRoot());
    grid.XamlRoot().Changed(on_xaml_root_changed);

    // -------------------- [ Text blocks - headers ] -------------------- //

    auto init_header_tb = [=](TextBlock& tb, const wchar_t* t, int32_t col)
    {
        constexpr Thickness margin = { 0.0, 5.0, 0.0, 5.0 };
        tb.Text(t);
        tb.HorizontalAlignment(HorizontalAlignment::Right);
        tb.VerticalAlignment(VerticalAlignment::Center);
        tb.Margin(margin);
        tb.FontWeight(FontWeights::SemiBold()); // BodyStrongTextBlockStyle
        Grid::SetColumn(tb, col);
        Grid::SetRow(tb, 0);
        grid.Children().Append(tb);
    };

    init_header_tb(tb_hdr_c0, L"Velocity", 0);
    init_header_tb(tb_hdr_c2, L"Size", 2);
    init_header_tb(tb_hdr_c4, L"Controls", 4);
    
    // ------------------------ [ Text blocks ] ------------------------ //

    auto init_tb = [=](TextBlock& tb, const wchar_t* t, int32_t col, int32_t row, double bm = -5.0)
    {
        Thickness margin = { 0.0, 0.0, 0.0, bm };
        tb.Text(t);
        tb.HorizontalAlignment(HorizontalAlignment::Right);
        tb.VerticalAlignment(VerticalAlignment::Center);
        tb.Margin(margin);
        Grid::SetColumn(tb, col);
        Grid::SetRow(tb, row);
        grid.Children().Append(tb);
    };

    init_tb(tb_c0_r1, L"Searcher One", 0, 1);
    init_tb(tb_c0_r2, L"Searcher Two", 0, 2);
    init_tb(tb_c0_r3, L"Equal velocities", 0, 3, 0.0);
    init_tb(tb_c2_r1, L"Searcher One", 2, 1);
    init_tb(tb_c2_r2, L"Searcher Two", 2, 2);
    init_tb(tb_c2_r3, L"Equal sizes", 2, 3, 0.0);
    init_tb(tb_c4_r1, L"Screen speed", 4, 1);
    init_tb(tb_c4_r2, L"Step size", 4, 2);

    // -------------------------- [ Sliders ] -------------------------- //

    constexpr auto
        v_min = 1.0, v_max = 25.0,
        s_min = 1.0, s_max = 10.0,
        scr_speed_min = 1.0, scr_speed_max = 10.0,
        step_min = 1.0, step_max = 10.0;

    auto init_sl = [=](Slider& sl, double rmin, double rmax, int32_t col, int32_t row)
    {
        constexpr double lm = 20.0;
        constexpr Thickness margin = { lm, 0.0, 0.0, -5.0 };
        sl.Width(column_width_even - lm);
        sl.HorizontalAlignment(HorizontalAlignment::Left);
        sl.VerticalAlignment(VerticalAlignment::Center);
        sl.Margin(margin);
        sl.Minimum(rmin);
        sl.Maximum(rmax);
        Grid::SetColumn(sl, col);
        Grid::SetRow(sl, row);
        grid.Children().Append(sl);
    };

    init_sl(slider_c1_r1, v_min, v_max, 1, 1);
    init_sl(slider_c1_r2, v_min, v_max, 1, 2);
    init_sl(slider_c3_r1, s_min, s_max, 3, 1);
    init_sl(slider_c3_r2, s_min, s_max, 3, 2);

    init_sl(slider_c5_r1, scr_speed_min, scr_speed_max, 5, 1);
    //slider_c5_r1.StepFrequency(0.1);
    //slider_c5_r1.SnapsTo(SliderSnapsTo::StepValues);

    init_sl(slider_c5_r2, step_min, step_max, 5, 2);
    //slider_c5_r2.StepFrequency(0.1);
    //slider_c5_r2.SnapsTo(SliderSnapsTo::StepValues);

    auto on_slider_c1_r1_changed = [=](IInspectable const&, RangeBaseValueChangedEventArgs const&)
    {
        if (ts1.IsOn()) slider_c1_r2.Value(slider_c1_r1.Value());
    };

    auto on_slider_c1_r2_changed = [=](IInspectable const&, RangeBaseValueChangedEventArgs const&)
    {
        if (ts1.IsOn()) slider_c1_r1.Value(slider_c1_r2.Value());
    };

    auto on_slider_c3_r1_changed = [=](IInspectable const&, RangeBaseValueChangedEventArgs const&)
    {
        if (ts2.IsOn()) slider_c3_r2.Value(slider_c3_r1.Value());
    };

    auto on_slider_c3_r2_changed = [=](IInspectable const&, RangeBaseValueChangedEventArgs const&)
    {
        if (ts2.IsOn()) slider_c3_r1.Value(slider_c3_r2.Value());
    };

    slider_c1_r1.ValueChanged(on_slider_c1_r1_changed);
    slider_c1_r2.ValueChanged(on_slider_c1_r2_changed);
    slider_c3_r1.ValueChanged(on_slider_c3_r1_changed);
    slider_c3_r2.ValueChanged(on_slider_c3_r2_changed);

    // ---------------------- [ Toggle switches ] ---------------------- //

    auto init_ts = [=](ToggleSwitch& ts, int32_t col)
    {
        constexpr Thickness margin = { 20.0, 2.0, 0.0, 0.0 };
        ts.HorizontalAlignment(HorizontalAlignment::Left);
        ts.VerticalAlignment(VerticalAlignment::Center);
        ts.Margin(margin);
        ts.OffContent(box_value(L"Off"));
        ts.OnContent(box_value(L"On"));
        Grid::SetColumn(ts, col);
        Grid::SetRow(ts, 3);
        grid.Children().Append(ts);
    };

    init_ts(ts1, 1);
    init_ts(ts2, 3);

    auto on_ts1_toggled = [=](IInspectable const&, RoutedEventArgs const&)
    {
        if (ts1.IsOn()) slider_c1_r2.Value(slider_c1_r1.Value());
    };

    auto on_ts2_toggled = [=](IInspectable const&, RoutedEventArgs const&)
    {
        if (ts2.IsOn()) slider_c3_r2.Value(slider_c3_r1.Value());
    };

    ts1.Toggled(on_ts1_toggled);
    ts2.Toggled(on_ts2_toggled);

    // ------------------- [ Stack panel for buttons ] ------------------- //

    sp_buttons.Orientation(Orientation::Horizontal);
    sp_buttons.HorizontalAlignment(HorizontalAlignment::Left);
    sp_buttons.VerticalAlignment(VerticalAlignment::Top);
    Grid::SetColumn(sp_buttons, 4);
    Grid::SetColumnSpan(sp_buttons, 2);
    Grid::SetRow(sp_buttons, 3);
    grid.Children().Append(sp_buttons);

    // --------------------------- [ Buttons ] --------------------------- //

    auto init_bu = [=](Button& bu, const wchar_t* c, double lm)
    {
        Thickness margin = { lm, 13.0, 0.0, 5.0 };
        //bu.HorizontalAlignment(HorizontalAlignment::Left);
        bu.VerticalAlignment(VerticalAlignment::Center);
        bu.Margin(margin);
        bu.Content(box_value(c));
        bu.Width(77.0);
        bu.CornerRadius(corner_radius);
        //bu.Style(BuAccentedStyle());
        sp_buttons.Children().Append(bu);
    };

    init_bu(bu_colors, L"Colors...", 30.0);
    init_bu(bu_step, L"Step", 9.0);
    init_bu(bu_run, L"Run", 9.0);

    bu_step.Style(BuAccentedStyle());
    bu_run.Style(BuAccentedStyle());

    auto on_bu_colors_clicked = [=](IInspectable const&, RoutedEventArgs const&)
    {
        ClrDlg()->ShowHost(true);
    };

    auto on_bu_step_clicked = [=](IInspectable const&, RoutedEventArgs const&)
    {
        MessageBeep(MB_ICONSTOP);
    };

    auto on_bu_run_clicked = [=](IInspectable const&, RoutedEventArgs const&)
    {
        static bool f{};
        bu_run.Content(box_value((f = !f) ? L"Pause" : L"Run"));
    };

    bu_colors.Click(on_bu_colors_clicked);
    bu_step.Click(on_bu_step_clicked);
    bu_run.Click(on_bu_run_clicked);
}

template<typename ClrDlgT>
void MainWndXaml<ClrDlgT>::AdjustWndSize() const
{
    auto xr = DoTopXamlContainer().XamlRoot();

    if (xr)
    {
        auto factor = RasterizationScale();

        RECT rc{};
        GetClientRect(HwndHost(), &rc);

        auto height = grid.Height();
        height = std::min(height * factor, double(rc.bottom - rc.top));

        SetWindowPos(HwndXamlIsland(), 0, 0, 0, rc.right - rc.left, int(height), SWP_SHOWWINDOW);
    }
}

template<typename ClrDlgT>
void MainWndXaml<ClrDlgT>::EnableControls(bool e) const
{
    slider_c1_r1.IsEnabled(e);
    slider_c1_r2.IsEnabled(e);
    slider_c3_r1.IsEnabled(e);
    slider_c3_r2.IsEnabled(e);
    slider_c5_r1.IsEnabled(e);
    slider_c5_r2.IsEnabled(e);

    ts1.IsEnabled(e);
    ts2.IsEnabled(e);

    bu_colors.IsEnabled(e);
    bu_step.IsEnabled(e);
    bu_run.IsEnabled(e);
}

template<typename ClrDlgT>
void MainWndXaml<ClrDlgT>::GetTargetRect(RECT& rct) const
{
    auto factor = RasterizationScale();

    RECT rcc{};
    GetClientRect(HwndHost(), &rcc);

    auto height = grid.Height();
    height = std::min(height * factor, double(rcc.bottom - rcc.top));

    rct.left = 0;
    rct.top = LONG(height + 0.5);
    rct.right = rcc.right;
    rct.bottom = rcc.bottom;
}

}