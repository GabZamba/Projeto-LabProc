
SECTIONS {                  /* arquivo kernel.ld para placa Evaluator7T */
  . = 0;
  .reset : { *(.reset) }

  . = 0x8000;               /* 0x41000000 no QEMU */
  .text : { *(.text) }
  .data : { *(.data) }
}
