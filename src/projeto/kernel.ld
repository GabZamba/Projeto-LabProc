
SECTIONS {                  /* arquivo kernel.ld para QEMU arm virt */
   /*
    * interruption vector
    */
   . = 0;
   .reset : { *(.reset) }

   /*
    * text and data Segmentos
    */
   . = 0x8000;  /* 0x8000 on Evaluator7T, 0x40000000 on QEMU */
  .text : { *(.text) }
  .data : { *(.data) }

  /*
   * bss segment
   */
  bss_start = .;
  .bss : { *(.bss) }
  . = ALIGN(4); 
  bss_end = .;
  heap_start = .;

  /*
   * Reserve space for the system stack
   */
  . = . + 4096;
  . = ALIGN(8);
  stack_svr = .;
}
