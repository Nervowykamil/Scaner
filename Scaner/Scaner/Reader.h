#pragma once
#include "windows.h"

class Reader
{
public:
    Reader();
    void ReadWowBaseAddressAndHandle(DWORD &addr, HANDLE &handle);
    void ReadPlayerXYZ(float &x, float &y, float &z);
    DWORD FindByGUID(UINT64 guid);
    UINT64 GetLocalGUID();
private:
    HANDLE hWoW;
    DWORD baseAddr;
    DWORD objMgr;
};