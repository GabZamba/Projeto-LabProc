#ifndef STRUCTS_H
#define STRUCTS_H

struct __attribute__((packed)) IOPCON1_t
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

struct __attribute__((packed)) IOPCON2_t
{
    union
    {
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
        unsigned int value : 32;
    };
};

struct __attribute__((packed)) IOPMOD1_t
{
    unsigned int switch4 : 1;
    unsigned int switch3 : 1;
    unsigned int switch2 : 1;
    unsigned int switch1 : 1;

    unsigned int led4 : 1;
    unsigned int led3 : 1;
    unsigned int led2 : 1;
    unsigned int led1 : 1;

    unsigned int button : 1;
    unsigned int GPIO9 : 1;

    unsigned int displayA : 1;
    unsigned int displayB : 1;
    unsigned int displayC : 1;
    unsigned int displayD : 1;
    unsigned int displayE : 1;
    unsigned int displayG : 1;
    unsigned int displayF : 1;

    unsigned int GIPO17 : 1;

    unsigned int trash : 14;
};

struct __attribute__((packed)) IOPMOD2_t
{
    // Switches
    union
    {
        unsigned int GPIO0 : 1;
        unsigned int switch4 : 1;
    };
    union
    {
        unsigned int GPIO1 : 1;
        unsigned int switch3 : 1;
    };
    union
    {
        unsigned int GPIO2 : 1;
        unsigned int switch2 : 1;
    };
    union
    {
        unsigned int GPIO3 : 1;
        unsigned int switch1 : 1;
    };

    // Leds
    union
    {
        unsigned int GPIO4 : 1;
        unsigned int led4 : 1;
        unsigned int ledGreenR : 1;
    };
    union
    {
        unsigned int GPIO5 : 1;
        unsigned int led3 : 1;
        unsigned int ledYellow : 1;
    };
    union
    {
        unsigned int GPIO6 : 1;
        unsigned int led2 : 1;
        unsigned int ledOrange : 1;
    };
    union
    {
        unsigned int GPIO7 : 1;
        unsigned int led1 : 1;
        unsigned int ledGreenL : 1;
    };

    union
    {
        unsigned int GPIO8 : 1;
        unsigned int button : 1;
        unsigned int EIRQ0 : 1;
    };

    union
    {
        unsigned int GPIO9 : 1;
        unsigned int EIRQ1 : 1;
    };

    // 7 Segments Displays
    union
    {
        unsigned int GPIO10 : 1;
        unsigned int EIRQ2 : 1;
        unsigned int displayA : 1;
    };
    union
    {
        unsigned int GPIO11 : 1;
        unsigned int EIRQ3 : 1;
        unsigned int displayB : 1;
    };
    union
    {
        unsigned int GPIO12 : 1;
        unsigned int DMAREQ0 : 1;
        unsigned int displayC : 1;
    };
    union
    {
        unsigned int GPIO13 : 1;
        unsigned int DMAREQ1 : 1;
        unsigned int displayD : 1;
    };
    union
    {
        unsigned int GPIO14 : 1;
        unsigned int DMAACK0 : 1;
        unsigned int displayE : 1;
    };
    union
    {
        unsigned int GPIO15 : 1;
        unsigned int DMAACK1 : 1;
        unsigned int displayG : 1;
    };
    union
    {
        unsigned int GPIO16 : 1;
        unsigned int T0EN0 : 1;
        unsigned int displayF : 1;
    };

    union
    {
        unsigned int GPIO17 : 1;
        unsigned int T1EN1 : 1;
    };

    unsigned int trash : 14;
};

#endif