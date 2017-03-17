#pragma once

enum SomeOffsets
{
    OFFSET_NONE,
    OFF_CLIENT_CONECTION        = 0x00C79CE0,
    OFF_OBJ_MGR                 = 0x2ED0,
    OFF_FIRST_OBJ               = 0xAC,
    OFF_NEXT_OBJ                = 0x3C,
    OFF_OBJ_TYPE                = 0x14,
    OFF_OBJ_GUID                = 0x30,
    OFF_OBJ_MGR_PLR_GUID        = 0xC0,
    OFF_PLR_BASE_STATIC         = 0x00CD87A8,

    OFF_UNIT_X                  = 0x798,
    OFF_UNIT_Y                  = 0x798 + 0x04,
    OFF_UNIT_Z                  = 0x798 + 0x08,

};