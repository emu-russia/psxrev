//
// Kernel exception handling.
// Written on asm.
//

//
// Interrupt callback queue
// dword_100 (ToT[0]) contain pointer on 4 interrupt callback queues :
// 		Queue 0 : CPU exception handlers (syscalls, overflow etc.)
// 		Queue 1 : Root counter interrupt handlers (4)
//		Queue 2 : Not used by kernel
//		Queue 3 : Hardware interrupt handlers
//

typedef struct _IntRP 
{
	struct _IntRP * next; 		// NULL terminated
	int  	(*func2)(int);
	int 	(*func1)(void);
	long 	flag; 				// Always 0
} IntRP;

typedef struct _ExCB
{
	IntRP 	*head; 		// NULL if queue is empty
	long 	flag;
} ExCB;

//
// Variables
//

PVOID 	KernelSp = 0x85D8; 			// 0x6CF0

jmp_buf * EntryInt; 			// 0x75D0 (somewhere in KernelData)

//
// Default return from exception route
//

jmp_buf DefaultEntryInt = {  		// 0x6CF4
	ReturnFromException,
	0x85D4, 		// sp, KernelData + sizeof(KernelData) + 0x1200
	0, 		// fp
	0, 0, 0, 0, 0, 0, 0, 0,  		// S0 - S7
	0  		// gp
};

VOID ExceptionHandler (VOID) 		// 0xC80
{
	TCHB * TCBH_Entry;
	TCBH * TCB_Entry;
	ULONG Cause, Epc;
	int Opcode;
	ExCB * Head;
	IntRP * Entry;
	int n;
	int Result;

	nop x4; 		// 4 Nops (?)

	TCBH_Entry = dword_108; 		// TCBH
	TCB_Entry = TCBH_Entry->entry; 		// Current TCB (k0)

	//
	// Save context in current TCB
	// k0 is never saved and used as temp kernel pointer.
	//

	TCB_Entry->reg[R_AT] = at;
	TCB_Entry->reg[R_V0] = v0;
	TCB_Entry->reg[R_V1] = v1;
	TCB_Entry->reg[R_RA] = ra;

	GetCauseEpc ( &Cause, &Epc );

	if ( (Cause & 0x3f) == 0 )
	{
		Opcode = (*Epc >> 24) & 0xFE;
		if ( Opcode == 0x4A ) 
			Epc += 4;
	}

	TCB_Entry->reg[R_EPC] = Epc;

	nop x16; 		// 16 Nops (Reset pipeline?)

	TCB_Entry->reg[R_A0] = a0;
	TCB_Entry->reg[R_A1] = a1;
	TCB_Entry->reg[R_A2] = a2;
	TCB_Entry->reg[R_A3] = a3;

	TCB_Entry->reg[R_SR] = COP0.SR;
	TCB_Entry->reg[R_CAUSE] = COP0.Cause;

	TCB_Entry->reg[R_K1] = k1;

	TCB_Entry->reg[R_S0] = s0;
	TCB_Entry->reg[R_S1] = s1;
	TCB_Entry->reg[R_S2] = s2;
	TCB_Entry->reg[R_S3] = s3;
	TCB_Entry->reg[R_S4] = s4;
	TCB_Entry->reg[R_S5] = s5;
	TCB_Entry->reg[R_S6] = s6;
	TCB_Entry->reg[R_S7] = s7;

	TCB_Entry->reg[R_T0] = t0;
	TCB_Entry->reg[R_T1] = t1;
	TCB_Entry->reg[R_T2] = t2;
	TCB_Entry->reg[R_T3] = t3;
	TCB_Entry->reg[R_T4] = t4;
	TCB_Entry->reg[R_T5] = t5;
	TCB_Entry->reg[R_T6] = t6;
	TCB_Entry->reg[R_T7] = t7;
	TCB_Entry->reg[R_T8] = t8;
	TCB_Entry->reg[R_T9] = t9;

	TCB_Entry->reg[R_GP] = gp;
	TCB_Entry->reg[R_SP] = sp;
	TCB_Entry->reg[R_FP] = fp;

	TCB_Entry->reg[R_HI] = HI;
	TCB_Entry->reg[R_LO] = LO;

	//
	// Invoke 4 interrupt callback queues (IntRPs)
	//

	sp = KernelSp;
	Head = dword_100;		// ToT[0]
	gp = 0xF450;
	fp = sp;

	for ( n=0; n<4; n++ )
	{
		Entry = Head[n].head;

		while ( Entry )
		{
			if ( Entry->func1 )
			{
				if ( Result = Entry->func1() && Entry->func2 )
				{
					Entry->func2 ( Result );
				}
			}
			Entry = Entry->next;
		}
	}

	//
	// Restore context from EntryInt
	//

	k0 = &TCBH_Entry->entry.reg;		// Unused

	a0 = dword_75D0;	// jmp_buf * EntryInt
	a1 = 1;

	ra = JmpBuf[JB_PC];
	gp = JmpBuf[JB_GP];
	sp = JmpBuf[JB_SP];
	fp = JmpBuf[JB_FP];
	s0 = JmpBuf[JB_S0];
	s1 = JmpBuf[JB_S1];
	s2 = JmpBuf[JB_S2];
	s3 = JmpBuf[JB_S3];
	s4 = JmpBuf[JB_S4];
	s5 = JmpBuf[JB_S5];
	s6 = JmpBuf[JB_S6];
	s7 = JmpBuf[JB_S7];

	v0 = 1;
	jr ra;
}

