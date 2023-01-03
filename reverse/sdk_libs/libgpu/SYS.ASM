    OPT c+,at-,m-

    xref    __SN_GP_BASE

    XREF    VSync
    XREF    SetIntrMask
    XREF    GetVideoMode
    XREF    memcpy
    XREF    DMACallback
    XREF    GPU_cw
    XREF    ResetCallback
    XREF    printf

    SECTION .sdata





    SECTION .sbss





    ASSUME  gp:__SN_GP_BASE,.sdata,.sbss

    SECTION .rdata

; "$Id: sys.c,v 1.140 1998/01/12 07:52:27 noda Exp yos $"

    DW  $3A644924,$73797320,$762C632E,$312E3120,$31203034,$2F383939
    DW  $312F3130,$37302032,$3A32353A,$6E203732,$2061646F,$20707845
    DW  $20736F79,$00000024
rdata_38:
; As string: "ResetGraph:jtb=%08x,env=%08x\n"
    DW  $65736552,$61724774,$6A3A6870,$253D6274,$2C783830,$3D766E65
    DW  $78383025,$0000000A
rdata_58:
; As string: "ResetGraph(%d)...\n"
    DW  $65736552,$61724774,$25286870,$2E2E2964,$00000A2E
rdata_6C:
; As string: "SetGraphDebug:level:%d,type:%d reverse:%d\n"
    DW  $47746553,$68706172,$75626544,$656C3A67,$3A6C6576,$742C6425
    DW  $3A657079,$72206425,$72657665,$253A6573,$00000A64
rdata_98:
; As string: "SetGrapQue(%d)...\n"
    DW  $47746553,$51706172,$25286575,$2E2E2964,$00000A2E
rdata_AC:
; As string: "DrawSyncCallback(%08x)...\n"
    DW  $77617244,$636E7953,$6C6C6143,$6B636162,$38302528,$2E2E2978
    DW  $00000A2E
rdata_C8:
; As string: "SetDispMask(%d)...\n"
    DW  $44746553,$4D707369,$286B7361,$2E296425,$000A2E2E
rdata_DC:
; As string: "DrawSync(%d)...\n"
    DW  $77617244,$636E7953,$29642528,$0A2E2E2E,$00000000
rdata_F0:
; As string: "%s:bad RECT"
    DW  $623A7325,$52206461,$00544345
rdata_FC:
; As string: "(%d,%d)-(%d,%d)\n"
    DW  $2C642528,$2D296425,$2C642528,$0A296425,$00000000
rdata_110:
; As string: "%s:"
    DW  $003A7325
rdata_114:
; As string: "ClearImage"
    DW  $61656C43,$616D4972,$00006567
rdata_120:
; As string: "ClearImage2"
    DW  $61656C43,$616D4972,$00326567
rdata_12C:
; As string: "LoadImage"
    DW  $64616F4C,$67616D49,$00000065
rdata_138:
; As string: "StoreImage"
    DW  $726F7453,$616D4965,$00006567
rdata_144:
; As string: "MoveImage"
    DW  $65766F4D,$67616D49,$00000065
rdata_150:
; As string: "ClearOTag(%08x,%d)...\n"
    DW  $61656C43,$61544F72,$30252867,$252C7838,$2E2E2964,$00000A2E
rdata_168:
; As string: "ClearOTagR(%08x,%d)...\n"
    DW  $61656C43,$61544F72,$25285267,$2C783830,$2E296425,$000A2E2E
rdata_180:
; As string: "DrawOTag(%08x)...\n"
    DW  $77617244,$6761544F,$38302528,$2E2E2978,$00000A2E
rdata_194:
; As string: "PutDrawEnv(%08x)...\n"
    DW  $44747550,$45776172,$2528766E,$29783830,$0A2E2E2E,$00000000
rdata_1AC:
; As string: "DrawOTagEnv(%08x,&08x)...\n"
    DW  $77617244,$6761544F,$28766E45,$78383025,$3830262C,$2E2E2978
    DW  $00000A2E
rdata_1C8:
; As string: "PutDispEnv(%08x)...\n"
    DW  $44747550,$45707369,$2528766E,$29783830,$0A2E2E2E,$00000000
rdata_1E0:
; As string: "GPU timeout:que=%d,stat=%08x,chcr=%08x,madr=%08x\n"
    DW  $20555047,$656D6974,$3A74756F,$3D657571,$732C6425,$3D746174
    DW  $78383025,$6368632C,$30253D72,$6D2C7838,$3D726461,$78383025
    DW  $0000000A
rdata_214:
; As string: "LoadImage2"
    DW  $64616F4C,$67616D49,$00003265



    SECTION .text

    XDEF    DrawOTag2
    XDEF    MoveImage2
    XDEF    StoreImage2
    XDEF    _GPU_ResetCallback
    XDEF    LoadImage2
    XDEF    SetDrawEnv
    XDEF    SetDrawOffset
    XDEF    SetDrawArea
    XDEF    GetODE
    XDEF    GetDispEnv
    XDEF    PutDispEnv
    XDEF    GetDrawEnv
    XDEF    DrawOTagEnv
    XDEF    PutDrawEnv
    XDEF    DrawOTag
    XDEF    DrawPrim
    XDEF    ClearOTagR
    XDEF    ClearOTag
    XDEF    MoveImage
    XDEF    StoreImage
    XDEF    LoadImage
    XDEF    ClearImage2
    XDEF    ClearImage
    XDEF    DrawSync
    XDEF    SetDispMask
    XDEF    DrawSyncCallback
    XDEF    GetGraphDebug
    XDEF    SetGraphQueue
    XDEF    SetGraphDebug
    XDEF    ResetGraph

ResetGraph:
    addiu   sp,sp,-32
    sw  s1,20(sp)
    move    s1,a0
    andi    v1,s1,$0007
    li  v0,3
    sw  ra,24(sp)
    beq v1,v0,text_4C
    sw  s0,16(sp)
    slti    v0,v1,4
    beqz    v0,text_3C
    li  v0,5
    beqz    v1,text_4C
    nop
    j   text_110
    nop
text_3C:
    beq v1,v0,text_68
    nop
    j   text_110
    nop
text_4C:
    lui a0,_hi(rdata_38)
    addiu   a0,a0,_lo(rdata_38)
    lui a1,_hi(data_8)
    addiu   a1,a1,_lo(data_8)
    lui a2,_hi(GEnv)
    jal printf
    addiu   a2,a2,_lo(GEnv)
text_68:
    lui s0,_hi(GEnv)
    addiu   s0,s0,_lo(GEnv)
    move    a0,s0
    move    a1,zero
    jal text_2ED8
    li  a2,128
    jal ResetCallback
    nop
    lui v0,$00FF
    lui a0,_hi(data_48)
    lw  a0,_lo(data_48)(a0)
    ori v0,v0,$FFFF
    jal GPU_cw
    and a0,a0,v0
    jal text_2608
    move    a0,s1
    addiu   a0,s0,16
    sb  v0,0(s0)
    lbu v0,0(s0)
    li  v1,1
    sb  v1,1(s0)
    sll v0,v0,2
    lui v1,_hi(data_D0)
    addu    v1,v1,v0
    lhu v1,_lo(data_D0)(v1)
    lbu v0,0(s0)
    li  a1,-1
    sll v0,v0,2
    sh  v1,4(s0)
    lui at,_hi(data_DC)
    addu    at,at,v0
    lhu v0,_lo(data_DC)(at)
    li  a2,92
    jal text_2ED8
    sh  v0,6(s0)
    addiu   a0,s0,108
    li  a1,-1
    jal text_2ED8
    li  a2,20
    lbu v0,0(s0)
    j   text_160
    nop
text_110:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    nop
    sltiu   v0,v0,2
    bnez    v0,text_144
    nop
    lui a0,_hi(rdata_58)
    addiu   a0,a0,_lo(rdata_58)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s1
text_144:
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    nop
    lw  v0,52(v0)
    nop
    jalr    ra,v0
    li  a0,1
text_160:
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

SetGraphDebug:
    addiu   sp,sp,-24
    lui v1,_hi(data_52)
    addiu   v1,v1,_lo(data_52)
    sw  ra,20(sp)
    sw  s0,16(sp)
    lbu s0,0(v1)
    sb  a0,0(v1)
    andi    a0,a0,$00FF
    beqz    a0,text_1C0
    move    v0,s0
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    lbu a1,0(v1)
    lbu a2,-2(v1)
    lbu a3,1(v1)
    lui a0,_hi(rdata_6C)
    jalr    ra,v0
    addiu   a0,a0,_lo(rdata_6C)
    move    v0,s0
text_1C0:
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

SetGraphQueue:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    sw  s1,20(sp)
    lui s1,_hi(data_51)
    addiu   s1,s1,_lo(data_51)
    sw  ra,28(sp)
    sw  s2,24(sp)
    lbu v0,1(s1)
    lbu s2,0(s1)
    sltiu   v0,v0,2
    bnez    v0,text_21C
    move    s0,a0
    lui a0,_hi(rdata_98)
    addiu   a0,a0,_lo(rdata_98)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s0
text_21C:
    lbu v0,0(s1)
    nop
    beq s0,v0,text_25C
    move    v0,s2
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    nop
    lw  v0,52(v0)
    nop
    jalr    ra,v0
    li  a0,1
    li  a0,2
    move    a1,zero
    jal DMACallback
    sb  s0,0(s1)
    move    v0,s2
text_25C:
    lw  ra,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

GetGraphDebug:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    jr  ra
    nop

DrawSyncCallback:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    lui s0,_hi(data_52)
    addiu   s0,s0,_lo(data_52)
    sw  ra,24(sp)
    sw  s1,20(sp)
    lbu v0,0(s0)
    nop
    sltiu   v0,v0,2
    bnez    v0,text_2C8
    move    s1,a0
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    lui a0,_hi(rdata_AC)
    addiu   a0,a0,_lo(rdata_AC)
    jalr    ra,v0
    move    a1,s1
text_2C8:
    lw  v0,10(s0)
    sw  s1,10(s0)
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

SetDispMask:
    addiu   sp,sp,-32
    sw  s1,20(sp)
    lui s1,_hi(data_52)
    addiu   s1,s1,_lo(data_52)
    sw  ra,24(sp)
    sw  s0,16(sp)
    lbu v0,0(s1)
    nop
    sltiu   v0,v0,2
    bnez    v0,text_32C
    move    s0,a0
    lui a0,_hi(rdata_C8)
    addiu   a0,a0,_lo(rdata_C8)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s0
text_32C:
    bnez    s0,text_340
    addiu   a0,s1,106
    li  a1,-1
    jal text_2ED8
    li  a2,20
