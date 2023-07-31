#ifndef STRUCTS_H
#define STRUCTS_H

/* Configuração dos GPIOs */
struct __attribute__((packed)) IOPCON_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int EIRQ0_interruptEvent : 2;
            unsigned int EIRQ0_debounce : 1;
            unsigned int EIRQ0_verifyLevel : 1;
            unsigned int EIRQ0_enable : 1;

            unsigned int EIRQ1_interruptEvent : 2;
            unsigned int EIRQ1_debounce : 1;
            unsigned int EIRQ1_verifyLevel : 1;
            unsigned int EIRQ1_enable : 1;

            unsigned int EIRQ2_interruptEvent : 2;
            unsigned int EIRQ2_debounce : 1;
            unsigned int EIRQ2_verifyLevel : 1;
            unsigned int EIRQ2_enable : 1;

            unsigned int EIRQ3_interruptEvent : 2;
            unsigned int EIRQ3_debounce : 1;
            unsigned int EIRQ3_verifyLevel : 1;
            unsigned int EIRQ3_enable : 1;

            unsigned int DMA_REQ0_level : 1;
            unsigned int DMA_REQ0_debounce : 1;
            unsigned int DMA_REQ0_enable : 1;

            unsigned int DMA_REQ1_level : 1;
            unsigned int DMA_REQ1_debounce : 1;
            unsigned int DMA_REQ1_enable : 1;

            unsigned int DMA_ACK0_level : 1;
            unsigned int DMA_ACK0_enable : 1;

            unsigned int DMA_ACK1_level : 1;
            unsigned int DMA_ACK1_enable : 1;

            unsigned int TOUT_EN_timer0 : 1;
            unsigned int TOUT_EN_timer1 : 1;
        };
    };
};

/* Entrada: 0, Saída: 1 */
struct __attribute__((packed)) IOPMOD_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int GPIO0 : 1;
            unsigned int GPIO1 : 1;
            unsigned int GPIO2 : 1;
            unsigned int GPIO3 : 1;
            unsigned int GPIO4 : 1;
            unsigned int GPIO5 : 1;
            unsigned int GPIO6 : 1;
            unsigned int GPIO7 : 1;
            unsigned int GPIO8 : 1;
            unsigned int GPIO9 : 1;
            unsigned int GPIO10 : 1;
            unsigned int GPIO11 : 1;
            unsigned int GPIO12 : 1;
            unsigned int GPIO13 : 1;
            unsigned int GPIO14 : 1;
            unsigned int GPIO15 : 1;
            unsigned int GPIO16 : 1;
            unsigned int GPIO17 : 1;
            unsigned int : 14;
        };
        struct
        {
            unsigned int switch4 : 1;
            unsigned int switch3 : 1;
            unsigned int switch2 : 1;
            unsigned int switch1 : 1;
            unsigned int ledGreenR : 1;
            unsigned int ledYellow : 1;
            unsigned int ledOrange : 1;
            unsigned int ledGreenL : 1;
            unsigned int button : 1;
            unsigned int : 1;
            unsigned int displayA : 1;
            unsigned int displayB : 1;
            unsigned int displayC : 1;
            unsigned int displayD : 1;
            unsigned int displayE : 1;
            unsigned int displayG : 1;
            unsigned int displayF : 1;
            unsigned int : 15;
        };
        struct
        {
            unsigned int switches : 4;
            unsigned int leds : 4;
            unsigned int : 2;
            unsigned int displays : 7;
            unsigned int : 15;
        };
    };
};

/* Valor presente no GPIO */
struct __attribute__((packed)) IOPDATA_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int GPIO0 : 1;
            unsigned int GPIO1 : 1;
            unsigned int GPIO2 : 1;
            unsigned int GPIO3 : 1;
            unsigned int GPIO4 : 1;
            unsigned int GPIO5 : 1;
            unsigned int GPIO6 : 1;
            unsigned int GPIO7 : 1;
            unsigned int GPIO8 : 1;
            unsigned int GPIO9 : 1;
            unsigned int GPIO10 : 1;
            unsigned int GPIO11 : 1;
            unsigned int GPIO12 : 1;
            unsigned int GPIO13 : 1;
            unsigned int GPIO14 : 1;
            unsigned int GPIO15 : 1;
            unsigned int GPIO16 : 1;
            unsigned int GPIO17 : 1;
            unsigned int : 14;
        };
        struct
        {
            unsigned int switch4 : 1;
            unsigned int switch3 : 1;
            unsigned int switch2 : 1;
            unsigned int switch1 : 1;
            unsigned int ledGreenR : 1;
            unsigned int ledYellow : 1;
            unsigned int ledOrange : 1;
            unsigned int ledGreenL : 1;
            unsigned int button : 1;
            unsigned int : 1;
            unsigned int displayA : 1;
            unsigned int displayB : 1;
            unsigned int displayC : 1;
            unsigned int displayD : 1;
            unsigned int displayE : 1;
            unsigned int displayG : 1;
            unsigned int displayF : 1;
            unsigned int : 15;
        };
        struct
        {
            unsigned int switches : 4;
            unsigned int leds : 4;
            unsigned int : 2;
            unsigned int displays : 7;
            unsigned int : 15;
        };
    };
};

