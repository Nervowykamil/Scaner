#include "Reader.h"
#include "Offsets.h"
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
    HWND Wow = FindWindow(NULL, _T("World of Warcraft"));
    DWORD Pid;
    GetWindowThreadProcessId(Wow, &Pid);
    handle = OpenProcess(PROCESS_ALL_ACCESS, 0, Pid);
    
    addr = GetModuleBase(handle, std::string("Wow.exe"));

    DWORD objMgrpre;
    ReadProcessMemory(hWoW, (LPVOID)OFF_CLIENT_CONECTION, &objMgrpre, sizeof(DWORD), 0);
    ReadProcessMemory(hWoW, (LPVOID)(objMgrpre + OFF_OBJ_MGR), &objMgr, sizeof(DWORD), 0);

}

void Reader::UpdatePlayerInfo()
{
    DWORD plrBase;
    object plr;
    UINT64 guid = GetLocalGUID();
    plrBase = FindPlayerByGUID(guid);
    plr = ReadObjectInfo(plrBase);
    localGUID = plr.guid;

    ReadProcessMemory(hWoW, (LPVOID)(plrBase + OFF_UNIT_X), &local_x, sizeof(float), 0);
    ReadProcessMemory(hWoW, (LPVOID)(plrBase + OFF_UNIT_Y), &local_y, sizeof(float), 0);
    ReadProcessMemory(hWoW, (LPVOID)(plrBase + OFF_UNIT_Z), &local_z, sizeof(float), 0);
    ReadProcessMemory(hWoW, (LPVOID)(plrBase + OFF_UNIT_R), &local_r, sizeof(float), 0);
}

DWORD Reader::FindPlayerByGUID(UINT64 guid)
{
    DWORD nextobj, objType;
    UINT64 objGUID;

    ReadProcessMemory(hWoW, (LPVOID)(objMgr + OFF_FIRST_OBJ), &nextobj, sizeof(DWORD), 0);
    ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_TYPE), &objType, sizeof(DWORD), 0);

    while (objType < 7 && objType > 0)
    {
        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_GUID), &objGUID, sizeof(UINT64), 0);

        if (objGUID == guid && objType == 4)
            return nextobj;

        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_NEXT_OBJ), &nextobj, sizeof(DWORD), 0);
        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_TYPE), &objType, sizeof(DWORD), 0);
    }

    return 0;
}

object Reader::ReadObjectInfo(DWORD loc)
{
    object obj;
    DWORD name1, name2;
    char name[20];
    // guid
    ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_OBJ_GUID), &obj.guid, sizeof(UINT64), 0);
    // type
    ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_OBJ_TYPE), &obj.type, sizeof(DWORD), 0);
    // name
    switch (obj.type)
    {
        case 0: // ot_none
        {
            obj.name = "None";
            break;
        }
        case 1: // ot_item
        {
            obj.name = "Item";
            break;
        }
        case 2: // ot_container
        {
            obj.name = "Container";
            break;
        }
        case 3: // ot_unit TODO
        {
            ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_UNIT_NAME1), &name1, sizeof(DWORD), 0);
            ReadProcessMemory(hWoW, (LPVOID)(name1 + OFF_UNIT_NAME2), &name2, sizeof(DWORD), 0);
            ReadProcessMemory(hWoW, (LPVOID)(name2), &name, sizeof(char[20]), 0);
            obj.name = name;
            break;
        }
        case 4: // ot_player TODO
        {
            UINT64 mask, base_, offset, current, shortGUID, testGUID;
            ReadProcessMemory(hWoW, (LPVOID)(0xC5D938 + 0x8 + 0x024), &mask, sizeof(DWORD), 0);
            ReadProcessMemory(hWoW, (LPVOID)(0xC5D938 + 0x8 + 0x01c), &base_, sizeof(DWORD), 0);

            shortGUID = obj.guid & 0xffffffff;
            offset = 12 * (mask & shortGUID);

            ReadProcessMemory(hWoW, (LPVOID)(base_ + offset + 8), &current, sizeof(UINT), 0);
            ReadProcessMemory(hWoW, (LPVOID)(base_ + offset), &offset, sizeof(UINT), 0);

            if ((current & 0x1) == 0x1) { obj.name == ""; break; }

            while (testGUID != shortGUID)
            {
                ReadProcessMemory(hWoW, (LPVOID)(base_ + offset + 4), &current, sizeof(UINT), 0);

                if ((current & 0x1) == 0x1) { obj.name == ""; break; }
                ReadProcessMemory(hWoW, (LPVOID)(current), &testGUID, sizeof(UINT), 0);
            }

            ReadProcessMemory(hWoW, (LPVOID)(current + 0x020), &name, sizeof(char[20]), 0);
            obj.name = name;
            break;
        }
        case 5: // ot_gobject TODO
        {
            ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_OBJ_NAME1), &name1, sizeof(DWORD), 0);
            ReadProcessMemory(hWoW, (LPVOID)(name1 + OFF_OBJ_NAME2), &name2, sizeof(DWORD), 0);
            ReadProcessMemory(hWoW, (LPVOID)(name2), &name, sizeof(char[20]), 0);
            obj.name = name;
            break;
        }
        case 6: // ot_dynobject
        {
            obj.name = "DynObj";
            break;
        }
        case 7: // ot_corpse
        {
            obj.name = "Corpse";
            break;
        }
        default:
        {
            break;
        }
    }
    // possition
    if (obj.type == 3 || obj.type == 4)
    {
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_UNIT_X), &obj.x, sizeof(float), 0);
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_UNIT_Y), &obj.y, sizeof(float), 0);
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_UNIT_Z), &obj.z, sizeof(float), 0);
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_UNIT_R), &obj.r, sizeof(float), 0);
    }
    if (obj.type == 5)
    {
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_OBJ_X), &obj.x, sizeof(float), 0);
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_OBJ_Y), &obj.y, sizeof(float), 0);
        ReadProcessMemory(hWoW, (LPVOID)(loc + OFF_OBJ_Z), &obj.z, sizeof(float), 0);
    }
    return obj;
}

