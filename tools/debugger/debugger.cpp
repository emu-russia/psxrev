// Universal Debugger (Win32 version)
//

#include <Windows.h>
#include <tchar.h>

#include "debugger.h"
#include "typedefs.h"

typedef struct DEBUG_CB
{
	bool	opened;

} DEBUG_CB;

static DEBUG_CB db;

// ---------------------------------------------------------------------------------------------

bool DebugOpen(TCHAR *InitialTitle, TCHAR *InitialSubtitle)
{
	if (db.opened) return true;

	db.opened = true;
	return true;
}

void DebugDestroy()
{
	if (db.opened) {

		db.opened = false;
	}
}

void DebugRefresh()
{

}

void DebugSetTitle(TCHAR *Title)
{

}

void DebugSetSubtitle(TCHAR *SubTitle)
{

}

// ---------------------------------------------------------------------------------------------