text_340:
    lui a0,$0300
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    beqz    s0,text_358
    ori a0,a0,$0001
    lui a0,$0300
text_358:
    lw  v0,16(v0)
    nop
    jalr    ra,v0
    nop
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

DrawSync:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    sltiu   v0,v0,2
    bnez    v0,text_3B8
    sw  ra,20(sp)
    lui a0,_hi(rdata_DC)
    addiu   a0,a0,_lo(rdata_DC)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s0
text_3B8:
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    nop
    lw  v0,60(v0)
    nop
    jalr    ra,v0
    move    a0,s0
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

text_3E4:
    addiu   sp,sp,-32
    move    t0,a0
    sw  s0,24(sp)
    lui a0,_hi(data_52)
    addiu   a0,a0,_lo(data_52)
    sw  ra,28(sp)
    lbu v1,0(a0)
    li  v0,1
    beq v1,v0,text_420
    move    s0,a1
    li  v0,2
    beq v1,v0,text_4AC
    nop
    j   text_4F0
    nop
text_420:
    lh  a1,4(s0)
    lh  v1,2(a0)
    nop
    slt v0,v1,a1
    bnez    v0,text_4A0
    nop
    lh  a3,0(s0)
    nop
    addu    v0,a1,a3
    slt v0,v1,v0
    bnez    v0,text_4A0
    nop
    lh  v1,2(s0)
    lh  a0,4(a0)
    nop
    slt v0,a0,v1
    bnez    v0,text_4A0
    nop
    lh  a2,6(s0)
    nop
    addu    v0,v1,a2
    slt v0,a0,v0
    bnez    v0,text_4A0
    nop
    blez    a1,text_4A0
    nop
    bltz    a3,text_4A0
    nop
    bltz    v1,text_4A0
    nop
    bgtz    a2,text_4F0
    nop
text_4A0:
    lui a0,_hi(rdata_F0)
    j   text_4B4
    addiu   a0,a0,_lo(rdata_F0)
text_4AC:
    lui a0,_hi(rdata_110)
    addiu   a0,a0,_lo(rdata_110)
text_4B4:
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,t0
    lh  a1,0(s0)
    lh  a2,2(s0)
    lh  a3,4(s0)
    lh  v1,6(s0)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    lui a0,_hi(rdata_FC)
    addiu   a0,a0,_lo(rdata_FC)
    jalr    ra,v0
    sw  v1,16(sp)
text_4F0:
    lw  ra,28(sp)
    lw  s0,24(sp)
    jr  ra
    addiu   sp,sp,32

ClearImage:
    addiu   sp,sp,-40
    sw  s3,28(sp)
    move    s3,a0
    lui a0,_hi(rdata_114)
    addiu   a0,a0,_lo(rdata_114)
    sw  s2,24(sp)
    move    s2,a1
    move    a1,s3
    sw  s1,20(sp)
    move    s1,a2
    sw  s0,16(sp)
    sw  ra,32(sp)
    jal text_3E4
    move    s0,a3
    move    a1,s3
    andi    s0,s0,$00FF
    sll s0,s0,16
    andi    s1,s1,$00FF
    sll s1,s1,8
    or  s0,s0,s1
    andi    s2,s2,$00FF
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    li  a2,8
    lw  a0,12(v0)
    lw  v0,8(v0)
    nop
    jalr    ra,v0
    or  a3,s0,s2
    lw  ra,32(sp)
    lw  s3,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,40

ClearImage2:
    addiu   sp,sp,-40
    sw  s3,28(sp)
    move    s3,a0
    lui a0,_hi(rdata_120)
    addiu   a0,a0,_lo(rdata_120)
    sw  s2,24(sp)
    move    s2,a1
    move    a1,s3
    sw  s0,16(sp)
    move    s0,a2
    sw  s1,20(sp)
    sw  ra,32(sp)
    jal text_3E4
    move    s1,a3
    move    a1,s3
    andi    s1,s1,$00FF
    sll s1,s1,16
    andi    s0,s0,$00FF
    sll s0,s0,8
    lui v0,$8000
    or  s0,s0,v0
    or  s1,s1,s0
    andi    s2,s2,$00FF
    lui v1,_hi(data_48)
    lw  v1,_lo(data_48)(v1)
    li  a2,8
    lw  a0,12(v1)
    lw  v0,8(v1)
    nop
    jalr    ra,v0
    or  a3,s1,s2
    lw  ra,32(sp)
    lw  s3,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,40

LoadImage:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s1,20(sp)
    move    s1,a1
    lui a0,_hi(rdata_12C)
    addiu   a0,a0,_lo(rdata_12C)
    sw  ra,24(sp)
    jal text_3E4
    move    a1,s0
    move    a1,s0
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    li  a2,8
    lw  a0,32(v0)
    lw  v0,8(v0)
    nop
    jalr    ra,v0
    move    a3,s1
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

StoreImage:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s1,20(sp)
    move    s1,a1
    lui a0,_hi(rdata_138)
    addiu   a0,a0,_lo(rdata_138)
    sw  ra,24(sp)
    jal text_3E4
    move    a1,s0
    move    a1,s0
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    li  a2,8
    lw  a0,28(v0)
    lw  v0,8(v0)
    nop
    jalr    ra,v0
    move    a3,s1
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

MoveImage:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s2,24(sp)
    move    s2,a1
    sw  s1,20(sp)
    move    s1,a2
    lui a0,_hi(rdata_144)
    addiu   a0,a0,_lo(rdata_144)
    sw  ra,28(sp)
    jal text_3E4
    move    a1,s0
    lh  v0,4(s0)
    nop
    beqz    v0,text_788
    li  v0,-1
    lh  v0,6(s0)
    nop
    bnez    v0,text_740
    sll v0,s1,16
    j   text_788
    li  v0,-1
text_740:
    lui v1,_hi(data_F0)
    addiu   v1,v1,_lo(data_F0)
    andi    a0,s2,$FFFF
    or  v0,v0,a0
    lw  a1,0(s0)
    lui a3,_hi(data_48)
    lw  a3,_lo(data_48)(a3)
    li  a2,20
    sw  v0,4(v1)
    sw  a1,0(v1)
    lw  v0,4(s0)
    addiu   a1,v1,-8
    sw  v0,8(v1)
    lw  a0,24(a3)
    lw  v0,8(a3)
    nop
    jalr    ra,v0
    move    a3,zero
text_788:
    lw  ra,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

ClearOTag:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s1,20(sp)
    move    s1,a1
    sltiu   v0,v0,2
    bnez    v0,text_7E4
    sw  ra,24(sp)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    lui a0,_hi(rdata_150)
    addiu   a0,a0,_lo(rdata_150)
    move    a1,s0
    jalr    ra,v0
    move    a2,s1
text_7E4:
    addiu   s1,s1,-1
    beqz    s1,text_820
    lui a1,$00FF
    ori a1,a1,$FFFF
    lui a2,$FF00
text_7F8:
    addiu   s1,s1,-1
    addiu   a0,s0,4
    sb  zero,3(s0)
    lw  v0,0(s0)
    and v1,a0,a1
    and v0,v0,a2
    or  v0,v0,v1
    sw  v0,0(s0)
    bnez    s1,text_7F8
    move    s0,a0
text_820:
    lui a2,$00FF
    ori a2,a2,$FFFF
    move    v0,s0
    lui a1,_hi(data_110)
    addiu   a1,a1,_lo(data_110)
    lui v1,_hi(data_FC)
    addiu   v1,v1,_lo(data_FC)
    and v1,v1,a2
    lui a0,$0400
    or  v1,v1,a0
    sw  v1,0(a1)
    and a1,a1,a2
    sw  a1,0(v0)
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

ClearOTagR:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s1,20(sp)
    move    s1,a1
    sltiu   v0,v0,2
    bnez    v0,text_8B0
    sw  ra,24(sp)
    lui a0,_hi(rdata_168)
    addiu   a0,a0,_lo(rdata_168)
    move    a1,s0
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a2,s1
text_8B0:
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    move    a0,s0
    lw  v0,44(v0)
    nop
    jalr    ra,v0
    move    a1,s1
    lui a2,$00FF
    ori a2,a2,$FFFF
    move    v0,s0
    lui a1,_hi(data_110)
    addiu   a1,a1,_lo(data_110)
    lui v1,_hi(data_FC)
    addiu   v1,v1,_lo(data_FC)
    and v1,v1,a2
    lui a0,$0400
    or  v1,v1,a0
    sw  v1,0(a1)
    and a1,a1,a2
    sw  a1,0(v0)
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

DrawPrim:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    sw  ra,24(sp)
    sw  s1,20(sp)
    lw  v0,60(v0)
    lbu s1,3(s0)
    jalr    ra,v0
    move    a0,zero
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    addiu   a0,s0,4
    lw  v0,20(v0)
    nop
    jalr    ra,v0
    move    a1,s1
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

DrawOTag:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    sltiu   v0,v0,2
    bnez    v0,text_9AC
    sw  ra,20(sp)
    lui a0,_hi(rdata_180)
    addiu   a0,a0,_lo(rdata_180)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s0
text_9AC:
    move    a1,s0
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    move    a2,zero
    lw  a0,24(v0)
    lw  v0,8(v0)
    nop
    jalr    ra,v0
    move    a3,zero
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

PutDrawEnv:
    addiu   sp,sp,-32
    sw  s2,24(sp)
    lui s2,_hi(data_52)
    addiu   s2,s2,_lo(data_52)
    sw  ra,28(sp)
    sw  s1,20(sp)
    sw  s0,16(sp)
    lbu v0,0(s2)
    nop
    sltiu   v0,v0,2
    bnez    v0,text_A2C
    move    s1,a0
    lui a0,_hi(rdata_194)
    addiu   a0,a0,_lo(rdata_194)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s1
text_A2C:
    addiu   s0,s1,28
    move    a0,s0
    jal text_13C0
    move    a1,s1
    lui a0,$00FF
    ori a0,a0,$FFFF
    move    a1,s0
    li  a2,64
    lw  v0,28(s1)
    lui v1,_hi(data_48)
    lw  v1,_lo(data_48)(v1)
    or  v0,v0,a0
    sw  v0,28(s1)
    lw  a0,24(v1)
    lw  v0,8(v1)
    nop
    jalr    ra,v0
    move    a3,zero
    addiu   a0,s2,14
    move    a1,s1
    jal memcpy
    li  a2,92
    move    v0,s1
    lw  ra,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

