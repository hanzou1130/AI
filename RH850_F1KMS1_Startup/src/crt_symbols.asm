; Provide linker-visible symbols expected by boot.asm (place in BSS)
    .section ".bss", bss
    .align 2
    .public __sdata
__sdata:
    .space 4
    .public __sdata_rom
__sdata_rom:
    .space 4
    .public __edata
__edata:
    .space 4
    .public __sbss
__sbss:
    .space 4
    .public __ebss
__ebss:
    .space 4
    .public __stack
__stack:
    .space 4

    .section ".text", text
    .align 2
    .public __cstart_pm1
__cstart_pm1:
    br __cstart_pm1
