//
// Kernel Startup and system tables.
// Kernel image is hardcopied at 0xA0000500
//

//
// This stuff is written on asm.
//

.org 0x500

SysInitKMem ()         // bios:BFC10000, kernel:0x500
{
    goto ClearKernelData;
}

SysStubA0:      // 0x510
    la      $t0, TableA0Handler
    jr      $t0
    nop

SysStubB0:      // 0x520
    la      $t0, TableB0Handler
    jr      $t0
    nop

SysStubC0:      // 0x530
    la      $t0, TableC0Handler
    jr      $t0
    nop

PatchA0Table ()     // 0x540
{
    memcpy ( 0x200, Patch_A0_1, sizeof(Patch_A0_1) );       // Patch from A0:00

    memcpy ( 0x2EC, Patch_A0_2, sizeof(Patch_A0_2) );       // Patch from A0:3C
}

ClearKernelData ()         // bios:0xBFC10098, kernel:0x598
{
    //
    // Clear kernel data.
    //

    memset ( 0xA0007460, 0, 0x14C0 );
}

TableA0Handler (t1)       // 0x5C4
{
    dword.0x200[t1] ();
}

TableB0Handler (t1)     // 0x5E0
{
    dword.B0_Table[t1] ();
}

TableC0Handler (t1)     // 0x600
{
    dword.C0_Table[t1] ();
}

ULONG GetCause ()       // 0x620
/* UNUSED */
{
    return COP0.Cause;
}

ULONG GetSr ()      // 0x630
/* UNUSED */
{
    return COP0.SR;
}

void SetSr (ULONG Value)    // 0x640
/* UNUSED */
{
    COP0.SR = Value;
}

void SwitchThread ()        // 0x650
{
    Syscall (0);    // Execute syscall instruction
}

PVOID GetC0Table ()         // 0x65C
{
    return C0_Table;
}

PVOID GetB0Table ()     // 0x668
{
    return B0_Table;
}

//
// 0x674
//

C0_Table:       .word InitRCnt
                .word InitException
                .word SysEnqIntRP
                .word SysDeqIntRP
                .word get_free_EvCB_slot
                .word get_free_TCB_slot
                .word ExceptionHandler
                .word InstallExceptionHandlers
                .word SysInitMemory
                .word SysInitKMem
                .word ChangeClearRCnt
                .word SystemError
                .word InitDefInt
                .word ChangeClearDefInt
                .word dev_stub
                .word dev_stub
                .word dev_stub
                .word dev_stub
                .word InstallDevices
                .word FlushStdInOutPut
                .word dev_stub
                .word _cdevinput
                .word _cdevscan
                .word _circgetc
                .word _circputc
                .word ioabort
                .word sub_3E5C
                .word KernelRedirect
                .word PatchA0Table
                .word sub_3E68
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

//
// 0x874
//

B0_Table:       .word SysMalloc
                .word sub_1408
                .word SetRCnt
                .word GetRCnt
                .word StartRCnt
                .word StopRCnt
                .word ResetRCnt
                .word DeliverEvent
                .word OpenEvent
                .word CloseEvent
                .word WaitEvent
                .word TestEvent
                .word EnableEvent
                .word DisableEvent
                .word OpenTh
                .word CloseTh
                .word ChangeTh
                .word 0
                .word InitPad
                .word StartPad
                .word StopPAD
                .word PAD_init
                .word PAD_dr
                .word ReturnFromException
                .word ResetEntryInt
                .word HookEntryInt
                .word Unk_B0_1A
                .word Unk_B0_1B
                .word Unk_B0_1C
                .word Unk_B0_1D
                .word Unk_B0_1E
                .word Unk_B0_1F
                .word UnDeliverEvent
                .word Unk_B0_21
                .word Unk_B0_22
                .word Unk_B0_23
                .word 0, 0, 0, 0, 0, 0
                .word Unk_B0_2A
                .word Unk_B0_2B
                .word 0, 0, 0, 0, 0, 0

//
// 0x93C
// Patch from A0:00
//

Patch_A0_1:     .word open
                .word lseek
                .word read
                .word write
                .word close
                .word ioctl
                .word exit
                .word Unk_B0_39
                .word getc
                .word putc

//
// 0x964
// Patch from A0:3C
//

Patch_A0_2:     .word getchar
                .word putchar
                .word gets
                .word puts

//
// 0x974 - Never patched A0 callbacks
//

off_974:        .word cd
                .word format
                .word firstfile
                .word nextfile
                .word rename
                .word delete
                .word undelete
                .word AddDevice
                .word RemoveDevice
                .word PrintInstalledDevices
                .word InitCARD
                .word StartCARD
                .word StopCARD
                .word _card_write
                .word _card_read
                .word _new_card
                .word Krom2RawAdd
                .word sub_65E0
                .word Unk_B0_52
                .word sub_6670
                .word get_errno
                .word get_error
                .word GetC0Table
                .word GetB0Table
                .word _card_chan
                .word sub_3678
                .word SysHalt
                .word ChangeClearPad
                .word _card_status
                .word _card_wait
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
