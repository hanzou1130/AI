/*******************************************************************************
 * File Name    : linker_script.ld
 * Device       : RH850/F1KM-S1
 * Description  : Linker script for RH850/F1KM-S1
 *                Memory layout and section placement
 ******************************************************************************/

/*------------------------------------------------------------------------------
 * Memory Regions (RH850/F1KM-S1 Specification)
 *----------------------------------------------------------------------------*/
MEMORY
{
    /* Code Flash: 2MB */
    ROM (rx)   : ORIGIN = 0x00000000, LENGTH = 0x00200000

    /* Data Flash: 64KB */
    DFLASH (r) : ORIGIN = 0x01000000, LENGTH = 0x00010000

    /* RAM: 256KB */
    RAM (rwx)  : ORIGIN = 0xFEDE8000, LENGTH = 0x00040000
}

/*------------------------------------------------------------------------------
 * Stack Configuration
 *----------------------------------------------------------------------------*/
__stack_size = 0x2000;      /* 8KB stack */
__heap_size  = 0x4000;      /* 16KB heap */

/*------------------------------------------------------------------------------
 * Entry Point
 *----------------------------------------------------------------------------*/
ENTRY(_RESET)

/*------------------------------------------------------------------------------
 * Section Definitions
 *----------------------------------------------------------------------------*/
SECTIONS
{
    /*--------------------------------------------------------------------------
     * Reset Vector (must be at address 0x00000000)
     *------------------------------------------------------------------------*/
    .reset 0x00000000 :
    {
        KEEP(*(.reset))
    } > ROM

    /*--------------------------------------------------------------------------
     * Exception Vector Table
     *------------------------------------------------------------------------*/
    .except ALIGN(512) :
    {
        __exception_table_start = .;
        KEEP(*(.except))
        __exception_table_end = .;
    } > ROM

    /*--------------------------------------------------------------------------
     * Code Section
     *------------------------------------------------------------------------*/
    .text ALIGN(4) :
    {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        
        /* C++ sections */
        *(.glue_7)
        *(.glue_7t)
        *(.vfp11_veneer)
        *(.v4_bx)
        
        . = ALIGN(4);
        __text_end = .;
    } > ROM

    /*--------------------------------------------------------------------------
     * C++ Constructor/Destructor Tables
     *------------------------------------------------------------------------*/
    .init_array ALIGN(4) :
    {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    } > ROM

    .fini_array ALIGN(4) :
    {
        __fini_array_start = .;
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        __fini_array_end = .;
    } > ROM

    /*--------------------------------------------------------------------------
     * ARM Exception Index (for C++ exception handling)
     *------------------------------------------------------------------------*/
    .ARM.extab ALIGN(4) :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM

    .ARM.exidx ALIGN(4) :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > ROM

    /*--------------------------------------------------------------------------
     * Initialized Data Section (copied from ROM to RAM at startup)
     *------------------------------------------------------------------------*/
    .data ALIGN(4) :
    {
        __sdata = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        __edata = .;
    } > RAM AT > ROM

    /* Store ROM address for data initialization */
    __sdata_rom = LOADADDR(.data);

    /*--------------------------------------------------------------------------
     * BSS Section (zero-initialized data)
     *------------------------------------------------------------------------*/
    .bss ALIGN(4) (NOLOAD) :
    {
        __sbss = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __ebss = .;
    } > RAM

    /*--------------------------------------------------------------------------
     * Heap Section
     *------------------------------------------------------------------------*/
    .heap ALIGN(8) (NOLOAD) :
    {
        __heap_start = .;
        . = . + __heap_size;
        __heap_end = .;
    } > RAM

    /*--------------------------------------------------------------------------
     * Stack Section (grows downward from top of RAM)
     *------------------------------------------------------------------------*/
    .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size (NOLOAD) :
    {
        __stack_start = .;
        . = . + __stack_size;
        __stack_end = .;
    } > RAM

    /*--------------------------------------------------------------------------
     * Debug Sections (not loaded to device)
     *------------------------------------------------------------------------*/
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    
    /* DWARF debug sections */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    .debug_ranges   0 : { *(.debug_ranges) }
}

/*------------------------------------------------------------------------------
 * Symbol Exports for Use in Code
 *----------------------------------------------------------------------------*/
/* Provide symbols used by startup code */
PROVIDE(__sdata = ADDR(.data));
PROVIDE(__edata = ADDR(.data) + SIZEOF(.data));
PROVIDE(__sbss = ADDR(.bss));
PROVIDE(__ebss = ADDR(.bss) + SIZEOF(.bss));

/* End of file */
