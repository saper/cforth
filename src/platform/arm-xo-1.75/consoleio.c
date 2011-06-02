
// Character I/O stubs

#define UART3REG ((unsigned int volatile *)0xd4018000)  // UART3 - main board
#define UART1REG ((unsigned int volatile *)0xd4030000)  // UART1 - JTAG board

void tx1(char c)
{
    // send the character to the console output device
    while ((UART1REG[5] & 0x20) == 0)
        ;
    UART1REG[0] = (unsigned int)c;
}
void tx3(char c)
{
    // send the character to the console output device
    while ((UART3REG[5] & 0x20) == 0)
        ;
    UART3REG[0] = (unsigned int)c;
}

void tx(char c)
{
    tx1(c);
    tx3(c);
}

int putchar(int c)
{
    if (c == '\n')
        tx('\r');
    tx(c);
    return c;
}

int kbhit1() {
    return (UART1REG[5] & 0x1) != 0;
}
int kbhit3() {
    return (UART3REG[5] & 0x1) != 0;
}

int kbhit() {
    return kbhit1() || kbhit3();
}

int getchar()
{
    while (!kbhit())
        ;
    // return the next character from the console input device
    
    return (unsigned char) (kbhit1() ? UART1REG[0] : UART3REG[0]);
}

void init_io()
{
    *(int *)0xd4051024 = 0xffffffff;  // PMUM_CGR_PJ - everything on
    *(int *)0xD4015064 = 0x7;         // APBC_AIB_CLK_RST - reset, functional and APB clock on
    *(int *)0xD4015064 = 0x3;         // APBC_AIB_CLK_RST - release reset, functional and APB clock on
    *(int *)0xD401502c = 0x13;        // APBC_UART1_CLK_RST - VCTCXO, functional and APB clock on (26 mhz)
    *(int *)0xD4015034 = 0x13;        // APBC_UART3_CLK_RST - VCTCXO, functional and APB clock on (26 mhz)

//  *(int *)0xd401e120 = 0xc1;        // GPIO51 = af1 for UART3 RXD
//  *(int *)0xd401e124 = 0xc1;        // GPIO52 = af1 for UART3 TXD

    *(int *)0xd401e260 = 0xc4;        // GPIO115 = af4 for UART3 RXD
    *(int *)0xd401e264 = 0xc4;        // GPIO116 = af4 for UART3 TXD

    *(int *)0xd401e0c8 = 0xc1;        // GPIO29 = af1 for UART1 RXD
    *(int *)0xd401e0cc = 0xc1;        // GPIO30 = af1 for UART1 TXD

    UART1REG[1] = 0x40;  // Marvell-specific UART Enable bit
    UART1REG[3] = 0x83;  // Divisor Latch Access bit
    UART1REG[0] = 14;    // 115200 baud
    UART1REG[1] = 00;    // 115200 baud
    UART1REG[3] = 0x03;  // 8n1
    UART1REG[2] = 0x07;  // FIFOs and stuff

    UART3REG[1] = 0x40;  // Marvell-specific UART Enable bit
    UART3REG[3] = 0x83;  // Divisor Latch Access bit
    UART3REG[0] = 14;    // 115200 baud
    UART3REG[1] = 00;    // 11500 baud
    UART3REG[3] = 0x03;  // 8n1
    UART3REG[2] = 0x07;  // FIFOs and stuff
}

void irq_handler()
{
}

void swi_handler()
{
}

void raise()  /* In case __div and friends need it */
{
}
