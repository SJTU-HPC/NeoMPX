/*
 * Copyright (c) 2019 by SJTU HPC
 * Contributed by Jinkun Chen <chenjinkun_429@sjtu.edu.cn>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Huawei Kunpeng920
 *
 * ARM Architecture Reference Manual, ARMv8, for ARMv8-A architecture profile,
 * ARM DDI 0487B.a (ID033117)
 *
 */

static const arm_entry_t arm_kunpeng920_pe[]={
	{.name = "L1I_CACHE_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x01,
     .desc = "Level 1 instruction cache refill"
    },	
	{.name = "L1I_TLB_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x02,
     .desc = "L1 instruction TLB refill"
    },
	{.name = "L1D_TLB_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x05,
     .desc = "L1 data TLB refill"
    },
	{.name = "INST_RETIRED",
     .modmsk = ARMV8_ATTRS,
     .code = 0x08,
     .desc = "Instruction architecturally executed"
    },
	{.name = "BR_MIS_PRED",
     .modmsk = ARMV8_ATTRS,
     .code = 0x10,
     .desc = "Mispredicted or not predicted branch speculatively executed"
    },
    {.name = "CPU_CYCLES",
     .modmsk = ARMV8_ATTRS,
     .code = 0x11,
     .desc = "Cycle"
    },
	{.name = "BR_PRED",
     .modmsk = ARMV8_ATTRS,
     .code = 0x12,
     .desc = "Predicted branch speculatively executed"
    },
	{.name = "MEM_ACCESS",
     .modmsk = ARMV8_ATTRS,
     .code = 0x13,
     .desc = "Data memory access"
    },
	{.name = "L1I_CACHE",
     .modmsk = ARMV8_ATTRS,
     .code = 0x14,
     .desc = "Level 1 instruction cache access"
    },
	{.name = "L1D_CACHE_WB",
     .modmsk = ARMV8_ATTRS,
     .code = 0x15,
     .desc = "Level 1 data cache access write back"
    },
	{.name = "L2D_CACHE",
     .modmsk = ARMV8_ATTRS,
     .code = 0x16,
     .desc = "Level 2 data cache access"
    },
	{.name = "L2D_CACHE_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x17,
     .desc = "Level 2 data cache refill"
    },
	{.name = "L2D_CACHE_WB",
     .modmsk = ARMV8_ATTRS,
     .code = 0x18,
     .desc = "Level 2 data cache write back"
    },
	{.name = "BUS_ACCESS",
     .modmsk = ARMV8_ATTRS,
     .code = 0x19,
     .desc = "Bus access"
    },
	{.name = "MEMORY_ERROR",
     .modmsk = ARMV8_ATTRS,
     .code = 0x1A,
     .desc = "Local memory error"
    },
	{.name = "BUS_CYCLES",
     .modmsk = ARMV8_ATTRS,
     .code = 0x1D,
     .desc = "Bus cycles"
    },
	{.name = "BR_RETIRED",
     .modmsk = ARMV8_ATTRS,
     .code = 0x21,
     .desc = "Instruction architecturally executed, branch."
    },
	{.name = "BR_MIS_PRED_RETIRED",
     .modmsk = ARMV8_ATTRS,
     .code = 0x22,
     .desc = "Instruction architecturally executed, mis-redicted branch."
    },
	{.name = "STALL_FRONTEND",
     .modmsk = ARMV8_ATTRS,
     .code = 0x23,
     .desc = "No operation issued due to the frontend"
    },
	{.name = "STALL_BACKEND",
     .modmsk = ARMV8_ATTRS,
     .code = 0x24,
     .desc = "No operation issued due to the backend"
    },
	{.name = "L1D_TLB",
     .modmsk = ARMV8_ATTRS,
     .code = 0x25,
     .desc = "Level 1 data TLB access"
    },
	{.name = "L1I_TLB",
     .modmsk = ARMV8_ATTRS,
     .code = 0x26,
     .desc = "Level 2 instruction TLB access"
    },
	{.name = "L2I_CACHE",
     .modmsk = ARMV8_ATTRS,
     .code = 0x27,
     .desc = "Level 2 instruction cache access"
    },
	{.name = "L2I_CACHE_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x28,
     .desc = "Level 2 instruction cache refill"
    },
	{.name = "L2D_TLB_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x2D,
     .desc = "Attributable L2 data/unified / L2 instruction TLB refill"
    },
	{.name = "L2I_TLB_REFILL",
     .modmsk = ARMV8_ATTRS,
     .code = 0x2E,
     .desc = "Attributable L2 data/unified / L2 instruction TLB refill"
    },
	{.name = "L2D_TLB",
     .modmsk = ARMV8_ATTRS,
     .code = 0x2F,
     .desc = "Level 2 data TLB access"
    },
	{.name = "L2I_TLB",
     .modmsk = ARMV8_ATTRS,
     .code = 0x30,
     .desc = "Level 2 instruction TLB access"
    },
	{.name = "REMOTE_ACCESS",
     .modmsk = ARMV8_ATTRS,
     .code = 0x31,
     .desc = "Access to another socket in a multi-socket system"
    },
	{.name = "LL_CACHE",
     .modmsk = ARMV8_ATTRS,
     .code = 0x32,
     .desc = "Last level cache access"
    },
	{.name = "LL_CACHE_MISS",
     .modmsk = ARMV8_ATTRS,
     .code = 0x33,
     .desc = "Last level cache miss"
    },
	{.name = "DTLB_WALK",
     .modmsk = ARMV8_ATTRS,
     .code = 0x34,
     .desc = "Access to data TLB that caused a page table walk"
    },
	{.name = "ITLB_WALK",
     .modmsk = ARMV8_ATTRS,
     .code = 0x35,
     .desc = "Access to instruction TLB that caused a page table walk"
    },
	{.name = "LL_CACHE_RD",
     .modmsk = ARMV8_ATTRS,
     .code = 0x36,
     .desc = "Last level cache access read"
    },
	{.name = "LL_CACHE_MISS_RD",
     .modmsk = ARMV8_ATTRS,
     .code = 0x37,
     .desc = "Last level cache miss read"
    },
	{.name = "REMOTE_ACCESS_RD",
     .modmsk = ARMV8_ATTRS,
     .code = 0x38,
     .desc = "Access to another socket in a multi-socket system, read"
    },
	{.name = "L1D_CACHE_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x40,
	 .desc = "Level 1 data cache access, read"
	},
	{.name = "L1D_CACHE_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x41,
	 .desc = "Level 1 data cache access, write"
	},
	{.name = "L1D_CACHE_REFILL_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x42,
	 .desc = "Level 1 data cache refill, read"
	},
	{.name = "L1D_CACHE_REFILL_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x43,
	 .desc = "Level 1 data cache refill, write"
	},
	{.name = "L1D_CACHE_REFILL_INNER",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x44,
	 .desc = "Level 1 data cache refill, inner"
	},
	{.name = "L1D_CACHE_REFILL_OUTER",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x45,
	 .desc = "Level 1 data cache refill, outer"
	},
	{.name = "L1D_CACHE_WB_VICTIM",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x46,
	 .desc = "Level 1 data cache write-back, victim"
	},
	{.name = "L1D_CACHE_WB_CLEAN",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x47,
	 .desc = "Level 1 data cache write-back, cleaning and coherency"
	},
	{.name = "L1D_CACHE_INVAL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x48,
	 .desc = "Level 1 data cache invalidate"
	},
	{.name = "L1D_TLB_REFILL_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x4C,
	 .desc = "Level 1 data TLB read refill"
	},
	{.name = "L1D_TLB_REFILL_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x4D,
	 .desc = "Level 1 data TLB write refill"
	},
	{.name = "L1D_TLB_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x4E,
	 .desc = "Level 1 data TLB access, read"
	},
	{.name = "L1D_TLB_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x4F,
	 .desc = "Level 1 data TLB access, write"
	},
	{.name = "L2D_CACHE_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x50,
	 .desc = "Level 2 data cache access, read"
	},
	{.name = "L2D_CACHE_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x51,
	 .desc = "Level 2 data cache access, write"
	},
	{.name = "L2D_CACHE_REFILL_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x52,
	 .desc = "Level 2 data cache refill, read"
	},
	{.name = "L2D_CACHE_REFILL_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x53,
	 .desc = "Level 2 data cache refill, write"
	},
	{.name = "L2D_CACHE_WB_VICTIM",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x56,
	 .desc = "Level 2 data cache write-back, victim"
	},
	{.name = "L2D_CACHE_WB_CLEAN",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x57,
	 .desc = "Level 2 data cache write-back, cleaning and coherency"
	},
	{.name = "L2D_CACHE_INVAL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x58,
	 .desc = "Level 2 data cache invalidate"
	},
	{.name = "L2D_TLB_REFILL_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5C,
	 .desc = "Level 2 data/unified TLB refill, read"
	},
	{.name = "L2D_TLB_REFILL_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5D,
	 .desc = "Level 2 data/unified TLB refill, write"
	},
	{.name = "L2D_TLB_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5E,
	 .desc = "Level 2 data/unified TLB access, read"
	},
	{.name = "L2D_TLB_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5F,
	 .desc = "Level 2 data/unified TLB access, write"
	},
	{.name = "BUS_ACCESS_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x60,
	 .desc = "Bus access, read"
	},
	{.name = "BUS_ACCESS_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x61,
	 .desc = "Bus access, write"
	},
	{.name = "BUS_ACCESS_SHARED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x62,
	 .desc = "Bus access, normal, cacheable, shareable"
	},
	{.name = "BUS_ACCESS_NOT_SHARED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x63,
	 .desc = "Bus not normal access"
	},
	{.name = "BUS_ACCESS_NORMAL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x64,
	 .desc = "Bus access, normal"
	},
	{.name = "BUS_ACCESS_PERIPH",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x65,
	 .desc = "Bus access, peripheral"
	},
	{.name = "MEM_ACCESS_RD",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x66,
	 .desc = "Data memory access, read"
	},
	{.name = "MEM_ACCESS_WR",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x67,
	 .desc = "Data memory access, write"
	},
	{.name = "UNALIGNED_LD_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x68,
	 .desc = "Unaligned access, read"
	},
	{.name = "UNALIGNED_ST_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x69,
	 .desc = "Unaligned access, write"
	},
	{.name = "UNALIGNED_LDST_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6A,
	 .desc = "Unaligned access"
	},
	{.name = "LDREX_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6C,
	 .desc = "Exclusive operation speculatively executed - LDREX or LDX"
	},
	{.name = "STREX_PASS_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6D,
	 .desc = "Exclusive operation speculative executed - STREX or STX pass"
	},
	{.name = "STREX_FAIL_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6E,
	 .desc = "Exclusive operation speculative executed - STREX or STX fail"
	},
	{.name = "STREX_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6F,
	 .desc = "Exclusive operation speculatively executed - STREX or STX"
	},
	{.name = "LD_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x70,
	 .desc = "Operation speculatively executed, load"
	},
	{.name = "ST_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x71,
	 .desc = "Operation speculatively executed, store"
	},
	{.name = "LDST_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x72,
	 .desc = "Operation speculatively executed, load or store"
	},
	{.name = "DP_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x73,
	 .desc = "Operation speculatively executed, data-processing"
	},
	{.name = "ASE_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x74,
	 .desc = "Operation speculatively executed, Advanced SIMD instruction"
	},
	{.name = "VFP_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x75,
	 .desc = "Operation speculatively executed, floating point instruction"
	},
	{.name = "PC_WRITE_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x76,
	 .desc = "Operation speculatively executed, software change of the PC"
	},
	{.name = "CRYPTO_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x77,
	 .desc = "Operation speculatively executed, Cryptographic instruction"
	},
	{.name = "BR_IMMED_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x78,
	 .desc = "Branch speculatively executed, immediate branch"
	},
	{.name = "BR_RETURN_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x79,
	 .desc = "Branch speculatively executed, return"
	},
	{.name = "BR_INDIRECT_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7A,
	 .desc = "Branch speculatively executed, indirect branch"
	},
	{.name = "ISB_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7C,
	 .desc = "Barrier speculatively executed, ISB"
	},
	{.name = "DSB_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7D,
	 .desc = "barrier speculatively executed, DSB"
	},
	{.name = "DMB_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x7E,
	 .desc = "Barrier speculatively executed, DMB"
	},
	{.name = "EXC_UNDEF",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x81,
	 .desc = "Exception taken, other synchronous"
	},
	{.name = "EXC_SVC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x82,
	 .desc = "Exception taken, supervisor call"
	},
	{.name = "EXC_PABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x83,
	 .desc = "Exception taken, instruction abort"
	},
	{.name = "EXC_DABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x84,
	 .desc = "Exception taken, data abort or SError"
	},
	{.name = "EXC_IRQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x86,
	 .desc = "Exception taken, irq"
	},
	{.name = "EXC_FIQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x87,
	 .desc = "Exception taken, fiq"
	},
	{.name = "EXC_SMC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x88,
	 .desc = "Exception taken, smc"
	},
	{.name = "EXC_HVC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8A,
	 .desc = "Exception taken, hypervisor call"
	},
	{.name = "EXC_TRAP_PABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8B,
	 .desc = "Exception taken, instruction abort not taken locally"
	},
	{.name = "EXC_TRAP_DABORT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8C,
	 .desc = "Exception taken, data abort or SError not taken locally"
	},
	{.name = "EXC_TRAP_OTHER",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8D,
	 .desc = "Exception taken, other traps not taken locally"
	},
	{.name = "EXC_TRAP_IRQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8E,
	 .desc = "Exception taken, irq not taken locally"
	},
	{.name = "EXC_TRAP_FIQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x8F,
	 .desc = "Exception taken, fiq not taken locally"
	},
	{.name = "RC_LD_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x90,
	 .desc = "Release consistency instruction speculatively executed (load-acquire)"
	},
	{.name = "RC_ST_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x91,
	 .desc = "Release consistency instruction speculatively executed (store-release)"
	},
	{.name = "UOP_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2000,
	 .desc = "micro-op speculatively executed"
	},	
	{.name = "ROB_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2004,
	 .desc = "rob full"
	},
	{.name = "ALU_ISQ_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x200b,
	 .desc = "All ALU issue queues are full"
	},
	{.name = "LSU_ISQ_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x200c,
	 .desc = "All LSU issue queues are full"
	},
	{.name = "FSU_ISQ_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x200d,
	 .desc = "All FSU issue queues are full"
	},
	{.name = "ALU_PORT_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2011,
	 .desc = "Stall because of limited ALU issue queue write ports for certain kind of uops"
	},
	{.name = "FETCH_BUBBLE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2014,
	 .desc = "after ifu initial done, number of instructions that decode can receive but ifu not send, from 0 to 4"
	},
	{.name = "DEC_BUBBLE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2016,
	 .desc = "after ifu initial done, number of instructions that fetch can supply but decode not receive, from 0 to 4"
	},
	{.name = "VFP_SYNC_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x201f,
	 .desc = "sync buffer full at vfp side"
	},
	{.name = "INT_SYNC_STALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2020,
	 .desc = "sync buffer full at int side"
	},
	{.name = "FSTD_ISQ_FULL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2021,
	 .desc = "fstd issue queue is full"
	},
	{.name = "ALU_ISQ_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2022,
	 .desc = "All ALU issue queues full is resource bound for stall"
	},
	{.name = "LSU_ISQ_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2023,
	 .desc = "All LSU issue queues full is resource bound for stall"
	},
	{.name = "FSU_ISQ_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2024,
	 .desc = "All FSU issue queues full is resource bound for stall"
	},
	{.name = "FSTD_ISQ_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2025,
	 .desc = "All FSTD issue queues full is resource bound for stall"
	},
	{.name = "INT_PTAG_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2026,
	 .desc = "INT ptag full is resource bound for stall"
	},
	{.name = "VFP_PTAG_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2027,
	 .desc = "VFP ptag full is resource bound for stall"
	},
	{.name = "CC_PTAG_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2028,
	 .desc = "CC ptag full is resource bound for stall"
	},
	{.name = "INT_SYNC_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2029,
	 .desc = "INT sync full is resource bound for stall"
	},
	{.name = "VFP_SYNC_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x202a,
	 .desc = "VFP sync full is resource bound for stall"
	},
	{.name = "ALU_PORT_BOUND",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x202b,
	 .desc = "alu port full is resource bound for stall"
	},
	{.name = "I2F_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2037,
	 .desc = "int to fp speculative decoded"
	},
	{.name = "F2I_SPEC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2038,
	 .desc = "fp to int speculative decoded"
	},
	{.name = "ROB_EMPTY",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203a,
	 .desc = "rob empty cycles"
	},
	{.name = "BR_COND_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203b,
	 .desc = "conditional branch retired"
	},
	{.name = "BR_PUSH_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203c,
	 .desc = "bl/blr retired"
	},
	{.name = "BR_IND_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203d,
	 .desc = "br/blr/ret retired"
	},
	{.name = "BR_NOT_TKN_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203e,
	 .desc = "not taken branch retired"
	},
	{.name = "BR_MISP_COND_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203f,
	 .desc = "mispredict conditional branch retired"
	},
	{.name = "BR_MISP_PUSH_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2040,
	 .desc = "mispredict bl/blr retired"
	},
	{.name = "BR_MISP_RET_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2041,
	 .desc = "mispredict ret retired"
	},
	{.name = "BR_MISP_IND_RETIRED",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2042,
	 .desc = "mispredict br/blr/ret retired"
	},
	{.name = "LD_RETIRED_OOO",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2043,
	 .desc = "Load Instruction architecturally executed"
	},
	{.name = "ST_RETIRED_OOO",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x2044,
	 .desc = "Store Instruction architecturally executed"
	},
	{.name = "ROB_EMPTY",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x203a,
	 .desc = "rob empty cycles"
	},
	{.name = "IEX_LD_REJECT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x501b,
	 .desc = "LSU reject load to IEX"
	},
	{.name = "IEX_ST_REJECT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x501c,
	 .desc = "LSU reject store to IEX"
	},
	{.name = "IEX_STD_REJECT",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x501d,
	 .desc = "LSU reject STD to IEX"
	},
	{.name = "STREAM",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5023,
	 .desc = "store with streaming"
	},
	{.name = "STA_RETIRE_MERGE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5024,
	 .desc = "store sta operation with merge"
	},
	{.name = "STA_RETIRE_ALLOC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5025,
	 .desc = "store sta operation with allocate"
	},
	{.name = "STD_RETIRE_MERGE",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5026,
	 .desc = "store std operation with merge"
	},
	{.name = "STD_RETIRE_ALLOC",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5027,
	 .desc = "store std operation with allocate"
	},
	{.name = "L1D_CACHE_REFILL_PF",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x5033,
	 .desc = "Attributable Level 1 data cache refill with leading prefetch"
	},
	{.name = "LSU_RD_ALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6008,
	 .desc = "L2 cache LSU ACCESS",
	},
	{.name = "IFU_RD_ALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6009,
	 .desc = "L2 cache IFU ACCESS"
	},
	{.name = "MMU_RD_ALL",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x600a,
	 .desc = "L2 cache MMU ACCESS"
	},
	{.name = "LSU_HIT_L2",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x600b,
	 .desc = "LSU request hit L2 Cache"
	},
	{.name = "IFU_HIT_L2",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x600c,
	 .desc = "IFU request hit L2 Cache"
	},
	{.name = "MMU_HIT_L2",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x600d,
	 .desc = "MMU request hit L2 Cache"
	},
	{.name = "PRF_REQ",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6013,
	 .desc = "prefetchh request from lsu"
	},
	{.name = "HIT_ON_PRF",
	 .modmsk = ARMV8_ATTRS,
	 .code = 0x6014,
	 .desc = "hit on prefetched data"
	},
};