/* IRQ: 1, FIQ: 0 */
struct __attribute__((packed)) INTMOD_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int ExtSignal_GPIO08 : 1;
            unsigned int ExtSignal_GPIO09 : 1;
            unsigned int ExtSignal_GPIO10 : 1;
            unsigned int ExtSignal_GPIO11 : 1;
            unsigned int UART0_Tx : 1;
            unsigned int UART0_Rx : 1;
            unsigned int UART1_Tx : 1;
            unsigned int UART1_Rx : 1;
            unsigned int DMA0_end : 1;
            unsigned int DMA1_end : 1;
            unsigned int Timer0 : 1;
            unsigned int Timer1 : 1;
            unsigned int HDLCA_Tx : 1;
            unsigned int HDLCA_Rx : 1;
            unsigned int HDLCB_Tx : 1;
            unsigned int HDLCB_Rx : 1;
            unsigned int EthernetDMA_Tx : 1;
            unsigned int EthernetDMA_Rx : 1;
            unsigned int EthernetMAC_Tx : 1;
            unsigned int EthernetMAC_Rx : 1;
            unsigned int I2CEvent : 1;
            unsigned int : 11;
        };
    };
};

/* Interrupções pendentes em 1 */
struct __attribute__((packed)) INTPEND_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int ExtSignal_GPIO08 : 1;
            unsigned int ExtSignal_GPIO09 : 1;
            unsigned int ExtSignal_GPIO10 : 1;
            unsigned int ExtSignal_GPIO11 : 1;
            unsigned int UART0_Tx : 1;
            unsigned int UART0_Rx : 1;
            unsigned int UART1_Tx : 1;
            unsigned int UART1_Rx : 1;
            unsigned int DMA0_end : 1;
            unsigned int DMA1_end : 1;
            unsigned int Timer0 : 1;
            unsigned int Timer1 : 1;
            unsigned int HDLCA_Tx : 1;
            unsigned int HDLCA_Rx : 1;
            unsigned int HDLCB_Tx : 1;
            unsigned int HDLCB_Rx : 1;
            unsigned int EthernetDMA_Tx : 1;
            unsigned int EthernetDMA_Rx : 1;
            unsigned int EthernetMAC_Tx : 1;
            unsigned int EthernetMAC_Rx : 1;
            unsigned int I2CEvent : 1;
            unsigned int : 11;
        };
    };
};

/* Mascarado: 1, Habilitado: 0 */
struct __attribute__((packed)) INTMSK_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int ExtSignal_GPIO08 : 1;
            unsigned int ExtSignal_GPIO09 : 1;
            unsigned int ExtSignal_GPIO10 : 1;
            unsigned int ExtSignal_GPIO11 : 1;
            unsigned int UART0_Tx : 1;
            unsigned int UART0_Rx : 1;
            unsigned int UART1_Tx : 1;
            unsigned int UART1_Rx : 1;
            unsigned int DMA0_end : 1;
            unsigned int DMA1_end : 1;
            unsigned int Timer0 : 1;
            unsigned int Timer1 : 1;
            unsigned int HDLCA_Tx : 1;
            unsigned int HDLCA_Rx : 1;
            unsigned int HDLCB_Tx : 1;
            unsigned int HDLCB_Rx : 1;
            unsigned int EthernetDMA_Tx : 1;
            unsigned int EthernetDMA_Rx : 1;
            unsigned int EthernetMAC_Tx : 1;
            unsigned int EthernetMAC_Rx : 1;
            unsigned int I2CEvent : 1;
            unsigned int GlobalInterrupt : 1;
            unsigned int : 10;
        };
    };
};

/* Configuração dos Temporizadores */
struct __attribute__((packed)) TMOD_t
{
    union
    {
        unsigned int value : 32;
        struct
        {
            unsigned int Timer0_enable : 1;
            unsigned int Timer0_toggle : 1;
            unsigned int Timer0_outputState : 1;
            unsigned int Timer1_enable : 1;
            unsigned int Timer1_toggle : 1;
            unsigned int Timer1_outputState : 1;
            unsigned int : 26;
        };
        struct
        {
            unsigned int Timer0 : 3;
            unsigned int Timer1 : 3;
            unsigned int : 26;
        };
    };
};

#endif
