
MEMORY
{
  SYSTEXT (xrw) :  ORIGIN = 0x24060000, LENGTH = 96K
  SYSRAM (xrw) :   ORIGIN = 0x24078000, LENGTH = 32K
}

REGION_ALIAS("TEXT", SYSTEXT);
REGION_ALIAS("RAM", SYSRAM);

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
INCLUDE nvcm_stack_predef.ld

ENTRY(_start)

SECTIONS
{
  .app_header : ALIGN(4)  /* application header (64 byte) for our bootloader */
  {

  	KEEP(*(.application_header))

  } > TEXT
  
  .isr_vector : ALIGN(4)
  {
    FILL(0xFF)
    __vectors_start = ABSOLUTE(.) ;
    KEEP(*(.isr_vector))
    __vectors_end = ABSOLUTE(.) ;

  } > TEXT

  .inits : ALIGN(4)
  {
    /* DATA COPY REGIONS */

    __data_regions_array_start = .;

    LONG(LOADADDR(.data));         LONG(ADDR(.data));         LONG(ADDR(.data)+SIZEOF(.data));
    LONG(LOADADDR(.data_RAM));     LONG(ADDR(.data_RAM));     LONG(ADDR(.data_RAM)+SIZEOF(.data_RAM));

    LONG(LOADADDR(.text_RAM));     LONG(ADDR(.text_RAM));     LONG(ADDR(.text_RAM)+SIZEOF(.text_RAM));

    __data_regions_array_end = .;

    /* DATA CLEAR REGIONS */

    __bss_regions_array_start = .;

    LONG(ADDR(.bss));         LONG(ADDR(.bss)+SIZEOF(.bss));
    LONG(ADDR(.bss_RAM));     LONG(ADDR(.bss_RAM)+SIZEOF(.bss_RAM));

    __bss_regions_array_end = .;

  } > TEXT

  .startup      : ALIGN(4)  {  KEEP(*(.startup))  } > TEXT

  .text_RAM    : ALIGN(4)  { *(.text_RAM) }   > RAM    AT > TEXT
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

  .data_RAM    : ALIGN(4)  { *(.data_RAM) }   > RAM    AT > TEXT
  .data : ALIGN(4)
  {
    *(.data_begin .data_begin.*)
    *(.data .data.*)
    *(.data_end .data_end.*)
    . = ALIGN(4);

  } > RAM AT > TEXT
  
	/* end of the application image: */
	__image_end = LOADADDR(.data) + SIZEOF(.data);

  .bss_RAM   (NOLOAD) : ALIGN(4) { *(.bss_RAM) }   > RAM
  .bss (NOLOAD) : ALIGN(4)
  {
    *(.bss_begin .bss_begin.*)
    *(.bss .bss.*)
    *(COMMON)
    *(.bss_end .bss_end.*)
  } > RAM

  .noinit_RAM   (NOLOAD) : ALIGN(4)  { *(.noinit_RAM) }   > RAM
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