UINT64 Reader::GetLocalGUID()
{
    UINT64 localGUID;
    ReadProcessMemory(hWoW, (LPVOID)(objMgr + OFF_OBJ_MGR_PLR_GUID), &localGUID, sizeof(UINT64), 0);
    return localGUID;
}

float Reader::GetDist2D(float x, float y)
{
    float distX = fabs(x) > fabs(local_x) ? fabs(x) - fabs(local_x) : fabs(local_x) - fabs(x);
    float distY = fabs(y) > fabs(local_y) ? fabs(y) - fabs(local_y) : fabs(local_y) - fabs(y);

    return sqrt(distX * distX + distY * distY);
}

float Reader::GetAngleToXY(float x, float y)
{
    return atan2(x, y);;
}

void Reader::PositionForMap(float x, float y, int &x1, int &y1)
{
    float m_pi = 3.141592653;
    float x2 = local_x - x;
    float y2 = local_y - y;
    float dist = GetDist2D(x, y);
    float angle = GetAngleToXY(x2, y2) + local_r;
    
    // normalize angle to be between 0 and 2 * m_pi
    while (angle > (2 * m_pi))
    {
        angle -= 2 * m_pi;
    }

    // rotate to current player orientation
    x1 = dist * cos(angle);
    y1 = dist * sin(angle);

    x1 = int(2 * x1);
    y1 = int(2 * y1);
    return;
}

void Reader::GetObjectsForMap(int range)
{
    DWORD nextobj, objType;
    UINT64 objGUID;

    // initial cleanup of the list
    list.clear();

    ReadProcessMemory(hWoW, (LPVOID)(objMgr + OFF_FIRST_OBJ), &nextobj, sizeof(DWORD), 0);
    ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_TYPE), &objType, sizeof(DWORD), 0);

    while (objType < 7 && objType > 0)
    {
        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_GUID), &objGUID, sizeof(UINT64), 0);

        if (objType > 2 && objType < 6)
        {
            object obj = ReadObjectInfo(nextobj);
            if (GetDist2D(obj.x, obj.y) < range)
                list.push_back(obj);
        }

        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_NEXT_OBJ), &nextobj, sizeof(DWORD), 0);
        ReadProcessMemory(hWoW, (LPVOID)(nextobj + OFF_OBJ_TYPE), &objType, sizeof(DWORD), 0);
    }

    return;
}
