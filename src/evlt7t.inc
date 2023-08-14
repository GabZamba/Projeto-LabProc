
/*
 * GPIO registers addresses.
 */
.set IOPMOD, 0x03ff5000     // GPIO mode (0 input, 0 output)
.set IOPCON, 0x03ff5004     // GPIO configuration
.set IOPDATA, 0x03ff5008    // GPIO values

/*
 * Interruption Controller registers addresses
 */
 .set INTMOD, 0x03ff4000    // 1 IRQ, 0 FIQ (fast)
 .set INTPND, 0x03ff4004    // pending interruptions in 1
 .set INTMSK, 0x03ff4008    // 1 masked, 0 enabled

/*
 * Timer registers addresses
 */
 .set TMOD, 0x03ff6000      // timer configuration
 .set TDATA0, 0x03ff6004    // recharge register of timer 0
 .set TDATA1, 0x03ff6008    // recharge register of timer 1
 .set TCNT0, 0x03ff600c     // count register of timer 0
 .set TCNT1, 0x03ff6010     // count register of timer 1

/*
 * UART 0 registers
 */
.set ULCON0, 0x03ffd000
.set UCON0, 0x03ffd004
.set USTAT0, 0x03ffd008
.set UTXBUF0, 0x03ffd00c
.set URXBUF0, 0x03ffd010
.set UBRDIV0, 0x03ffd014
