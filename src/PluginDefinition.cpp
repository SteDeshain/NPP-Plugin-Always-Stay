//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

wfstream fs;

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Edit always-stay-file list"), eidtRCFile, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void info(const wchar_t* msg)
{
	::MessageBoxW(NULL, msg, L"Notepad++ Plugin Information", MB_OK);
}

void eidtRCFile()
{
	wchar_t nppPath[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)nppPath);
	wcscat(nppPath, L"\\");
	wcscat(nppPath, RC_FILE_NAME);
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)nppPath);
}

//
void afterNPPLaunched()
{
	// get current opened, which is the last-opened file
	wchar_t currentPath[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)currentPath);

	fs.open(RC_FILE_NAME, ios::out | ios::in | ios::app);
	fs.imbue(locale(locale::classic(), new codecvt_utf8<wchar_t>));
	fs.seekg(0, ios::beg);
	wchar_t buffer[MAX_PATH];
	while (!fs.eof())
	{
		fs.getline(buffer, MAX_PATH);
		if (buffer[0] == '\0' || buffer[0] == '#')
			continue;
		::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)buffer);
	}
	fs.close();
	
	// Switch to the last-opened file
	//::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)currentPath);
	::SendMessage(nppData._nppHandle, NPPM_SWITCHTOFILE, 0, (LPARAM)currentPath);
}