DrawOTagEnv:
    addiu   sp,sp,-40
    sw  s2,24(sp)
    move    s2,a0
    sw  s3,28(sp)
    lui s3,_hi(data_52)
    addiu   s3,s3,_lo(data_52)
    sw  ra,32(sp)
    sw  s1,20(sp)
    sw  s0,16(sp)
    lbu v0,0(s3)
    nop
    sltiu   v0,v0,2
    bnez    v0,text_AF8
    move    s1,a1
    lui a0,_hi(rdata_1AC)
    addiu   a0,a0,_lo(rdata_1AC)
    move    a1,s2
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a2,s1
text_AF8:
    addiu   s0,s1,28
    move    a0,s0
    jal text_13C0
    move    a1,s1
    lui a0,$00FF
    ori a0,a0,$FFFF
    move    a1,s0
    li  a2,64
    lui v1,$FF00
    lw  v0,28(s1)
    and a0,s2,a0
    and v0,v0,v1
    lui v1,_hi(data_48)
    lw  v1,_lo(data_48)(v1)
    or  v0,v0,a0
    sw  v0,28(s1)
    lw  a0,24(v1)
    lw  v0,8(v1)
    nop
    jalr    ra,v0
    move    a3,zero
    addiu   a0,s3,14
    move    a1,s1
    jal memcpy
    li  a2,92
    lw  ra,32(sp)
    lw  s3,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,40

GetDrawEnv:
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    lui a1,_hi(data_60)
    addiu   a1,a1,_lo(data_60)
    sw  ra,20(sp)
    jal memcpy
    li  a2,92
    move    v0,s0
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

PutDispEnv:
    addiu   sp,sp,-32
    sw  s1,20(sp)
    move    s1,a0
    sw  s2,24(sp)
    lui s2,_hi(data_52)
    addiu   s2,s2,_lo(data_52)
    sw  ra,28(sp)
    sw  s0,16(sp)
    lbu v0,0(s2)
    nop
    sltiu   v0,v0,2
    bnez    v0,text_BFC
    lui s0,$0800
    lui a0,_hi(rdata_1C8)
    addiu   a0,a0,_lo(rdata_1C8)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s1
