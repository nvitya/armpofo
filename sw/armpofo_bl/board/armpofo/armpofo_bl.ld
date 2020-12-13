
MEMORY
{
  FLASH (rx) :    ORIGIN = 0x08000000, LENGTH = 128K
  RAM2 (xrw) :    ORIGIN = 0x24000000, LENGTH = 32K
}

REGION_ALIAS("TEXT", FLASH);
REGION_ALIAS("RAM", RAM2);

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
INCLUDE nvcm_stack_predef.ld


ENTRY(_start)

SECTIONS
{
  .isr_vector : ALIGN(4)
  {
    FILL(0xFF)
    __vectors_start = ABSOLUTE(.) ;
    KEEP(*(.isr_vector))
    __vectors_end = ABSOLUTE(.) ;

  } > FLASH

  .inits : ALIGN(4)
  {
    /* DATA COPY REGIONS */

    __data_regions_array_start = .;

    LONG(LOADADDR(.data));         LONG(ADDR(.data));         LONG(ADDR(.data)+SIZEOF(.data));
    LONG(LOADADDR(.data_RAM2));    LONG(ADDR(.data_RAM2));    LONG(ADDR(.data_RAM2)+SIZEOF(.data_RAM2));

    LONG(LOADADDR(.text_RAM2));    LONG(ADDR(.text_RAM2));    LONG(ADDR(.text_RAM2)+SIZEOF(.text_RAM2));

    __data_regions_array_end = .;

    /* DATA CLEAR REGIONS */

    __bss_regions_array_start = .;

    LONG(ADDR(.bss));         LONG(ADDR(.bss)+SIZEOF(.bss));
    LONG(ADDR(.bss_RAM2));    LONG(ADDR(.bss_RAM2)+SIZEOF(.bss_RAM2));

    __bss_regions_array_end = .;

  } > FLASH

  .startup      : ALIGN(4)  {  KEEP(*(.startup))  } > FLASH

  .text_RAM2    : ALIGN(4)  { *(.text_RAM2) }   > RAM2    AT > FLASH
  .text : ALIGN(4)
  {
    /* the CPP init must be included into .text section for Atmel Studio compatibility */
    INCLUDE nvcm_cppinit.ld

    *(.text .text.* .gnu.linkonce.t.*)
    *(.rodata .rodata.* .constdata .constdata.* .gnu.linkonce.r.*)
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(vtable)

    KEEP(*(.eh_frame*))

    *(.glue_7) *(.glue_7t)

    . = ALIGN(4);
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend.o(.ctors))

  } > TEXT

  /* .ARM.exidx is sorted, so has to go in its own output section.  */
  PROVIDE_HIDDEN (__exidx_start = .);
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > TEXT
  PROVIDE_HIDDEN (__exidx_end = .);
  . = ALIGN(4);
  _etext = .;
  __etext = .;

  .data_RAM2    : ALIGN(4)  { *(.data_RAM2) }   > RAM2    AT > FLASH
  .data : ALIGN(4)
  {
    *(.data_begin .data_begin.*)
    *(.data .data.*)
    *(.data_end .data_end.*)
    . = ALIGN(4);

  } > RAM AT > FLASH

  .bss_RAM2   (NOLOAD) : ALIGN(4) { *(.bss_RAM2) }   > RAM2
  .bss (NOLOAD) : ALIGN(4)
  {
    *(.bss_begin .bss_begin.*)
    *(.bss .bss.*)
    *(COMMON)
    *(.bss_end .bss_end.*)
  } > RAM

  .noinit_RAM2   (NOLOAD) : ALIGN(4)  { *(.noinit_RAM2) }   > RAM2
  .noinit (NOLOAD) : ALIGN(4)
  {
    _noinit = .;
    *(.noinit .noinit.*)
    . = ALIGN(4) ;
    _end_noinit = .;
  } > RAM

  INCLUDE nvcm_stack_heap.ld
  INCLUDE nvcm_debug_sections.ld
}
