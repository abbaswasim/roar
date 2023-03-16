// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the 'Software'),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version: 1.0.0

#pragma once

#include "foundation/rormacros.hpp"
#include <imgui/imgui.h>

namespace ror
{

// Copied from ImGuizmo's version of new frame
FORCE_INLINE auto imgui_draw_list()
{
	const ImU32 flags = ImGuiWindowFlags_NoTitleBar |
	                    ImGuiWindowFlags_NoResize |
	                    ImGuiWindowFlags_NoScrollbar |
	                    ImGuiWindowFlags_NoInputs |
	                    ImGuiWindowFlags_NoSavedSettings |
	                    ImGuiWindowFlags_NoFocusOnAppearing |
	                    ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGuiIO &io = ImGui::GetIO();
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("empty_list", nullptr, flags);
	auto draw_list = ImGui::GetWindowDrawList();
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);

	return draw_list;
}

const ImU32 imgui_red          = ImGui::ColorConvertFloat4ToU32({1.0f, 0.0f, 0.0f, 1.0f});
const ImU32 imgui_green        = ImGui::ColorConvertFloat4ToU32({0.0f, 1.0f, 0.0f, 1.0f});
const ImU32 imgui_blue         = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 1.0f, 1.0f});
const ImU32 imgui_white        = ImGui::ColorConvertFloat4ToU32({1.0f, 1.0f, 1.0f, 1.0f});
const ImU32 imgui_black        = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.0f, 1.0f});
const ImU32 imgui_dim_red      = ImGui::ColorConvertFloat4ToU32({0.6f, 0.0f, 0.0f, 0.5f});
const ImU32 imgui_dim_green    = ImGui::ColorConvertFloat4ToU32({0.0f, 0.6f, 0.0f, 0.5f});
const ImU32 imgui_dim_blue     = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.6f, 0.5f});
const ImU32 imgui_dim_white    = ImGui::ColorConvertFloat4ToU32({1.0f, 1.0f, 1.0f, 0.5f});
const ImU32 imgui_dim_black    = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.0f, 0.5f});
const ImU32 imgui_dimmer_red   = ImGui::ColorConvertFloat4ToU32({1.0f, 0.0f, 0.0f, 0.3f});
const ImU32 imgui_dimmer_green = ImGui::ColorConvertFloat4ToU32({0.0f, 1.0f, 0.0f, 0.3f});
const ImU32 imgui_dimmer_blue  = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 1.0f, 0.3f});
const ImU32 imgui_dimmer_white = ImGui::ColorConvertFloat4ToU32({0.5f, 0.5f, 0.5f, 0.3f});
const ImU32 imgui_dimmer_black = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.0f, 0.3f});

}        // namespace ror
