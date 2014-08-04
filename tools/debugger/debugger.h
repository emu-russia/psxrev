// Debugger API.

#pragma once

// Main controls

bool	DebugOpen(TCHAR *InitialTitle, TCHAR *InitialSubtitle);
void	DebugDestroy();
void	DebugRefresh();
void	DebugSetTitle(TCHAR *Title);
void	DebugSetSubtitle(TCHAR *SubTitle);

// Menu

bool	DebugCreateMenu();
void	DebugDestroyMenu();
HDEBUGMENUITEM DebugAddMenuItem(TCHAR *Caption);
void	DebugDestroyMenuItem(HDEBUGMENUITEM);
HDEBUGSUBMENUITEM DebugAddSubmenuItem(TCHAR *Caption, DEBUGSUBMENUPROC DebugSubmenuProc);
void	DebugDestroySubmenuItem(HDEBUGSUBMENUITEM);
void	DebugCheckSubmenuItem(HDEBUGSUBMENUITEM Item, BOOL Checked);
void	DebugGraySubmenuItem(HDEBUGSUBMENUITEM Item, BOOL Grayed);

// Buttons

bool	DebugCreateButtons();
void	DebugDestroyButtons();
HDEBUGBUTTON DebugAddButton(TCHAR *Caption, DEBUGBUTTONPROC ButtonProc);
void	DebugDestroyButton(HDEBUGBUTTON Button);
void	DebugGrayButton(HDEBUGBUTTON Button);

// Views

bool	DebugCreateViews();
void	DebugDestroyViews();
HDEBUGVIEW DebugAddView(int DockOrder);
void	DebugDestroyView(HDEBUGVIEW);
void	DebugGrayView(HDEBUGVIEW);

// Log output

bool	DebugCreateLog();
void	DebugDestroyLog();
HDEBUGLOGGROUP DebugAddLogGroup(TCHAR *GroupName);
void    DebugLog(HDEBUGLOGGROUP Group, TCHAR *Fmt, ...);

// Command line

bool	DebugCreateCommandline();
void	DebugDestroyCommandline();
void	DebugCommandlineHandler(DEBUGCMDLINEPROC CommandlineProc);

// Status

bool	DebugCreateStatus();
void	DebugDestroyStatus();
HDEBUGSTATUS DebugAddStatusColumn(TCHAR *InitialText, int ColumnWidth);
void	DebugDestroyStatusColumn(HDEBUGSTATUS StatusColumn);
void	DebugGetStatusText(HDEBUGSTATUS StatusColumn, TCHAR *Text, int MaxLength);
void	DebugSetStatusText(HDEBUGSTATUS StatusColumn, TCHAR *Text);

// Hotkeys

HDEBUGHOTKEY DebugRegisterHotkey(int VKey, DEBUGHOTKEYPROC HotkeyProc);
void	DebugDestroyHotkey(HDEBUGHOTKEY HotKey);

// Data Input