GetCauseEpc () 		// 0xEA0
{
	v0 = COP0.Cause;
	v1 = COP0.EPC;
}

InstallExceptionHandlers () 	// 0xEB0
{
	memcpy ( 0x80000080, ExceptionStub, sizeof(ExceptionStub) ); 		// General Exception

	memcpy ( 0x80000000, ExceptionStub, sizeof(ExceptionStub) ); 		// TLB Miss

	FlushCache ();
}

ExceptionStub () 		// 0xF0C
{
	goto ExceptionHandler; 		// k0
}

HookEntryInt (jmp_buf * Jmpbuf) 		// 0xF20
{
	EntryInt = Jmpbuf;
}

ResetEntryInt () 		// 0xF2C
{
	EntryInt = &DefaultEntryInt;
}

ReturnFromException () 		// 0xF40
{
	TCHB * TCBH_Entry;
	TCBH * TCB_Entry;

	TCBH_Entry = dword_108; 		// TCBH
	TCB_Entry = TCBH_Entry->entry; 		// Current TCB (k0)

	//
	// Restore context
	//

	LO = TCB_Entry->reg[R_LO];
	HI = TCB_Entry->reg[R_HI];

	COP0.SR = TCB_Entry->reg[R_SR];

	v0 = TCB_Entry->reg[R_V0];
	v1 = TCB_Entry->reg[R_V1];

	a1 = TCB_Entry->reg[R_A1];
	a2 = TCB_Entry->reg[R_A2];
	a3 = TCB_Entry->reg[R_A3];

	t0 = TCB_Entry->reg[R_T0];
	t1 = TCB_Entry->reg[R_T1];
	t2 = TCB_Entry->reg[R_T2];
	t3 = TCB_Entry->reg[R_T3];
	t4 = TCB_Entry->reg[R_T4];
	t5 = TCB_Entry->reg[R_T5];
	t6 = TCB_Entry->reg[R_T6];
	t7 = TCB_Entry->reg[R_T7];

	s0 = TCB_Entry->reg[R_S0];
	s1 = TCB_Entry->reg[R_S1];
	s2 = TCB_Entry->reg[R_S2];
	s3 = TCB_Entry->reg[R_S3];
	s4 = TCB_Entry->reg[R_S4];
	s5 = TCB_Entry->reg[R_S5];
	s6 = TCB_Entry->reg[R_S6];
	s7 = TCB_Entry->reg[R_S7];

	t8 = TCB_Entry->reg[R_T8];
	t9 = TCB_Entry->reg[R_T9];

	k1 = TCB_Entry->reg[R_K1];
	gp = TCB_Entry->reg[R_GP];
	sp = TCB_Entry->reg[R_SP];
	fp = TCB_Entry->reg[R_FP];
	ra = TCB_Entry->reg[R_RA];
	at = TCB_Entry->reg[R_AT];

	k0 = TCB_Entry->reg[R_EPC];
	a0 = TCB_Entry->reg[R_A0];

	RFE;

	goto k0;
}

PVOID GetKernelSp () 	// 0x1018
/* UNUSED */
{
	return KernelSp;
}