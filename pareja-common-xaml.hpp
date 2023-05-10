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

namespace Pareja
{
	constexpr CornerRadius corner_radius = { 4.0, 4.0, 4.0, 4.0 };
	
	inline const auto BuAccentedStyle()
	{
		return
			Application::Current().Resources().Lookup(
				box_value(L"AccentButtonStyle")).as<Style>();
	}
}