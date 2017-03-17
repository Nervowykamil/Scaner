#include "Reader.h"
#include "Offsets.h"
#include <TlHelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <string>

#define UNINITIALIZED 0xFFFFFFFF

Reader::Reader()
{
    ReadWowBaseAddressAndHandle(baseAddr, hWoW);
}

DWORD GetModuleBase(HANDLE hProc, std::string &sModuleName)
{
    HMODULE *hModules;
    char szBuf[50];
    DWORD cModules;
    DWORD dwBase = -1;
    //------

    EnumProcessModules(hProc, hModules, 0, &cModules);
    hModules = new HMODULE[cModules / sizeof(HMODULE)];

    if (EnumProcessModules(hProc, hModules, cModules / sizeof(HMODULE), &cModules)) {
        for (int i = 0; i < cModules / sizeof(HMODULE); i++) {
            if (GetModuleBaseName(hProc, hModules[i], LPWSTR(szBuf), sizeof(szBuf))) {
                if (sModuleName.compare(szBuf) == 0) {
                    dwBase = (DWORD)hModules[i];
                    break;
                }
            }
        }
    }

    delete[] hModules;

    return dwBase;
}

void Reader::ReadWowBaseAddressAndHandle(DWORD &addr, HANDLE &handle)
{
    DWORD value;
    HWND Wow = FindWindow(NULL, _T("World of Warcraft"));
    DWORD Pid;
    GetWindowThreadProcessId(Wow, &Pid);
    handle = OpenProcess(PROCESS_ALL_ACCESS, 0, Pid);
    
    addr = GetModuleBase(handle, std::string("Wow.exe"));

    DWORD objMgrpre;
    ReadProcessMemory(hWoW, (LPVOID)OFF_CLIENT_CONECTION, &objMgrpre, sizeof(DWORD), 0);
    ReadProcessMemory(hWoW, (LPVOID)(objMgrpre + OFF_OBJ_MGR), &objMgr, sizeof(DWORD), 0);

}

void Reader::ReadPlayerXYZ(float &x, float &y, float &z)
{
    DWORD lvl1PTR, lvl2PTR, plrBASE;
    UINT64 guid = GetLocalGUID();
    plrBASE = FindByGUID(guid);

    ReadProcessMemory(hWoW, (LPVOID)(plrBASE + OFF_UNIT_X), &x, sizeof(float), 0);
    ReadProcessMemory(hWoW, (LPVOID)(plrBASE + OFF_UNIT_Y), &y, sizeof(float), 0);
    ReadProcessMemory(hWoW, (LPVOID)(plrBASE + OFF_UNIT_Z), &z, sizeof(float), 0);

}

DWORD Reader::FindByGUID(UINT64 guid)
{
    DWORD nextobj, objType;
    UINT64 objGUID;

    ReadProcessMemory(hWoW, (LPVOID)(objMgr + OFF_FIRST_OBJ), &nextobj, sizeof(DWORD), 0);
    ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_TYPE), &objType, sizeof(DWORD), 0);

    while (objType < 7 && objType > 0)
    {
        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_GUID), &objGUID, sizeof(UINT64), 0);

        if (objGUID == guid)
            return nextobj;

        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_NEXT_OBJ), &nextobj, sizeof(DWORD), 0);
        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_TYPE), &objType, sizeof(DWORD), 0);
    }

    return 0;
}

UINT64 Reader::GetLocalGUID()
{
    UINT64 localGUID;
    ReadProcessMemory(hWoW, (LPVOID)(objMgr + OFF_OBJ_MGR_PLR_GUID), &localGUID, sizeof(UINT64), 0);
    return localGUID;
}
