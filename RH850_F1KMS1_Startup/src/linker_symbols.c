/* Define weak symbols referenced by startup code to satisfy linker during build.
 * These are placeholders for sections normally provided by the linker script.
 */

char __sdata_rom = 0;
char __sdata = 0;
char __edata = 0;
char __sbss = 0;
char __ebss = 0;
char __stack = 0;

/* Provide minimal cstart symbol */
void __cstart_pm1(void) { return; }
