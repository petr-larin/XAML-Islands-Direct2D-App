#pragma once

#include "win32-xaml-island-base.hpp"
#include "pareja-common-xaml.hpp"

namespace Pareja
{

class MainWndXaml;

class ClrDlgXamlBase : public Win32XamlIsland
{
public:

	ClrDlgXamlBase() = default;
	auto RegisterMainWnd(const MainWndXaml& main_wnd) { this->main_wnd = &main_wnd; }
	virtual void ShowHost(bool) const = 0;
	virtual void CreateHostAndAttach(HINSTANCE, HWND parent) = 0;
	virtual void OnMainWndResize() const {};

protected:

	UIElement DoTopXamlContainer() const final { return sp_buttons.as<UIElement>(); }

	const MainWndXaml* MainWnd() const
	{
		assert(main_wnd != nullptr);
		return main_wnd;
	}

	static LRESULT CALLBACK ClrDlgProc(HWND, UINT, WPARAM, LPARAM);

//private:

	StackPanel sp_buttons{};
	Button bu_ok{}, bu_cancel{};
	const MainWndXaml* main_wnd{ nullptr };

private:

	void DoInitXamlContent() final;
	virtual void AdjustWndSize() const = 0;
};

class ClrDlgXamlPopup : public ClrDlgXamlBase
{
public:

	ClrDlgXamlPopup() = default;
	void ShowHost(bool) const final;
	void CreateHostAndAttach(HINSTANCE, HWND) final;

private:

	void AdjustWndSize() const final;
};

class ClrDlgXamlChild : public ClrDlgXamlBase
{
public:

	ClrDlgXamlChild() = default;
	void ShowHost(bool) const final;
	void CreateHostAndAttach(HINSTANCE, HWND) final;
	virtual void OnMainWndResize() const final { AdjustWndSize(); }

private:

	void AdjustWndSize() const final;
};


}