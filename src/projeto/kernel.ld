
SECTIONS {                  /* arquivo kernel.ld para QEMU arm virt */
   /*
    * Vetor de reset
    */
   . = 0;
   .reset : { *(.reset) }

   /*
    * Segmentos text e data
    */
   . = 0x40000000;  /* 0x8000 no Evaluator7T, 0x40000000 no QEMU */
  .text : { *(.text) }
  .data : { *(.data) }

  /*
   * Segmento bss
   */
  inicio_bss = .;
  .bss : { *(.bss) }
  . = ALIGN(4); 
  fim_bss = .;
  heap_start = .;

  /*
   * Reserva espaço para as pilhas
   * Sistema, e restante do usuário
   */
  . = . + 4096;
  . = ALIGN(8);
  stack_svr = .;

  . = . + 4096;
  . = ALIGN(8);
  stack_usr = .;
}
