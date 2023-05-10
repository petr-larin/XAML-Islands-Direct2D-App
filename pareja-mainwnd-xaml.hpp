#pragma once

#include "win32-xaml-island-base.hpp"
#include "pareja-common-xaml.hpp"


namespace Pareja
{

class ClrDlgXamlBase;

class MainWndXaml : public Win32XamlIsland
{

public:

	MainWndXaml() = default;
	auto RegisterClrDlg(const ClrDlgXamlBase& clr_dlg) { this->clr_dlg = &clr_dlg; }
	void OnHostWndResize() const { AdjustWndSize(); }
	void EnableControls(bool) const;
	void GetTargetRect(RECT& rc) const;
	
private:

	void DoInitXamlContent() override;
	UIElement DoTopXamlContainer() const override { return grid.as<UIElement>(); }
	void AdjustWndSize() const;

	auto ClrDlg() const
	{
		assert(clr_dlg != nullptr);
		return clr_dlg;
	}

	Grid grid{};

	TextBlock
		tb_hdr_c0{}, tb_hdr_c2{}, tb_hdr_c4{},
		tb_c0_r1{}, tb_c0_r2{}, tb_c0_r3{},
		tb_c2_r1{}, tb_c2_r2{}, tb_c2_r3{},
		tb_c4_r1{}, tb_c4_r2{};

	Slider 
		slider_c1_r1{}, slider_c1_r2{},
		slider_c3_r1{}, slider_c3_r2{},
		slider_c5_r1{}, slider_c5_r2{};

	ToggleSwitch ts1{}, ts2{};

	StackPanel sp_buttons{};

	Button bu_colors{}, bu_step{}, bu_run{};

	const ClrDlgXamlBase* clr_dlg{ nullptr };
};



}