/******************************************************************************/
/* LNKX.CMD - COMMAND FILE FOR LINKING C PROGRAMS IN LARGE/HUGE MEMORY MODEL  */
/*                                                                            */
/* Usage:                                                                     */
/*  cl55 <src files> -z -o<out file> -m<map file> lnkx.cmd -l<RTS library>   */
/*                                                                            */
/* Description: This file is a sample command file that can be used for       */
/*              linking programs built with the C Compiler.  Use it as a      */
/*              guideline; you  may want to change the allocation scheme      */
/*              according to the size of your program and the memory layout   */
/*              of your target system.                                        */
/*                                                                            */
/*   Notes: (1) You must specify the directory in which <RTS library> is      */
/*              located.  Either add a "-i<directory>" line to this file      */
/*              file, or use the system environment variable C55X_C_DIR to    */
/*              specify a search path for the libraries.                      */
/*                                                                            */
/******************************************************************************/

-stack    0x2000      /* Primary stack size   */
-sysstack 0x1000      /* Secondary stack size */
-heap     0x2000      /* Heap area size       */

-c                    /* Use C linking conventions: auto-init vars at runtime */
-u _Reset             /* Force load of reset interrupt handler                */

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
 PAGE 0:  /* ---- Unified Program/Data Address Space ---- */

  MMR    (RWIX): origin = 0x000000, length = 0x0000C0  /* MMRs */
  BTRSVD (RWIX): origin = 0x0000C0, length = 0x000240  /* Reserved for Boot Loader */
  DARAM0 (RWIX): origin = 0x000300, length = 0x00FB00  /* 64KB - MMRs - VECS*/
  VECS   (RWIX): origin = 0x00FE00, length = 0x000200  /* 256 bytes Vector Table */
  CE0          : origin = 0x010000, length = 0x3f0000  /* 4M minus 64K Bytes */
  CE1          : origin = 0x400000, length = 0x400000
  CE2          : origin = 0x800000, length = 0x400000
  CE3          : origin = 0xC00000, length = 0x3F8000
  PDROM   (RIX): origin = 0xFF8000, length = 0x008000  /*  32KB */
  
 PAGE 2:  /* -------- 64K-word I/O Address Space -------- */

  IOPORT (RWI) : origin = 0x000000, length = 0x020000
}
 
/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
   .text     >> DARAM0                /* Code                        */
   .stack    >  DARAM0                /* Primary system stack        */
   .sysstack >  DARAM0                /* Secondary system stack      */
   .csldata  >  DARAM0                /* CSL data                    */
   .data     >> DARAM0                /* Initialized vars            */
   .bss      >> DARAM0                /* Global & static vars        */
   .const    >> DARAM0                /* Constant data               */
   .sysmem   >  DARAM0                /* Dynamic memory (malloc)     */
   .switch   >  DARAM0                /* Switch statement tables     */
   .cinit    >  DARAM0                /* Auto-initialization tables  */
   .pinit    >  DARAM0                /* Initialization fn tables    */
   .cio      >  DARAM0                /* C I/O buffers               */
   .args     >  DARAM0                /* Arguments to main()         */
       
	/* interrupt vector table must be on 256 "page" boundry*/
	vectors:  >  VECS

   .ioport   >  IOPORT PAGE 2         /* Global & static ioport vars */
   .fftcode  > DARAM0 PAGE 0
}
