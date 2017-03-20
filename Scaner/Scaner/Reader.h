#pragma once
#include "windows.h"
#include <string>
#include <list>

struct object
{
    UINT64 guid;
    DWORD type;
    float x, y, z, r;
    std::string name;
};

class Reader
{
public:
    Reader();
    void ReadWowBaseAddressAndHandle(DWORD &addr, HANDLE &handle);
    void UpdatePlayerInfo();
    DWORD FindPlayerByGUID(UINT64 guid);
    object ReadObjectInfo(DWORD loc);
    UINT64 GetLocalGUID();
    float GetDist2D(float x, float y);
    float GetAngleToXY(float x, float y);
    void PositionForMap(float x, float y, int &x1, int &y1);
    void GetObjectsForMap(int range);
    std::list<object> list;
    float local_x, local_y, local_z, local_r;
private:
    UINT64 localGUID;
    HANDLE hWoW;
    DWORD baseAddr;
    DWORD objMgr;
};