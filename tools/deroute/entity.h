
//
// Vector data format
//

#pragma once

//
// Area Types
//

enum {
    AreaViasInput = 1,
    AreaViasOutput,
    AreaViasInout,
    AreaViasConnect,
    AreaViasFloating,
    AreaViasPower,
    AreaViasGround,
    AreaWireInterconnect,
    AreaWirePower,
    AreaWireGround,
    AreaCellNot,
    AreaCellBuffer,
    AreaCellMux,
    AreaCellLogic,
    AreaCellAdder,
    AreaCellBusSupp,
    AreaCellFlipFlop,
    AreaCellLatch,
    AreaUnitRegfile,
    AreaUnitMemory,
    AreaUnitCustom,
};

//
// Label alignment types
//

enum {
    ValignTop = 1,
    ValignMiddle,
    ValignBottom,
    HalignTop,
    HalignMiddle,
    HalignBottom,
};

//
// Area description
//

typedef struct _AREA_ENTRY
{
    LONG PosX;
    LONG PosY;
    LONG Width;
    LONG Height;
    ULONG Type;
    CHAR Label[16];
    USHORT VAlign;
    USHORT HAlign;
} AREA_ENTRY, *PAREA_ENTRY;

//
// Entity description
//

typedef struct _ENTITY_ENTRY
{
    ULONG AreaNum;
    AREA_ENTRY Area[1];     // Variable size
} ENTITY_ENTRY, *PENTITY_ENTRY;

//
// Entity image description
//

#define ENTITY_IMAGE_SIGNATURE 'TCEV'

#define ENTITY_IMAGE_PADDING 32

typedef struct _ENTITY_IMAGE
{
    union
    {
        struct
        {
            ULONG Signature;
            ULONG EntityNum;
            ULONG EntitySize;
        };
        UCHAR Padding[ENTITY_IMAGE_PADDING];
    };

    ENTITY_ENTRY Entity[1];         // Variable size
} ENTITY_IMAGE, *PENTITY_IMAGE;
