#pragma once

#include "win32-xaml-island-base.hpp"
#include "pareja-common-xaml.hpp"

namespace Pareja
{

template<typename ClrDlgT> class MainWndXaml;

class ClrDlgXamlBase : public Win32XamlIsland
{
public:

	ClrDlgXamlBase() = default;

	virtual void ShowHost(bool) const = 0;
	virtual void CreateHostAndAttach(HINSTANCE, HWND parent) = 0;
	virtual void OnMainWndResize() const {};

protected:

	UIElement DoTopXamlContainer() const final { return sp_buttons.as<UIElement>(); }
	static LRESULT CALLBACK ClrDlgProc(HWND, UINT, WPARAM, LPARAM);

	StackPanel sp_buttons{};
	Button bu_ok{}, bu_cancel{};

private:

	void DoInitXamlContent() final;
	virtual void AdjustWndSize() const = 0;
};

class ClrDlgXamlPopup : public ClrDlgXamlBase
{
public:

	ClrDlgXamlPopup(MainWndXaml<ClrDlgXamlPopup>& main_wnd) : main_wnd{ main_wnd } {}
	void ShowHost(bool) const final;
	void CreateHostAndAttach(HINSTANCE, HWND) final;

private:

	//MainWndXaml<ClrDlgXamlPopup>& MainWnd() const {	return main_wnd; }
	void AdjustWndSize() const final;
	MainWndXaml<ClrDlgXamlPopup>& main_wnd;
};


class ClrDlgXamlChild : public ClrDlgXamlBase
{
public:

	ClrDlgXamlChild(MainWndXaml<ClrDlgXamlChild>& main_wnd) : main_wnd{ main_wnd } {}
	void ShowHost(bool) const final;
	void CreateHostAndAttach(HINSTANCE, HWND) final;
	virtual void OnMainWndResize() const final { AdjustWndSize(); }

private:

	//MainWndXaml<ClrDlgXamlChild>& MainWnd() const { return main_wnd; }
	void AdjustWndSize() const final;
	MainWndXaml<ClrDlgXamlChild>& main_wnd;
};


}