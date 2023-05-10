#pragma once

#include "pch.h"

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Text;
using namespace Windows::Foundation;

struct InitXamlEngine // a singleton which needs created once before using XAML
{
	InitXamlEngine() : winxamlmanager{(
			init_apartment(apartment_type::single_threaded),
			WindowsXamlManager::InitializeForCurrentThread() )}
	{
		// After destroying the WindowsXamlManager for a given thread, creating a new 
		// WindowsXamlManager on the same thread is not supported and will result in 
		// unpredictable behavior. - Microsoft

		static auto call_count{ 0 };
		assert(call_count == 0);
		++call_count;
	}

	~InitXamlEngine() { winxamlmanager.Close(); }
		
	InitXamlEngine(const InitXamlEngine&) = delete;
	InitXamlEngine(InitXamlEngine&&) = delete;
	InitXamlEngine& operator=(const InitXamlEngine&) = delete;
	InitXamlEngine& operator=(InitXamlEngine&&) = delete;

	const WindowsXamlManager winxamlmanager;
};

class Win32Host // a classic Win32 window to host a XAML island
{
friend class Win32XamlIsland;
public:
	auto HwndHost() const { return hWndHost; }
private:
	HWND hWndHost{ NULL };
};

class Win32XamlIsland : public Win32Host // the base class for a Win32 XAML island
{
public:

	Win32XamlIsland() = default;
	virtual ~Win32XamlIsland() { desktopSource.Close(); }
	Win32XamlIsland(const Win32XamlIsland&) = delete;
	Win32XamlIsland(Win32XamlIsland&&) = delete;
	Win32XamlIsland& operator=(const Win32XamlIsland&) = delete;
	Win32XamlIsland& operator=(Win32XamlIsland&&) = delete;

	auto AttachTo(HWND);
	auto HwndXamlIsland() const { return hWndXamlIsland; }
	//auto HwndHost() const { return hWndHost; }

private:

	virtual void DoInitXamlContent() = 0; // XAML controls initialization
	virtual UIElement DoTopXamlContainer() const = 0; // the control to be passed to desktopSource.Content

	DesktopWindowXamlSource desktopSource{};
	HWND hWndXamlIsland{ NULL };
	//HWND hWndHost{ NULL };

	// Declare XAML controls here (or under protected)
};

inline auto Win32XamlIsland::AttachTo(HWND hwnd) // Template Method
{
	// Make sure that your code calls the AttachToWindow method only once 
	// per DesktopWindowXamlSource object. Calling this method more than once 
	// for a DesktopWindowXamlSource object could result in a memory leak.
	// - Microsoft

	assert(hWndXamlIsland == NULL);

	assert(hwnd != NULL);
	hWndHost = hwnd;

	auto interop = desktopSource.as<IDesktopWindowXamlSourceNative>();
	check_hresult(interop->AttachToWindow(hwnd));
	interop->get_WindowHandle(&hWndXamlIsland);

	desktopSource.Content(DoTopXamlContainer());

	DoInitXamlContent();

	DoTopXamlContainer().UpdateLayout();
}

inline double RasterizationScale()
{
	auto desktopDc = GetDC(NULL);
	if (desktopDc == NULL) return 1.0;

	auto horizontalDPI = GetDeviceCaps(desktopDc, LOGPIXELSX);
	return double(horizontalDPI) * 25.0 / 24.0 / 100.0;
}