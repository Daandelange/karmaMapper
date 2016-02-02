//
//  karmaConsole.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 02/02/2016.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

// This class is based on Omar Cornut's imgui_demo.cpp console struct
// https://github.com/ocornut/imgui/blob/bf6f6ba858170c321c622856d645ccfe84b7bc36/imgui_demo.cpp

#include "karmaConsole.h"

karmaConsole::karmaConsole() {
	ClearLog();
	HistoryPos = -1;
	Commands.push_back("HELP");
	Commands.push_back("HISTORY");
	Commands.push_back("CLEAR");
	Commands.push_back("CLASSIFY");  // "classify" is here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
}

karmaConsole::~karmaConsole() {
	ClearLog();
	for (int i = 0; i < Items.Size; i++)
		free(History[i]);
}

void karmaConsole::ClearLog() {
	for (int i = 0; i < Items.Size; i++)
		free(Items[i]);
	Items.clear();
	ScrollToBottom = true;
}

void karmaConsole::AddLog(const char* fmt, ...){
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf)-1] = 0;
	va_end(args);
	Items.push_back(strdup(buf));
	ScrollToBottom = true;
}

void karmaConsole::Draw(const char* title, bool& opened){
	ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin(title, &opened))
	{
		ImGui::End();
		return;
	}
	
	ImGui::TextWrapped("Welcome to the console! :)");
	ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

	ImGui::Separator();
	
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
	static ImGuiTextFilter filter;
	filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	ImGui::PopStyleVar();
	ImGui::Separator();
	
	// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
	// NB- if you have thousands of entries this approach may be too inefficient. You can seek and display only the lines that are visible - CalcListClipping() is a helper to compute this information.
	// If your items are of variable size you may want to implement code similar to what CalcListClipping() does. Or split your data into fixed height items to allow random-seeking into your list.
	ImGui::BeginChild("ScrollingRegion", ImVec2(0,-ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
	for (int i = 0; i < Items.Size; i++)
	{
		const char* item = Items[i];
		if (!filter.PassFilter(item))
			continue;
		ImVec4 col = ImColor(255,255,255); // A better implementation may store a type per-item. For the sample let's just parse the text.
		if (strstr(item, "[error]")) col = ImColor(255,100,100);
		else if (strncmp(item, "# ", 2) == 0) col = ImColor(255,200,150);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item);
		ImGui::PopStyleColor();
	}
	if (ScrollToBottom)
		ImGui::SetScrollHere();
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();
	
	// Command-line
	if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
	{
		char* input_end = InputBuf+strlen(InputBuf);
		while (input_end > InputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
		if (InputBuf[0])
			ExecCommand(InputBuf);
		strcpy(InputBuf, "");
	}
	
	// Demonstrate keeping auto focus on the input box
	if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
		ImGui::SetKeyboardFocusHere(-1); // Auto focus
	
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear")) ClearLog();
	
	ImGui::End();
}

void karmaConsole::ExecCommand(const char* command_line) {
	AddLog("# %s\n", command_line);
	
	// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
	HistoryPos = -1;
	for (int i = History.Size-1; i >= 0; i--)
		if (Stricmp(History[i], command_line) == 0)
		{
			free(History[i]);
			History.erase(History.begin() + i);
			break;
		}
	History.push_back(strdup(command_line));
	
	// Process command
	if (Stricmp(command_line, "CLEAR") == 0)
	{
		ClearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0)
	{
		AddLog("Commands:");
		for (int i = 0; i < Commands.Size; i++)
			AddLog("- %s", Commands[i]);
	}
	else if (Stricmp(command_line, "HISTORY") == 0)
	{
		for (int i = History.Size >= 10 ? History.Size - 10 : 0; i < History.Size; i++)
			AddLog("%3d: %s\n", i, History[i]);
	}
	else
	{
		AddLog("Unknown command: '%s'\n", command_line);
	}
}

shared_ptr<karmaConsoleChannel> karmaConsoleChannel::logger;
karmaConsoleChannel *karmaConsoleChannel::logPtr = NULL;

shared_ptr<karmaConsoleChannel> &karmaConsoleChannel::getLogger() {
	
	if(logPtr==NULL) {
		logPtr = new karmaConsoleChannel( );
		logger = shared_ptr<karmaConsoleChannel>(logPtr);
	}

	return logger;
}

karmaConsoleChannel::karmaConsoleChannel() {
	
	bLogToConsole = true;
	bLogToScreen = true;
}

karmaConsoleChannel::~karmaConsoleChannel() {
	
}

void karmaConsoleChannel::log(ofLogLevel level, const string & module, const string & message) {
	
	if(bLogToConsole) consoleLogger.log(level, module, message);
	
	if(bLogToScreen) console.AddLog( message.c_str() );
	
}

void karmaConsoleChannel::log(ofLogLevel logLevel, const string & module, const char* format, ...) {
	va_list args;
	va_start(args, format);
	log(logLevel, module, format, args);
	va_end(args);
}


void karmaConsoleChannel::log(ofLogLevel logLevel, const string & module, const char* format, va_list args) {
	
	if(bLogToConsole) consoleLogger.log(logLevel, module, format, args);
	if(bLogToScreen) console.AddLog( format, args );
	
}

void karmaConsoleChannel::drawImGui(const string& _title, bool& _show) {
	if(!bLogToScreen) return;
	
	console.Draw( _title.c_str() , _show);
}