text_BFC:
    lui v0,$0500
    lhu v1,2(s1)
    lhu a0,0(s1)
    lui a1,_hi(data_48)
    lw  a1,_lo(data_48)(a1)
    andi    v1,v1,$03FF
    sll v1,v1,10
    andi    a0,a0,$03FF
    or  a0,a0,v0
    lw  v0,16(a1)
    nop
    jalr    ra,v0
    or  a0,v1,a0
    lw  v1,122(s2)
    lw  v0,16(s1)
    nop
    bne v1,v0,text_CA4
    addiu   a0,s2,106
    lhu v0,106(s2)
    lh  v1,0(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_CA4
    nop
    lhu v0,2(a0)
    lh  v1,2(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_CA4
    nop
    lhu v0,4(a0)
    lh  v1,4(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_CA4
    nop
    lhu v0,6(a0)
    lh  v1,6(s1)
    sll v0,v0,16
    sra v0,v0,16
    beq v0,v1,text_D90
    nop
text_CA4:
    jal GetVideoMode
    nop
    sb  v0,18(s1)
    andi    v0,v0,$00FF
    li  v1,1
    bne v0,v1,text_CC4
    nop
    ori s0,s0,$0008
text_CC4:
    lbu v0,17(s1)
    nop
    beqz    v0,text_CD8
    nop
    ori s0,s0,$0010
text_CD8:
    lbu v0,16(s1)
    nop
    beqz    v0,text_CEC
    nop
    ori s0,s0,$0020
text_CEC:
    lui v0,_hi(data_53)
    lbu v0,_lo(data_53)(v0)
    nop
    beqz    v0,text_D04
    nop
    ori s0,s0,$0080
text_D04:
    lh  v1,4(s1)
    nop
    slti    v0,v1,281
    bnez    v0,text_D4C
    slti    v0,v1,353
    beqz    v0,text_D28
    slti    v0,v1,401
    j   text_D4C
    ori s0,s0,$0001
text_D28:
    beqz    v0,text_D38
    slti    v0,v1,561
    j   text_D4C
    ori s0,s0,$0040
text_D38:
    beqz    v0,text_D48
    nop
    j   text_D4C
    ori s0,s0,$0002
text_D48:
    ori s0,s0,$0003
text_D4C:
    lbu v0,18(s1)
    lh  v1,6(s1)
    bnez    v0,text_D60
    slti    v0,v1,289
    slti    v0,v1,257
text_D60:
    bnez    v0,text_D6C
    nop
    ori s0,s0,$0024
text_D6C:
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    nop
    lw  v0,16(v0)
    nop
    jalr    ra,v0
    move    a0,s0
    li  v0,8
    sb  v0,18(s1)
text_D90:
    lui a0,_hi(data_C4)
    addiu   a0,a0,_lo(data_C4)
    lhu v0,0(a0)
    lh  v1,8(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_E08
    nop
    lhu v0,2(a0)
    lh  v1,10(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_E08
    nop
    lhu v0,4(a0)
    lh  v1,12(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_E08
    nop
    lhu v0,6(a0)
    lh  v1,14(s1)
    sll v0,v0,16
    sra v0,v0,16
    bne v0,v1,text_E08
    li  v0,8
    lbu v1,18(s1)
    nop
    bne v1,v0,text_1074
    nop
text_E08:
    jal GetVideoMode
    nop
    sb  v0,18(s1)
    andi    v0,v0,$00FF
    lh  a0,10(s1)
    bnez    v0,text_E28
    addiu   s0,a0,19
    addiu   s0,a0,16
text_E28:
    lh  v0,14(s1)
    nop
    bnez    v0,text_E3C
    addu    s2,s0,v0
    addiu   s2,s0,240
text_E3C:
    lh  v1,4(s1)
    nop
    slti    v0,v1,281
    bnez    v0,text_E78
    move    a2,zero
    slti    v0,v1,353
    bnez    v0,text_E78
    li  a2,1
    slti    v0,v1,401
    bnez    v0,text_E78
    li  a2,2
    slti    v0,v1,561
    beqz    v0,text_E78
    li  a2,4
    li  a2,3
text_E78:
    lh  v1,8(s1)
    lui v0,_hi(data_14C)
    addu    v0,v0,a2
    lbu v0,_lo(data_14C)(v0)
    nop
    mult    v1,v0
    lbu v1,18(s1)
    nop
    sll v0,v1,2
    addu    v0,v0,v1
    addu    v0,v0,a2
    sll v0,v0,2
    lui v1,_hi(data_124)
    addu    v1,v1,v0
    lhu v1,_lo(data_124)(v1)
    lui at,_hi(data_126)
    addu    at,at,v0
    lhu v0,_lo(data_126)(at)
    lh  a1,12(s1)
    subu    v0,v0,v1
    mflo    a3
    beqz    a1,text_EE0
    addu    a0,v1,a3
    mult    v0,a1
    mflo    a3
    sra v0,a3,8
text_EE0:
    addu    v1,a0,v0
    lbu v0,18(s1)
    nop
    beqz    v0,text_F80
    slti    v0,a0,540
    bnez    v0,text_F0C
    li  a1,540
    slti    v0,a0,3221
    beqz    v0,text_F0C
    li  a1,3220
    move    a1,a0
text_F0C:
    lui v0,_hi(data_14C)
    addu    v0,v0,a2
    lbu v0,_lo(data_14C)(v0)
    move    a0,a1
    sll v0,v0,2
    addu    a1,a0,v0
    slt v0,v1,a1
    bnez    v0,text_F44
    slti    v0,s0,19
    slti    v0,v1,3261
    beqz    v0,text_F40
    li  a1,3260
    move    a1,v1
text_F40:
    slti    v0,s0,19
text_F44:
    bnez    v0,text_F60
    move    v1,a1
    slti    v0,s0,304
    beqz    v0,text_F64
    li  a1,303
    j   text_F64
    move    a1,s0
text_F60:
    li  a1,19
text_F64:
    move    s0,a1
    addiu   a1,s0,2
    slt v0,s2,a1
    bnez    v0,text_1018
    slti    v0,s2,306
    j   text_100C
    li  a1,305
text_F80:
    slti    v0,a0,500
    bnez    v0,text_F9C
    li  a1,500
    slti    v0,a0,3251
    beqz    v0,text_F9C
    li  a1,3250
    move    a1,a0
text_F9C:
    lui v0,_hi(data_14C)
    addu    v0,v0,a2
    lbu v0,_lo(data_14C)(v0)
    move    a0,a1
    sll v0,v0,2
    addu    a1,a0,v0
    slt v0,v1,a1
    bnez    v0,text_FD4
    slti    v0,s0,16
    slti    v0,v1,3291
    beqz    v0,text_FD0
    li  a1,3290
    move    a1,v1
text_FD0:
    slti    v0,s0,16
text_FD4:
    bnez    v0,text_FF0
    move    v1,a1
    slti    v0,s0,258
    beqz    v0,text_FF4
    li  a1,257
    j   text_FF4
    move    a1,s0
text_FF0:
    li  a1,16
text_FF4:
    move    s0,a1
    addiu   a1,s0,2
    slt v0,s2,a1
    bnez    v0,text_1018
    slti    v0,s2,259
    li  a1,258
text_100C:
    beqz    v0,text_1018
    nop
    move    a1,s2
text_1018:
    move    s2,a1
    andi    v1,v1,$0FFF
    sll v1,v1,12
    andi    a0,a0,$0FFF
    lui v0,$0600
    lui a1,_hi(data_48)
    lw  a1,_lo(data_48)(a1)
    or  a0,a0,v0
    lw  v0,16(a1)
    nop
    jalr    ra,v0
    or  a0,v1,a0
    andi    v1,s2,$03FF
    sll v1,v1,10
    andi    a0,s0,$03FF
    lui v0,$0700
    lui a1,_hi(data_48)
    lw  a1,_lo(data_48)(a1)
    or  a0,a0,v0
    lw  v0,16(a1)
    nop
    jalr    ra,v0
    or  a0,v1,a0
text_1074:
    lui a0,_hi(data_BC)
    addiu   a0,a0,_lo(data_BC)
    move    a1,s1
    jal memcpy
    li  a2,20
    move    v0,s1
    lw  ra,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

GetDispEnv:
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    lui a1,_hi(data_BC)
    addiu   a1,a1,_lo(data_BC)
    sw  ra,20(sp)
    jal memcpy
    li  a2,20
    move    v0,s0
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

GetODE:
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    addiu   sp,sp,-24
    sw  ra,16(sp)
    lw  v0,56(v0)
    nop
    jalr    ra,v0
    nop
    lw  ra,16(sp)
    srl v0,v0,31
    jr  ra
    addiu   sp,sp,24

SetDrawArea:
    addiu   sp,sp,-32
    sw  s1,20(sp)
    move    s1,a0
    sw  s0,16(sp)
    move    s0,a1
    li  v0,2
    sw  ra,24(sp)
    sb  v0,3(s1)
    lh  a0,0(s0)
    lh  a1,2(s0)
    jal text_1650
    nop
    sw  v0,4(s1)
    lhu a0,0(s0)
    lhu v0,4(s0)
    lhu a1,2(s0)
    addu    a0,a0,v0
    addiu   a0,a0,-1
    sll a0,a0,16
    lhu v0,6(s0)
    sra a0,a0,16
    addu    a1,a1,v0
    addiu   a1,a1,-1
    sll a1,a1,16
    jal text_16E8
    sra a1,a1,16
    sw  v0,8(s1)
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

SetDrawOffset:
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    li  v0,2
    sw  ra,20(sp)
    sb  v0,3(s0)
    lh  a0,0(a1)
    lh  a1,2(a1)
    jal text_1780
    nop
    sw  v0,4(s0)
    sw  zero,8(s0)
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

SetDrawEnv:
    addiu   sp,sp,-40
    sw  s0,24(sp)
    move    s0,a1
    sw  s1,28(sp)
    move    s1,a0
    sw  ra,32(sp)
    lh  a0,0(s0)
    lh  a1,2(s0)
    jal text_1650
    nop
    sw  v0,4(s1)
    lhu a0,4(s0)
    lhu v0,0(s0)
    lhu a1,2(s0)
    addu    a0,a0,v0
    addiu   a0,a0,-1
    sll a0,a0,16
    lhu v0,6(s0)
    sra a0,a0,16
    addu    a1,a1,v0
    addiu   a1,a1,-1
    sll a1,a1,16
    jal text_16E8
    sra a1,a1,16
    sw  v0,8(s1)
    lh  a0,8(s0)
    lh  a1,10(s0)
    jal text_1780
    nop
    sw  v0,12(s1)
    lbu a0,23(s0)
    lbu a1,22(s0)
    lhu a2,20(s0)
    jal text_1630
    nop
    addiu   a0,s0,12
    jal text_179C
    sw  v0,16(s1)
    sw  v0,20(s1)
    lui v0,$E600
    sw  v0,24(s1)
    lbu v0,24(s0)
    nop
    beqz    v0,text_13A4
    li  t0,7
    lhu v0,0(s0)
    nop
    sh  v0,16(sp)
    lhu v0,2(s0)
    nop
    sh  v0,18(sp)
    lhu a0,4(s0)
    nop
    sh  a0,20(sp)
    lhu v0,6(s0)
    nop
    sh  v0,22(sp)
    sll v0,a0,16
    sra v1,v0,16
    bltz    v1,text_12E4
    move    v0,zero
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,v1
    lui v1,_hi(data_54)
    lhu v1,_lo(data_54)(v1)
    bnez    v0,text_12E4
    addiu   v0,v1,-1
    move    v0,a0
text_12E4:
    lh  v1,22(sp)
    lhu a0,22(sp)
    bltz    v1,text_1320
    sh  v0,20(sp)
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,v1
    lui v1,_hi(data_56)
    lhu v1,_lo(data_56)(v1)
    bnez    v0,text_1324
    addiu   v0,v1,-1
    j   text_1324
    move    v0,a0
text_1320:
    move    v0,zero
text_1324:
    sll a2,t0,2
    addiu   t0,t0,1
    sll a3,t0,2
    addiu   t0,t0,1
    sll a1,t0,2
    addiu   t0,t0,1
    sh  v0,22(sp)
    lhu v0,16(sp)
    lhu v1,8(s0)
    addu    a2,a2,s1
    subu    v0,v0,v1
    sh  v0,16(sp)
    lhu v0,18(sp)
    lhu v1,10(s0)
    lui a0,$6000
    subu    v0,v0,v1
    sh  v0,18(sp)
    lbu v0,27(s0)
    lbu v1,26(s0)
    sll v0,v0,16
    sll v1,v1,8
    or  v1,v1,a0
    lbu a0,25(s0)
    or  v0,v0,v1
    or  v0,v0,a0
    sw  v0,0(a2)
    lw  v0,16(sp)
    addu    a3,a3,s1
    sw  v0,0(a3)
    lw  v0,20(sp)
    addu    a1,a1,s1
    sw  v0,0(a1)
text_13A4:
    addiu   v0,t0,-1
    sb  v0,3(s1)
    lw  ra,32(sp)
    lw  s1,28(sp)
    lw  s0,24(sp)
    jr  ra
    addiu   sp,sp,40
text_13C0:
    addiu   sp,sp,-40
    sw  s0,24(sp)
    move    s0,a1
    sw  s1,28(sp)
    move    s1,a0
    sw  ra,32(sp)
    lh  a0,0(s0)
    lh  a1,2(s0)
    jal text_1650
    nop
    sw  v0,4(s1)
    lhu a0,4(s0)
    lhu v0,0(s0)
    lhu a1,2(s0)
    addu    a0,a0,v0
    addiu   a0,a0,-1
    sll a0,a0,16
    lhu v0,6(s0)
    sra a0,a0,16
    addu    a1,a1,v0
    addiu   a1,a1,-1
    sll a1,a1,16
    jal text_16E8
    sra a1,a1,16
    sw  v0,8(s1)
    lh  a0,8(s0)
    lh  a1,10(s0)
    jal text_1780
    nop
    sw  v0,12(s1)
    lbu a0,23(s0)
    lbu a1,22(s0)
    lhu a2,20(s0)
    jal text_1630
    nop
    addiu   a0,s0,12
    jal text_179C
    sw  v0,16(s1)
    sw  v0,20(s1)
    lui v0,$E600
    sw  v0,24(s1)
    lbu v0,24(s0)
    nop
    beqz    v0,text_1614
    li  t0,7
    lhu v0,0(s0)
    nop
    sh  v0,16(sp)
    lhu v0,2(s0)
    nop
    sh  v0,18(sp)
    lhu a0,4(s0)
    nop
    sh  a0,20(sp)
    lhu v0,6(s0)
    nop
    sh  v0,22(sp)
    sll v0,a0,16
    sra v1,v0,16
    bltz    v1,text_14DC
    move    v0,zero
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,v1
    lui v1,_hi(data_54)
    lhu v1,_lo(data_54)(v1)
    bnez    v0,text_14DC
    addiu   v0,v1,-1
    move    v0,a0
text_14DC:
    lh  v1,22(sp)
    lhu a0,22(sp)
    bltz    v1,text_1518
    sh  v0,20(sp)
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,v1
    lui v1,_hi(data_56)
    lhu v1,_lo(data_56)(v1)
    bnez    v0,text_151C
    addiu   v0,v1,-1
    j   text_151C
    move    v0,a0
text_1518:
    move    v0,zero
text_151C:
    lhu v1,16(sp)
    sh  v0,22(sp)
    andi    v0,v1,$003F
    bnez    v0,text_1544
    sll a2,t0,2
    lhu v0,20(sp)
    nop
    andi    v0,v0,$003F
    beqz    v0,text_15BC
    sll a1,t0,2
text_1544:
    addiu   t0,t0,1
    sll a3,t0,2
    addiu   t0,t0,1
    sll a1,t0,2
    addiu   t0,t0,1
    lhu v0,8(s0)
    addu    a2,a2,s1
    subu    v0,v1,v0
    sh  v0,16(sp)
    lhu v0,18(sp)
    lhu v1,10(s0)
    lui a0,$6000
    subu    v0,v0,v1
    sh  v0,18(sp)
    lbu v0,27(s0)
    lbu v1,26(s0)
    sll v0,v0,16
    sll v1,v1,8
    or  v1,v1,a0
    lbu a0,25(s0)
    or  v0,v0,v1
    or  v0,v0,a0
    sw  v0,0(a2)
    lw  v0,16(sp)
    addu    a3,a3,s1
    sw  v0,0(a3)
    lw  v0,20(sp)
    addu    a1,a1,s1
    j   text_1614
    sw  v0,0(a1)
text_15BC:
    addiu   t0,t0,1
    sll a2,t0,2
    addiu   t0,t0,1
    sll a3,t0,2
    addiu   t0,t0,1
    addu    a1,a1,s1
    lui a0,$0200
    lbu v0,27(s0)
    lbu v1,26(s0)
    sll v0,v0,16
    sll v1,v1,8
    or  v1,v1,a0
    lbu a0,25(s0)
    or  v0,v0,v1
    or  v0,v0,a0
    sw  v0,0(a1)
    lw  v0,16(sp)
    addu    a2,a2,s1
    sw  v0,0(a2)
    lw  v0,20(sp)
    addu    a3,a3,s1
    sw  v0,0(a3)
text_1614:
    addiu   v0,t0,-1
    sb  v0,3(s1)
    lw  ra,32(sp)
    lw  s1,28(sp)
    lw  s0,24(sp)
    jr  ra
    addiu   sp,sp,40
text_1630:
    beqz    a1,text_163C
    lui v1,$E100
    ori v1,v1,$0200
text_163C:
    beqz    a0,text_1648
    andi    v0,a2,$09FF
    ori v0,v0,$0400
text_1648:
    jr  ra
    or  v0,v1,v0
text_1650:
    sll v0,a0,16
    sra a2,v0,16
    bltz    a2,text_1688
    move    v0,zero
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,a2
    lui a2,_hi(data_54)
    lhu a2,_lo(data_54)(a2)
    bnez    v0,text_1688
    addiu   v0,a2,-1
    move    v0,a0
text_1688:
    move    a0,v0
    sll v0,a1,16
    sra a2,v0,16
    bltz    a2,text_16C8
    nop
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,a2
    lui a2,_hi(data_56)
    lhu a2,_lo(data_56)(a2)
    beqz    v0,text_16D0
    andi    v1,a1,$03FF
    j   text_16CC
    addiu   a1,a2,-1
text_16C8:
    move    a1,zero
text_16CC:
    andi    v1,a1,$03FF
text_16D0:
    sll v1,v1,10
    andi    v0,a0,$03FF
    lui a0,$E300
    or  v0,v0,a0
    jr  ra
    or  v0,v1,v0
text_16E8:
    sll v0,a0,16
    sra a2,v0,16
    bltz    a2,text_1720
    move    v0,zero
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,a2
    lui a2,_hi(data_54)
    lhu a2,_lo(data_54)(a2)
    bnez    v0,text_1720
    addiu   v0,a2,-1
    move    v0,a0
text_1720:
    move    a0,v0
    sll v0,a1,16
    sra a2,v0,16
    bltz    a2,text_1760
    nop
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,a2
    lui a2,_hi(data_56)
    lhu a2,_lo(data_56)(a2)
    beqz    v0,text_1768
    andi    v1,a1,$03FF
    j   text_1764
    addiu   a1,a2,-1
text_1760:
    move    a1,zero
text_1764:
    andi    v1,a1,$03FF
text_1768:
    sll v1,v1,10
    andi    v0,a0,$03FF
    lui a0,$E400
    or  v0,v0,a0
    jr  ra
    or  v0,v1,v0

text_1780:
    andi    a1,a1,$07FF
    sll a1,a1,11
    andi    v0,a0,$07FF
    lui v1,$E500
    or  v0,v0,v1
    jr  ra
    or  v0,a1,v0

text_179C:
    bnez    a0,text_17AC
    addiu   sp,sp,-16
    j   text_1814
    move    v0,zero
text_17AC:
    lbu a1,0(a0)
    nop
    srl a1,a1,3
    sw  a1,0(sp)
    lh  a2,4(a0)
    nop
    subu    a2,zero,a2
    andi    a2,a2,$00FF
    sra a2,a2,3
    sw  a2,8(sp)
    lbu v0,2(a0)
    sll a1,a1,10
    srl v0,v0,3
    sw  v0,4(sp)
    sll v0,v0,15
    lh  v1,6(a0)
    lui a0,$E200
    or  a1,a1,a0
    or  v0,v0,a1
    subu    v1,zero,v1
    andi    v1,v1,$00FF
    sra v1,v1,3
    sll a0,v1,5
    or  v0,v0,a0
    or  v0,v0,a2
    sw  v1,12(sp)
text_1814:
    jr  ra
    addiu   sp,sp,16

readGP1
text_181C:
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)        return *gp1;
    nop
    lw  v0,0(v0)
    jr  ra
    nop

text_1834:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a1
    lui a1,_hi(data_174)
    lw  a1,_lo(data_174)(a1)
    sw  ra,24(sp)
    sw  s1,20(sp)
    lw  v0,0(a1)
    lui v1,$0800
    or  v0,v0,v1
    sw  v0,0(a1)
    lui v0,_hi(data_170)
    lw  v0,_lo(data_170)(v0)
    nop
    sw  zero,0(v0)
    sll v0,s0,2
    addiu   v0,v0,-4
    lui v1,_hi(data_168)
    lw  v1,_lo(data_168)(v1)
    addu    a0,a0,v0
    sw  a0,0(v1)
    lui v0,_hi(data_16C)
    lw  v0,_lo(data_16C)(v0)
    lui v1,$1100
    sw  s0,0(v0)
    lui v0,_hi(data_170)
    lw  v0,_lo(data_170)(v0)
    ori v1,v1,$0002
    jal text_2880
    sw  v1,0(v0)
    lui v0,_hi(data_170)
    lw  v0,_lo(data_170)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
    and v0,v0,v1
    beqz    v0,text_1900
    move    v0,s0
    lui s1,$0100
text_18D0:
    jal text_28B4
    nop
    bnez    v0,text_1900
    li  v0,-1
    lui v0,_hi(data_170)
    lw  v0,_lo(data_170)(v0)
    nop
    lw  v0,0(v0)
    nop
    and v0,v0,s1
    bnez    v0,text_18D0
    move    v0,s0
text_1900:
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32
text_1914:
    addiu   sp,sp,-32
    move    t0,a0
    sw  ra,28(sp)
    sw  s2,24(sp)
    sw  s1,20(sp)
    sw  s0,16(sp)
    lh  v1,4(t0)
    lhu a0,4(t0)
    bltz    v1,text_1968
    move    t1,a1
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,v1
    lui v1,_hi(data_54)
    lhu v1,_lo(data_54)(v1)
    bnez    v0,text_196C
    addiu   v0,v1,-1
    j   text_196C
    move    v0,a0
text_1968:
    move    v0,zero
text_196C:
    lh  v1,6(t0)
    lhu a0,6(t0)
    bltz    v1,text_19A8
    sh  v0,4(t0)
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    nop
    addiu   v0,v0,-1
    slt v0,v0,v1
    lui v1,_hi(data_56)
    lhu v1,_lo(data_56)(v1)
    bnez    v0,text_19AC
    addiu   v1,v1,-1
    j   text_19AC
    move    v1,a0
text_19A8:
    move    v1,zero
text_19AC:
    lhu v0,0(t0)
    nop
    andi    v0,v0,$003F
    bnez    v0,text_19D4
    sh  v1,6(t0)
    lhu v0,4(t0)
    nop
    andi    v0,v0,$003F
    beqz    v0,text_1AAC
    lui v0,$05FF
text_19D4:
    lui a2,$00FF
    ori a2,a2,$FFFF
    lui a0,$E4FF
    ori a0,a0,$FFFF
    lui a3,$03FF
    lui a1,_hi(sectbase(.bss))
    addiu   a1,a1,_lo(sectbase(.bss))
    lui s0,_hi(bss_28)
    addiu   s0,s0,_lo(bss_28)
    and v0,s0,a2
    lui v1,$0800
    or  v0,v0,v1
    lui s1,$E300
    lui s2,$E500
    sw  v0,0(a1)
    lui v0,$E600
    and a2,t1,a2
    lui v1,$6000
    or  a2,a2,v1
    sw  v0,16(a1)
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    lui v1,$E100
    sw  s1,4(a1)
    sw  a0,8(a1)
    sw  s2,12(a1)
    lw  a0,0(v0)
    srl v0,t1,31
    sll v0,v0,10
    or  v0,v0,v1
    sw  a2,24(a1)
    andi    a0,a0,$07FF
    or  a0,a0,v0
    sw  a0,20(a1)
    lw  v0,0(t0)
    ori a3,a3,$FFFF
    sw  v0,28(a1)
    lw  v0,4(t0)
    li  a0,3
    sw  a3,0(s0)
    jal text_20A4
    sw  v0,32(a1)
    li  a0,4
    or  v0,v0,s1
    jal text_20A4
    sw  v0,4(s0)
    li  a0,5
    lui v1,$E400
    or  v0,v0,v1
    jal text_20A4
    sw  v0,8(s0)
    or  v0,v0,s2
    j   text_1B1C
    sw  v0,12(s0)
text_1AAC:
    ori v0,v0,$FFFF
    lui v1,$00FF
    ori v1,v1,$FFFF
    lui a2,_hi(sectbase(.bss))
    addiu   a2,a2,_lo(sectbase(.bss))
    sw  v0,0(a2)
    lui v0,$E600
    and v1,t1,v1
    lui a1,$0200
    sw  v0,4(a2)
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    or  v1,v1,a1
    lw  a0,0(v0)
    srl v0,t1,31
    sll v0,v0,10
    sw  v1,12(a2)
    lui v1,$E100
    or  v0,v0,v1
    andi    a0,a0,$07FF
    or  a0,a0,v0
    sw  a0,8(a2)
    lw  v0,0(t0)
    nop
    sw  v0,16(a2)
    lw  v0,4(t0)
    nop
    sw  v0,20(a2)
text_1B1C:
    lui a0,_hi(sectbase(.bss))
    jal text_205C
    addiu   a0,a0,_lo(sectbase(.bss))
    move    v0,zero
    lw  ra,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32
text_1B44:
    addiu   sp,sp,-48
    sw  s1,20(sp)
    move    s1,a0
    sw  s2,24(sp)
    move    s2,a1
    sw  ra,40(sp)
    sw  s5,36(sp)
    sw  s4,32(sp)
    sw  s3,28(sp)
    jal text_2880
    sw  s0,16(sp)
    lh  a1,4(s1)
    lhu v1,4(s1)
    bltz    a1,text_1BA8
    move    s5,zero
    move    a0,v1
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    lui v1,_hi(data_54)
    lhu v1,_lo(data_54)(v1)
    slt v0,v0,a1
    beqz    v0,text_1BAC
    nop
    j   text_1BAC
    move    a0,v1
text_1BA8:
    move    a0,zero
text_1BAC:
    lh  a1,6(s1)
    lhu v1,6(s1)
    bltz    a1,text_1BE4
    sh  a0,4(s1)
    move    a0,v1
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    lui v1,_hi(data_56)
    lhu v1,_lo(data_56)(v1)
    slt v0,v0,a1
    beqz    v0,text_1BEC
    sll v0,a0,16
    j   text_1BE8
    move    a0,v1
text_1BE4:
    move    a0,zero
text_1BE8:
    sll v0,a0,16
text_1BEC:
    lh  v1,4(s1)
    sra v0,v0,16
    mult    v1,v0
    sh  a0,6(s1)
    mflo    a2
    addiu   v1,a2,1
    srl v0,v1,31
    addu    v1,v1,v0
    sra a0,v1,1
    bgtz    a0,text_1C20
    sra s0,v1,5
    j   text_1D5C
    li  v0,-1
text_1C20:
    move    v1,s0
    sll v0,v1,4
    subu    s0,a0,v0
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    move    s4,v1
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    bnez    v0,text_1C80
    lui a0,$A000
    lui s3,$0400
text_1C50:
    jal text_28B4
    nop
    bnez    v0,text_1D5C
    li  v0,-1
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    nop
    and v0,v0,s3
    beqz    v0,text_1C50
    lui a0,$A000
text_1C80:
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0400
    sw  v0,0(v1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lui v0,$0100
    sw  v0,0(v1)
    lui v0,_hi(data_154)
    lw  v0,_lo(data_154)(v0)
    beqz    s5,text_1CB4
    nop
    lui a0,$B000
text_1CB4:
    sw  a0,0(v0)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lw  v0,0(s1)
    nop
    sw  v0,0(v1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lw  v0,4(s1)
    addiu   s0,s0,-1
    sw  v0,0(v1)
    li  v0,-1
    beq s0,v0,text_1D0C
    nop
    li  a0,-1
text_1CF0:
    lw  v1,0(s2)
    addiu   s2,s2,4
    lui v0,_hi(data_154)
    lw  v0,_lo(data_154)(v0)
    addiu   s0,s0,-1
    bne s0,a0,text_1CF0
    sw  v1,0(v0)
text_1D0C:
    beqz    s4,text_1D58
    lui v1,$0400
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    ori v1,v1,$0002
    sw  v1,0(v0)
    lui v0,_hi(data_15C)
    lw  v0,_lo(data_15C)(v0)
    lui a0,$0100
    sw  s2,0(v0)
    sll v0,s4,16
    lui v1,_hi(data_160)
    lw  v1,_lo(data_160)(v1)
    ori v0,v0,$0010
    sw  v0,0(v1)
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    ori a0,a0,$0201
    sw  a0,0(v0)
text_1D58:
    move    v0,zero
text_1D5C:
    lw  ra,40(sp)
    lw  s5,36(sp)
    lw  s4,32(sp)
    lw  s3,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,48
text_1D80:
    addiu   sp,sp,-40
    sw  s1,20(sp)
    move    s1,a0
    sw  s2,24(sp)
    move    s2,a1
    sw  ra,36(sp)
    sw  s4,32(sp)
    sw  s3,28(sp)
    jal text_2880
    sw  s0,16(sp)
    lh  a1,4(s1)
    lhu v1,4(s1)
    bltz    a1,text_1DDC
    move    a0,v1
    lui v0,_hi(data_54)
    lh  v0,_lo(data_54)(v0)
    lui v1,_hi(data_54)
    lhu v1,_lo(data_54)(v1)
    slt v0,v0,a1
    beqz    v0,text_1DE0
    nop
    j   text_1DE0
    move    a0,v1
text_1DDC:
    move    a0,zero
text_1DE0:
    lh  a1,6(s1)
    lhu v1,6(s1)
    bltz    a1,text_1E18
    sh  a0,4(s1)
    move    a0,v1
    lui v0,_hi(data_56)
    lh  v0,_lo(data_56)(v0)
    lui v1,_hi(data_56)
    lhu v1,_lo(data_56)(v1)
    slt v0,v0,a1
    beqz    v0,text_1E20
    sll v0,a0,16
    j   text_1E1C
    move    a0,v1
text_1E18:
    move    a0,zero
text_1E1C:
    sll v0,a0,16
text_1E20:
    lh  v1,4(s1)
    sra v0,v0,16
    mult    v1,v0
    sh  a0,6(s1)
    mflo    a2
    addiu   v1,a2,1
    srl v0,v1,31
    addu    v1,v1,v0
    sra a0,v1,1
    bgtz    a0,text_1E54
    sra s0,v1,5
    j   text_1FE0
    li  v0,-1
text_1E54:
    move    v1,s0
    sll v0,v1,4
    subu    s0,a0,v0
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    move    s4,v1
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    bnez    v0,text_1EB4
    nop
    lui s3,$0400
text_1E84:
    jal text_28B4
    nop
    bnez    v0,text_1FE0
    li  v0,-1
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    nop
    and v0,v0,s3
    beqz    v0,text_1E84
    nop
text_1EB4:
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0400
    sw  v0,0(v1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lui v0,$0100
    sw  v0,0(v1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lui v0,$C000
    sw  v0,0(v1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lw  v0,0(s1)
    nop
    sw  v0,0(v1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    lw  v0,4(s1)
    nop
    sw  v0,0(v1)
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0800
    and v0,v0,v1
    bnez    v0,text_1F60
    nop
    lui s1,$0800
text_1F30:
    jal text_28B4
    nop
    bnez    v0,text_1FE0
    li  v0,-1
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    nop
    and v0,v0,s1
    beqz    v0,text_1F30
    nop
text_1F60:
    addiu   s0,s0,-1
    li  v0,-1
    beq s0,v0,text_1F90
    li  v1,-1
text_1F70:
    lui v0,_hi(data_154)
    lw  v0,_lo(data_154)(v0)
    nop
    lw  v0,0(v0)
    addiu   s0,s0,-1
    sw  v0,0(s2)
    bne s0,v1,text_1F70
    addiu   s2,s2,4
text_1F90:
    beqz    s4,text_1FDC
    lui v1,$0400
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    ori v1,v1,$0003
    sw  v1,0(v0)
    lui v0,_hi(data_15C)
    lw  v0,_lo(data_15C)(v0)
    lui a0,$0100
    sw  s2,0(v0)
    sll v0,s4,16
    lui v1,_hi(data_160)
    lw  v1,_lo(data_160)(v1)
    ori v0,v0,$0010
    sw  v0,0(v1)
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    ori a0,a0,$0200
    sw  a0,0(v0)
text_1FDC:
    move    v0,zero
text_1FE0:
    lw  ra,36(sp)
    lw  s4,32(sp)
    lw  s3,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,40

writeGP1
text_2000:
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    jr  ra
    sw  a0,0(v0)

text_2014:
    jr  ra
    nop

sendPrimIO.
text_201C:
    addiu   a2,a1,-1
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0400
    beqz    a1,text_2054
    sw  v0,0(v1)
    li  a1,-1
text_2038:
    lw  v1,0(a0)
    addiu   a0,a0,4
    lui v0,_hi(data_154)
    lw  v0,_lo(data_154)(v0)
    addiu   a2,a2,-1
    bne a2,a1,text_2038
    sw  v1,0(v0)
text_2054:
    jr  ra
    move    v0,zero

text_205C:
    lui v1,$0400
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    ori v1,v1,$0002
    sw  v1,0(v0)
    lui v0,_hi(data_15C)
    lw  v0,_lo(data_15C)(v0)
    nop
    sw  a0,0(v0)
    lui v0,_hi(data_160)
    lw  v0,_lo(data_160)(v0)
    lui v1,$0100
    sw  zero,0(v0)
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    ori v1,v1,$0401
    jr  ra
    sw  v1,0(v0)
text_20A4:
    lui v0,$1000
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    or  a0,a0,v0
    sw  a0,0(v1)
    lui v0,_hi(data_154)
    lw  v0,_lo(data_154)(v0)
    lui v1,$00FF
    lw  v0,0(v0)
    ori v1,v1,$FFFF
    jr  ra
    and v0,v0,v1

text_20D4:
    addiu   sp,sp,-24
    sw  ra,16(sp)
    move    a3,a2
    jal text_20F8
    move    a2,zero
    lw  ra,16(sp)
    addiu   sp,sp,24
    jr  ra
    nop

text_20F8:
    addiu   sp,sp,-40
    sw  s3,28(sp)
    move    s3,a0
    sw  s0,16(sp)
    move    s0,a1
    sw  s1,20(sp)
    move    s1,a2
    sw  s2,24(sp)
    sw  ra,32(sp)
    jal text_2880
    move    s2,a3
    j   text_2144
    nop
text_212C:
    jal text_28B4
    nop
    bnez    v0,text_238C
    li  v0,-1
    jal text_23A8
    nop
text_2144:
    lui v0,_hi(_qin)
    lw  v0,_lo(_qin)(v0)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    addiu   v0,v0,1
    andi    v0,v0,$003F
    beq v0,v1,text_212C
    nop
    jal SetIntrMask
    move    a0,zero
    lui a0,_hi(GEnv)
    addiu   a0,a0,_lo(GEnv)
    lui at,_hi(data_180)
    sw  v0,_lo(data_180)(at)
    lbu v1,1(a0)
    li  v0,1
    beqz    v1,text_21D8
    sw  v0,8(a0)
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    lui v0,_hi(_qout)
    lw  v0,_lo(_qout)(v0)
    nop
    bne v1,v0,text_221C
    nop
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
    and v0,v0,v1
    bnez    v0,text_221C
    nop
    lw  v0,12(a0)
    nop
    bnez    v0,text_221C
    nop
text_21D8:
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui a0,$0400
text_21E4:
    lw  v0,0(v1)
    nop
    and v0,v0,a0
    beqz    v0,text_21E4
    nop
    move    a0,s0
    jalr    ra,s3
    move    a1,s2
    lui a0,_hi(data_180)
    lw  a0,_lo(data_180)(a0)
    jal SetIntrMask
    nop
    j   text_238C
    move    v0,zero
text_221C:
    lui a1,_hi(text_23A8)
    addiu   a1,a1,_lo(text_23A8)
    jal DMACallback
    li  a0,2
    beqz    s1,text_22D8
    move    a2,zero
    lui t0,_hi(_que+$C)
    addiu   t0,t0,_lo(_que+$C)
    move    a3,s0
    move    v0,s1
text_2244:
    bgez    v0,text_2250
    nop
    addiu   v0,v0,3
text_2250:
    sra v0,v0,2
    slt v0,a2,v0
    beqz    v0,text_2294
    sll a0,a2,2
    lw  a1,0(a3)
    addiu   a3,a3,4
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    addiu   a2,a2,1
    sll v0,v1,1
    addu    v0,v0,v1
    sll v0,v0,5
    addu    v0,v0,t0
    addu    a0,a0,v0
    sw  a1,0(a0)
    j   text_2244
    move    v0,s1
text_2294:
    lui v0,_hi(_qin)
    lw  v0,_lo(_qin)(v0)
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    sll a0,v0,1
    addu    a0,a0,v0
    sll a0,a0,5
    sll v0,v1,1
    addu    v0,v0,v1
    sll v0,v0,5
    lui v1,_hi(_que+$C)
    addiu   v1,v1,_lo(_que+$C)
    addu    v0,v0,v1
    lui at,_hi(_que+$4)
    addu    at,at,a0
    j   text_22FC
    sw  v0,_lo(_que+$4)(at)
text_22D8:
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    nop
    sll v0,v1,1
    addu    v0,v0,v1
    sll v0,v0,5
    lui at,_hi(_que+$4)
    addu    at,at,v0
    sw  s0,_lo(_que+$4)(at)
text_22FC:
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    nop
    sll v0,v1,1
    addu    v0,v0,v1
    sll v0,v0,5
    lui at,_hi(_que+$8)
    addu    at,at,v0
    sw  s2,_lo(_que+$8)(at)
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    nop
    sll v0,v1,1
    addu    v0,v0,v1
    sll v0,v0,5
    lui at,_hi(_que)
    addu    at,at,v0
    sw  s3,_lo(_que)(at)
    lui v0,_hi(_qin)
    lw  v0,_lo(_qin)(v0)
    lui a0,_hi(data_180)
    lw  a0,_lo(data_180)(a0)
    addiu   v0,v0,1
    andi    v0,v0,$003F
    lui at,_hi(_qin)
    jal SetIntrMask
    sw  v0,_lo(_qin)(at)
    jal text_23A8
    nop
    lui v0,_hi(_qin)
    lw  v0,_lo(_qin)(v0)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    nop
    subu    v0,v0,v1
    andi    v0,v0,$003F
text_238C:
    lw  ra,32(sp)
    lw  s3,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,40

updateQueue
text_23A8:
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    addiu   sp,sp,-32
    sw  ra,24(sp)
    sw  s1,20(sp)
    sw  s0,16(sp)
    lw  v0,0(v0)
    lui s0,$0100
    and v0,v0,s0
    bnez    v0,text_25F4
    li  v0,1
    jal SetIntrMask
    move    a0,zero
    lui a0,_hi(_qin)
    lw  a0,_lo(_qin)(a0)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    lui at,_hi(data_184)
    beq a0,v1,text_2558
    sw  v0,_lo(data_184)(at)
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    nop
    and v0,v0,s0
    bnez    v0,text_2558
    nop
    lui s1,$0400
    lui s0,$0100
text_2420:
    lui v0,_hi(_qout)
    lw  v0,_lo(_qout)(v0)
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    addiu   v0,v0,1
    andi    v0,v0,$003F
    bne v0,v1,text_245C
    nop
    lui v0,_hi(data_5C)
    lw  v0,_lo(data_5C)(v0)
    nop
    bnez    v0,text_245C
    li  a0,2
    jal DMACallback
    move    a1,zero
text_245C:
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    nop
    lw  v0,0(v1)
    nop
    and v0,v0,s1
    bnez    v0,text_2490
    lui a0,$0400
text_247C:
    lw  v0,0(v1)
    nop
    and v0,v0,a0
    beqz    v0,text_247C
    nop
text_2490:
    lui a1,_hi(_qout)
    lw  a1,_lo(_qout)(a1)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    nop
    sll v0,v1,1
    addu    v0,v0,v1
    sll v0,v0,5
    sll v1,a1,1
    addu    v1,v1,a1
    lui a0,_hi(_que+$4)
    addu    a0,a0,v0
    lw  a0,_lo(_que+$4)(a0)
    lui a1,_hi(_qout)
    lw  a1,_lo(_qout)(a1)
    sll v1,v1,5
    sll v0,a1,1
    addu    v0,v0,a1
    sll v0,v0,5
    lui a1,_hi(_que+$8)
    addu    a1,a1,v0
    lw  a1,_lo(_que+$8)(a1)
    lui v0,_hi(_que)
    addu    v0,v0,v1
    lw  v0,_lo(_que)(v0)
    nop
    jalr    ra,v0
    nop
    lui v0,_hi(_qout)
    lw  v0,_lo(_qout)(v0)
    nop
    addiu   v0,v0,1
    andi    v0,v0,$003F
    lui at,_hi(_qout)
    sw  v0,_lo(_qout)(at)
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    lui v0,_hi(_qout)
    lw  v0,_lo(_qout)(v0)
    nop
    beq v1,v0,text_2558
    nop
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    nop
    and v0,v0,s0
    beqz    v0,text_2420
    nop
text_2558:
    lui a0,_hi(data_184)
    lw  a0,_lo(data_184)(a0)
    jal SetIntrMask
    nop
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    lui v0,_hi(_qout)
    lw  v0,_lo(_qout)(v0)
    nop
    bne v1,v0,text_25D8
    nop
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
    and v0,v0,v1
    bnez    v0,text_25D8
    nop
    lui v1,_hi(data_58)
    addiu   v1,v1,_lo(data_58)
    lw  v0,0(v1)
    nop
    beqz    v0,text_25D8
    nop
    lw  a0,4(v1)
    nop
    beqz    a0,text_25D8
    addiu   v0,v1,-8
    sw  zero,8(v0)
    jalr    ra,a0
    nop
text_25D8:
    lui v0,_hi(_qin)
    lw  v0,_lo(_qin)(v0)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    nop
    subu    v0,v0,v1
    andi    v0,v0,$003F
text_25F4:
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

resetGPU
text_2608:
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    sw  ra,20(sp)
    jal SetIntrMask
    move    a0,zero
    lui at,_hi(_qout)
    sw  zero,_lo(_qout)(at)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    lui at,_hi(data_188)
    sw  v0,_lo(data_188)(at)
    li  v0,1
    lui at,_hi(_qin)
    sw  v1,_lo(_qin)(at)
    andi    v1,s0,$0007
    beq v1,v0,text_26C4
    slti    v0,v1,2
    beqz    v0,text_2668
    li  v0,3
    beqz    v1,text_2678
    nop
    j   text_2710
    nop
text_2668:
    beq v1,v0,text_26C4
    li  v0,5
    bne v1,v0,text_2710
    nop
text_2678:
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    li  v0,1025
    sw  v0,0(v1)
    lui v1,_hi(data_174)
    lw  v1,_lo(data_174)(v1)
    lui a0,_hi(_que)
    addiu   a0,a0,_lo(_que)
    lw  v0,0(v1)
    move    a1,zero
    ori v0,v0,$0800
    sw  v0,0(v1)
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    li  a2,6144
    jal text_2ED8
    sw  zero,0(v0)
    j   text_2710
    nop
text_26C4:
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    li  v0,1025
    sw  v0,0(v1)
    lui v1,_hi(data_174)
    lw  v1,_lo(data_174)(v1)
    nop
    lw  v0,0(v1)
    nop
    ori v0,v0,$0800
    sw  v0,0(v1)
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0200
    sw  v0,0(v1)
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0100
    sw  v0,0(v1)
text_2710:
    lui a0,_hi(data_188)
    lw  a0,_lo(data_188)(a0)
    jal SetIntrMask
    nop
    andi    v0,s0,$0007
    bnez    v0,text_2734
    move    v0,zero
    jal text_29F8
    move    a0,s0
text_2734:
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

drawSync
text_2744:
    addiu   sp,sp,-24
    sw  ra,20(sp)
    bnez    a0,text_27F8
    sw  s0,16(sp)
    jal text_2880
    nop
    j   text_277C
    nop
text_2764:
    jal text_23A8
    nop
    jal text_28B4
    nop
    bnez    v0,text_2870
    li  v0,-1
text_277C:
    lui v1,_hi(_qin)
    lw  v1,_lo(_qin)(v1)
    lui v0,_hi(_qout)
    lw  v0,_lo(_qout)(v0)
    nop
    beq v1,v0,text_27B0
    nop
    j   text_2764
    nop
text_27A0:
    jal text_28B4
    nop
    bnez    v0,text_2870
    li  v0,-1
text_27B0:
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
    and v0,v0,v1
    bnez    v0,text_27A0
    nop
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    beqz    v0,text_27A0
    move    v0,zero
    j   text_2870
    nop
text_27F8:
    lui v0,_hi(_qin)
    lw  v0,_lo(_qin)(v0)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    nop
    subu    v0,v0,v1
    andi    s0,v0,$003F
    beqz    s0,text_2824
    nop
    jal text_23A8
    nop
text_2824:
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
    and v0,v0,v1
    bnez    v0,text_2864
    nop
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    bnez    v0,text_2870
    move    v0,s0
text_2864:
    bnez    s0,text_2870
    move    v0,s0
    li  v0,1
text_2870:
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

clearTimeout
text_2880:
    addiu   sp,sp,-24
    sw  ra,16(sp)
    jal VSync
    li  a0,-1
    addiu   v0,v0,240
    lui at,_hi(data_18C)
    sw  v0,_lo(data_18C)(at)
    lui at,_hi(data_190)
    sw  zero,_lo(data_190)(at)
    lw  ra,16(sp)
    addiu   sp,sp,24
    jr  ra
    nop

checkTimeout
text_28B4:
    addiu   sp,sp,-32
    sw  ra,24(sp)
    jal VSync
    li  a0,-1
    lui v1,_hi(data_18C)
    lw  v1,_lo(data_18C)(v1)
    nop
    slt v1,v1,v0
    bnez    v1,text_2908
    nop
    lui v1,_hi(data_190)
    addiu   v1,v1,_lo(data_190)
    lw  v0,0(v1)
    nop
    move    a0,v0
    addiu   v0,v0,1
    sw  v0,0(v1)
    lui v0,$000F
    slt v0,v0,a0
    beqz    v0,text_29E4
    nop
text_2908:
    lui a2,_hi(data_158)
    lw  a2,_lo(data_158)(a2)
    lui a0,_hi(rdata_1E0)
    addiu   a0,a0,_lo(rdata_1E0)
    lw  v0,0(a2)
    lui a1,_hi(_qin)
    lw  a1,_lo(_qin)(a1)
    lui v0,_hi(data_15C)
    lw  v0,_lo(data_15C)(v0)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    lw  v0,0(v0)
    subu    a1,a1,v1
    sw  v0,16(sp)
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    lw  a2,0(a2)
    lw  a3,0(v0)
    jal printf
    andi    a1,a1,$003F
    jal SetIntrMask
    move    a0,zero
    lui at,_hi(_qout)
    sw  zero,_lo(_qout)(at)
    lui v1,_hi(_qout)
    lw  v1,_lo(_qout)(v1)
    lui at,_hi(data_188)
    sw  v0,_lo(data_188)(at)
    lui at,_hi(_qin)
    sw  v1,_lo(_qin)(at)
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    li  v0,1025
    sw  v0,0(v1)
    lui v1,_hi(data_174)
    lw  v1,_lo(data_174)(v1)
    nop
    lw  v0,0(v1)
    nop
    ori v0,v0,$0800
    sw  v0,0(v1)
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0200
    sw  v0,0(v1)
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    lui v0,$0100
    sw  v0,0(v1)
    lui a0,_hi(data_188)
    lw  a0,_lo(data_188)(a0)
    jal SetIntrMask
    nop
    j   text_29E8
    li  v0,-1
text_29E4:
    move    v0,zero
text_29E8:
    lw  ra,24(sp)
    addiu   sp,sp,32
    jr  ra
    nop

resetGPUVersion.
text_29F8:
    lui v1,$1000
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    ori v1,v1,$0007
    sw  v1,0(v0)
    lui a1,_hi(data_154)
    lw  a1,_lo(data_154)(a1)
    lui v1,$00FF
    lw  v0,0(a1)
    ori v1,v1,$FFFF
    and v0,v0,v1
    li  v1,2
    beq v0,v1,text_2A68
    lui v1,$E100
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    ori v1,v1,$1000
    andi    v0,v0,$3FFF
    or  v0,v0,v1
    sw  v0,0(a1)
    lui v1,_hi(data_154)
    lw  v1,_lo(data_154)(v1)
    move    v0,zero
    lw  v1,0(v1)
    j   text_2A90
    nop
text_2A68:
    andi    v0,a0,$0008
    beqz    v0,text_2A8C
    lui a0,$0900
    ori a0,a0,$0001
    lui v1,_hi(data_158)
    lw  v1,_lo(data_158)(v1)
    li  v0,2
    j   text_2A90
    sw  a0,0(v1)
text_2A8C:
    li  v0,1
text_2A90:
    jr  ra
    nop

LoadImage2:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s1,20(sp)
    move    s1,a1
    lui a0,_hi(rdata_214)
    addiu   a0,a0,_lo(rdata_214)
    sw  ra,24(sp)
    jal text_3E4
    move    a1,s0
    jal VSync
    li  a0,-1
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    addiu   v0,v0,240
    lui at,_hi(data_18C)
    sw  v0,_lo(data_18C)(at)
    lui at,_hi(data_190)
    sw  zero,_lo(data_190)(at)
    lw  v0,0(v1)
    j   text_2B14
    lui v1,$0100
text_2AF0:
    jal text_28B4
    nop
    bnez    v0,text_2B70
    li  v0,-1
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
text_2B14:
    and v0,v0,v1
    bnez    v0,text_2AF0
    nop
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    beqz    v0,text_2AF0
    nop
    lui a1,_hi(_GPU_ResetCallback)
    addiu   a1,a1,_lo(_GPU_ResetCallback)
    jal DMACallback
    li  a0,2
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    move    a0,s0
    lw  v0,32(v0)
    nop
    jalr    ra,v0
    move    a1,s1
    move    v0,zero
text_2B70:
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

StoreImage2:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s1,20(sp)
    move    s1,a1
    lui a0,_hi(rdata_138)
    addiu   a0,a0,_lo(rdata_138)
    sw  ra,24(sp)
    jal text_3E4
    move    a1,s0
    jal VSync
    li  a0,-1
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    addiu   v0,v0,240
    lui at,_hi(data_18C)
    sw  v0,_lo(data_18C)(at)
    lui at,_hi(data_190)
    sw  zero,_lo(data_190)(at)
    lw  v0,0(v1)
    j   text_2C00
    lui v1,$0100
text_2BDC:
    jal text_28B4
    nop
    bnez    v0,text_2C5C
    li  v0,-1
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
text_2C00:
    and v0,v0,v1
    bnez    v0,text_2BDC
    nop
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    beqz    v0,text_2BDC
    nop
    lui a1,_hi(_GPU_ResetCallback)
    addiu   a1,a1,_lo(_GPU_ResetCallback)
    jal DMACallback
    li  a0,2
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    move    a0,s0
    lw  v0,28(v0)
    nop
    jalr    ra,v0
    move    a1,s1
    move    v0,zero
text_2C5C:
    lw  ra,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

MoveImage2:
    addiu   sp,sp,-32
    sw  s0,16(sp)
    move    s0,a0
    sw  s2,24(sp)
    move    s2,a1
    sw  s1,20(sp)
    move    s1,a2
    lui a0,_hi(rdata_144)
    addiu   a0,a0,_lo(rdata_144)
    sw  ra,28(sp)
    jal text_3E4
    move    a1,s0
    jal VSync
    li  a0,-1
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    addiu   v0,v0,240
    lui at,_hi(data_18C)
    sw  v0,_lo(data_18C)(at)
    lui at,_hi(data_190)
    sw  zero,_lo(data_190)(at)
    lw  v0,0(v1)
    j   text_2CF4
    lui v1,$0100
text_2CD0:
    jal text_28B4
    nop
    bnez    v0,text_2D9C
    li  v0,-1
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
text_2CF4:
    and v0,v0,v1
    bnez    v0,text_2CD0
    nop
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    beqz    v0,text_2CD0
    nop
    lui a1,_hi(_GPU_ResetCallback)
    addiu   a1,a1,_lo(_GPU_ResetCallback)
    jal DMACallback
    li  a0,2
    lh  v0,4(s0)
    nop
    beqz    v0,text_2D9C
    li  v0,-1
    lh  v0,6(s0)
    nop
    bnez    v0,text_2D58
    sll v0,s1,16
    j   text_2D9C
    li  v0,-1
text_2D58:
    andi    v1,s2,$FFFF
    or  v0,v0,v1
    lw  a1,0(s0)
    lui v1,_hi(data_48)
    lw  v1,_lo(data_48)(v1)
    lui a0,_hi(data_F0)
    addiu   a0,a0,_lo(data_F0)
    sw  v0,4(a0)
    sw  a1,0(a0)
    lw  v0,4(s0)
    nop
    sw  v0,8(a0)
    lw  v0,24(v1)
    nop
    jalr    ra,v0
    addiu   a0,a0,-8
    move    v0,zero
text_2D9C:
    lw  ra,28(sp)
    lw  s2,24(sp)
    lw  s1,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,32

DrawOTag2:
    lui v0,_hi(data_52)
    lbu v0,_lo(data_52)(v0)
    addiu   sp,sp,-24
    sw  s0,16(sp)
    move    s0,a0
    sltiu   v0,v0,2
    bnez    v0,text_2DF0
    sw  ra,20(sp)
    lui a0,_hi(rdata_180)
    addiu   a0,a0,_lo(rdata_180)
    lui v0,_hi(GPU_printf)
    lw  v0,_lo(GPU_printf)(v0)
    nop
    jalr    ra,v0
    move    a1,s0
text_2DF0:
    jal VSync
    li  a0,-1
    lui v1,_hi(data_164)
    lw  v1,_lo(data_164)(v1)
    addiu   v0,v0,240
    lui at,_hi(data_18C)
    sw  v0,_lo(data_18C)(at)
    lui at,_hi(data_190)
    sw  zero,_lo(data_190)(at)
    lw  v0,0(v1)
    j   text_2E44
    lui v1,$0100
text_2E20:
    jal text_28B4
    nop
    bnez    v0,text_2EA0
    li  v0,-1
    lui v0,_hi(data_164)
    lw  v0,_lo(data_164)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0100
text_2E44:
    and v0,v0,v1
    bnez    v0,text_2E20
    nop
    lui v0,_hi(data_158)
    lw  v0,_lo(data_158)(v0)
    nop
    lw  v0,0(v0)
    lui v1,$0400
    and v0,v0,v1
    beqz    v0,text_2E20
    nop
    lui a1,_hi(_GPU_ResetCallback)
    addiu   a1,a1,_lo(_GPU_ResetCallback)
    jal DMACallback
    li  a0,2
    lui v0,_hi(data_48)
    lw  v0,_lo(data_48)(v0)
    nop
    lw  v0,24(v0)
    nop
    jalr    ra,v0
    move    a0,s0
    move    v0,zero
text_2EA0:
    lw  ra,20(sp)
    lw  s0,16(sp)
    jr  ra
    addiu   sp,sp,24

_GPU_ResetCallback:
    addiu   sp,sp,-24
    sw  ra,16(sp)
    lui a1,_hi(text_23A8)
    addiu   a1,a1,_lo(text_23A8)
    jal DMACallback
    li  a0,2
    lw  ra,16(sp)
    addiu   sp,sp,24
    jr  ra
    nop

; memset
text_2ED8:
    beqz    a2,text_2EF4
    addiu   v0,a2,-1
    li  v1,-1
text_2EE4:
    sb  a1,0(a0)
    addiu   v0,v0,-1
    bne v0,v1,text_2EE4
    addiu   a0,a0,1
text_2EF4:
    jr  ra
    nop
    nop




    SECTION .data

    XDEF    _qout
    XDEF    _qin
    XDEF    GEnv
    XDEF    GPU_printf

// GPU initialization commands.
    DW  $00077350,$00460000

// Jump table.
jtb:
data_8:
0   DW  $00002F00,          "$Id: sys.c,v 1.140 1998/01/12 07:52:27 noda Exp yos $"
4       $000020D4,
8       $000020F8,          ClearImage ClearImage2
C       $00001914,          ClearImage ClearImage2
10      $00002000,          writeGP1
14      $0000201C           sendPrimIO
18  DW  $0000205C,          MoveImage DrawOTag
1C      $00001D80,          StoreImage
20      $00001B44,          LoadImage
24      $000023A8,
28      $00002014,
2C      $00001834           ClearOTagR
30  DW  $000020A4,
34      $00002608,          resetGPU
38      $0000181C,          readGP1
3C      $00002744           drawSync

static GPUCB *gpuCB = &jtb;
data_48:
    DW  $00003128



GPU_printf:
    DW  $00000000

// Size = 128
GEnv:
    DW  $00000000
    u_char  gpuType;
    u_char  que;
    u_char  debugLevel;
    u_char  reverse;

data_54:
    DW  $00000000
data_58:
    DW  $00000000
data_5C:
    DW  $00000000       drawSyncCB
data_60:
    DW  $00000000,$00000000,$00000000,$00000000,$00000000,$00000000
    DW  $00000000,$00000000,$00000000,$00000000,$00000000,$00000000
    DW  $00000000,$00000000,$00000000,$00000000,$00000000,$00000000
    DW  $00000000,$00000000,$00000000,$00000000,$00000000
data_BC:
    DW  $00000000,$00000000
data_C4:
    DW  $00000000,$00000000,$00000000

===============================

data_D0:
    DW  $00000400,$00000400,$00000400
data_DC:
    DW  $00000200,$00000200,$00000400,$04FFFFFF,$80000000
data_F0:
    DW  $00000000,$00000000,$00000000
data_FC:
    DW  $04FFFFFF,$00000000,$00000000,$00000000,$00000000
data_110:
    DW  $00000000,$80000000,$00000000,$00000000,$00010002
data_124:
    DW  $0C4E024E,$0C580258,$0C9B021B,$0C670267,$0C6C026C,$0C620262
    DW  $0C700270,$0CB00230,$0C7B027B,$0C800280
data_14C:
    DW  $0507080A,$00000004

data_154:
    DW  $1F801810               gp0
data_158:
    DW  $1F801814               gp1
data_15C:
    DW  $1F8010A0               gpu_dma_madr
data_160:
    DW  $1F8010A4               gpu_dma_bcr
data_164:
    DW  $1F8010A8               gpu_dma_chcr
data_168:
    DW  $1F8010E0               gpu_ot_madr
data_16C:
    DW  $1F8010E4               gpu_ot_bcr
data_170:
    DW  $1F8010E8               gpu_ot_chcr
data_174:
    DW  $1F8010F0               dma_pcr

_qin:
    DW  $00000000
_qout:
    DW  $00000000

intmask[0] (insertQueue)
data_180:
    DW  $00000000

intmask; (updateQueue)
data_184:
    DW  $00000000

oldmask: (resetGPU)
data_188:
    DW  $00000000

timeoutAmt
data_18C:
    DW  $00000000
timeoutFlag
data_190:
    DW  $00000000,

    $00000000,$00000000,$00000000



    SECTION .bss

typedef struct GQue
{
    u_long p1;
    u_long p2;
    void (*cb)(u_long p1, u_long p2);
    char   buf[84];
} GQue;

_que:   DSB 6224

GQue _que[64];
