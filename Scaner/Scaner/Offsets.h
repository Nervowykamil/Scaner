#pragma once

enum GeneralOffsets
{
    OFFSET_NONE,
    OFF_CLIENT_CONECTION        = 0x00C79CE0,
    
    // object manager offsets
    OFF_OBJ_MGR                 = 0x2ED0,
    OFF_FIRST_OBJ               = 0xAC,
    OFF_NEXT_OBJ                = 0x3C,

    // object info
    OFF_OBJ_TYPE                = 0x14,
    OFF_OBJ_GUID                = 0x30,
    OFF_OBJ_MGR_PLR_GUID        = 0xC0,
};

enum UnitOffsets
{
    OFF_UNIT_X                  = 0x798,
    OFF_UNIT_Y                  = 0x798 + 0x04,
    OFF_UNIT_Z                  = 0x798 + 0x08,
    OFF_UNIT_R                  = 0x798 + 0x10,
    OFF_UNIT_NAME1              = 0x964,
    OFF_UNIT_NAME2              = 0x5C,
};

enum GameobjectOffsets
{
    OFF_OBJ_NAME1               = 0x1A4,
    OFF_OBJ_NAME2               = 0x90,
    OFF_OBJ_X                   = 0xE8,
    OFF_OBJ_Y                   = 0xEC,
    OFF_OBJ_Z                   = 0xF0,
};