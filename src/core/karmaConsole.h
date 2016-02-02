//
//  karmaConsole.h
//  karmaMapper
//
//  Created by Daan de Lange on 02/02/2016.
//
//	An abstract class for making karmaMapper modules
//

#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

//
struct karmaConsole {
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;
	
	karmaConsole();
	~karmaConsole();
	
	void    ClearLog();
	
	void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2);
	
	void    Draw(const char* title, bool& opened);
	
	static int Stricmp(const char* str1, const char* str2)               { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int Strnicmp(const char* str1, const char* str2, int count)   { int d = 0; while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; count--; } return d; }
	
	void    ExecCommand(const char* command_line);
	
	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
	{
		karmaConsole* console = (karmaConsole*)data->UserData;
		return console->TextEditCallback(data);
	}
	
	int     TextEditCallback(ImGuiTextEditCallbackData* data)
	{
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				// Example of TEXT COMPLETION
				
				// Locate beginning of current word
				const char* word_end = data->Buf + data->CursorPos;
				const char* word_start = word_end;
				while (word_start > data->Buf)
				{
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}
				
				// Build a list of candidates
				ImVector<const char*> candidates;
				for (int i = 0; i < Commands.Size; i++)
					if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
						candidates.push_back(Commands[i]);
				
				if (candidates.Size == 0)
				{
					// No match
					AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
				}
				else if (candidates.Size == 1)
				{
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
					data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
					int match_len = (int)(word_end - word_start);
					for (;;)
					{
						int c = 0;
						bool all_candidates_matches = true;
						for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}
					
					if (match_len > 0)
					{
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}
					
					// List matches
					AddLog("Possible matches:\n");
					for (int i = 0; i < candidates.Size; i++)
						AddLog("- %s\n", candidates[i]);
				}
				
				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				// Example of HISTORY
				const int prev_history_pos = HistoryPos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (HistoryPos == -1)
						HistoryPos = History.Size - 1;
					else if (HistoryPos > 0)
						HistoryPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (HistoryPos != -1)
						if (++HistoryPos >= History.Size)
							HistoryPos = -1;
				}
				
				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != HistoryPos)
				{
					snprintf(data->Buf, data->BufSize, "%s", (HistoryPos >= 0) ? History[HistoryPos] : "");
					data->BufDirty = true;
					data->CursorPos = data->SelectionStart = data->SelectionEnd = (int)strlen(data->Buf);
				}
			}
		}
		return 0;
	}
};



class karmaConsoleChannel : public ofBaseLoggerChannel {
public:
	static shared_ptr<karmaConsoleChannel> &getLogger( );
	virtual ~karmaConsoleChannel();
	
	void log(ofLogLevel level, const string & module, const string & message);
	
	void log(ofLogLevel logLevel, const string & module, const char* format, ...);
	
	void log(ofLogLevel logLevel, const string & module, const char* format, va_list args);
	
	void drawImGui(const string& _title, bool& _show);
	
private:
	static ofPtr<karmaConsoleChannel> logger;
	static karmaConsoleChannel *logPtr;
	
	karmaConsoleChannel();
	bool bLogToConsole;
	bool bLogToScreen;
	ofConsoleLoggerChannel consoleLogger;
	karmaConsole console;
};