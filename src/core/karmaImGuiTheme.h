#pragma once

#include "BaseTheme.h"

class karmaImGuiTheme
	: public ofxImGui::BaseTheme
{
public:
	karmaImGuiTheme()
	{
		col_main_text = ofColor::white;
		col_main_head = ofColor::blue;
		col_main_area = ofColor::gray;
		col_win_popup = ofColor::yellow;
		col_win_backg = ofColor::black;
		
		col_main_text = ofColor::fromHex(0xdbede2);
		col_main_head = ofColor::fromHex(0xd12d49);
		col_main_area = ofColor::fromHex(0x333844);
		col_win_popup = ofColor::fromHex(0x77c4d3);
		col_win_backg = ofColor::fromHex(0x212121);//0x21232b);
	}
	
//	void updateColors(){
//		ImGuiStyle* style = &ImGui::GetStyle();
//		
//		style->Colors[ImGuiCol_Text] = ImVec4(col_main_text, 1.00f);
//		style->Colors[ImGuiCol_TextDisabled] = ImVec4(col_main_text, 0.58f);
//		style->Colors[ImGuiCol_WindowBg] = ImVec4(col_win_backg, 0.70f);
//		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(col_main_area, 0.58f);
//		style->Colors[ImGuiCol_Border] = ImVec4(col_win_backg, 0.00f);
//		style->Colors[ImGuiCol_BorderShadow] = ImVec4(col_win_backg, 0.00f);
//		style->Colors[ImGuiCol_FrameBg] = ImVec4(col_main_area, 1.00f);
//		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main_head, 0.78f);
//		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_TitleBg] = ImVec4(col_main_area, 1.00f);
//		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main_area, 0.75f);
//		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(col_main_area, 0.47f);
//		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_main_area, 1.00f);
//		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_win_popup, 0.21f);
//		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main_head, 0.78f);
//		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_ComboBg] = ImVec4(col_main_area, 1.00f);
//		style->Colors[ImGuiCol_CheckMark] = ImVec4(col_main_head, 0.80f);
//		style->Colors[ImGuiCol_SliderGrab] = ImVec4(col_win_popup, 0.14f);
//		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_Button] = ImVec4(col_win_popup, 0.14f);
//		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main_head, 0.86f);
//		style->Colors[ImGuiCol_ButtonActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_Header] = ImVec4(col_main_head, 0.76f);
//		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main_head, 0.86f);
//		style->Colors[ImGuiCol_HeaderActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_Column] = ImVec4(col_win_popup, 0.32f);
//		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(col_main_head, 0.78f);
//		style->Colors[ImGuiCol_ColumnActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(col_win_popup, 0.04f);
//		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main_head, 0.78f);
//		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_CloseButton] = ImVec4(col_main_text, 0.16f);
//		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_main_text, 0.39f);
//		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_main_text, 1.00f);
//		style->Colors[ImGuiCol_PlotLines] = ImVec4(col_main_text, 0.63f);
//		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(col_main_text, 0.63f);
//		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main_head, 1.00f);
//		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main_head, 0.43f);
//		style->Colors[ImGuiCol_PopupBg] = ImVec4(col_win_backg, 0.92f);
//		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(col_main_area, 0.73f);
//	}
};
