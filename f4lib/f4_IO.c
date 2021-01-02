#include "f4lib.h"

void IO_init () {
    GPIO_InitTypeDef gp;
  
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |\
        RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |\
        RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);
 
    GPIO_StructInit (&gp);
    gp.GPIO_Speed = GPIO_Speed_50MHz;   

    gp.GPIO_Mode = GPIO_Mode_OUT;    
    gp.GPIO_OType = GPIO_OType_PP;
    gp.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init (GPIOB, &gp);

    gp.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init (GPIOC, &gp);

    gp.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init (GPIOF, &gp);

    gp.GPIO_Mode = GPIO_Mode_IN;
    gp.GPIO_PuPd = GPIO_PuPd_DOWN;
    gp.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init (GPIOA, &gp);

    gp.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init (GPIOF, &gp);

    gp.GPIO_PuPd = GPIO_PuPd_UP;
    gp.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIO_Init (GPIOF, &gp);
}

void IO_USART_init () {                                                
    GPIO_InitTypeDef gp;
    USART_InitTypeDef us; 
    NVIC_InitTypeDef ic;

    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);

    GPIO_StructInit (&gp);
    gp.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gp.GPIO_Mode = GPIO_Mode_AF;
    gp.GPIO_Speed = GPIO_Speed_2MHz;
    gp.GPIO_OType = GPIO_OType_PP;
    gp.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init (GPIOA, &gp);
   
    GPIO_PinAFConfig (GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig (GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    USART_Cmd (USART1, ENABLE);

    USART_StructInit (&us);
    us.USART_BaudRate = 9600;
    us.USART_WordLength = USART_WordLength_8b;
    us.USART_StopBits = USART_StopBits_1;
    us.USART_Parity = USART_Parity_No;
    us.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    us.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init (USART1, &us);
    
    USART_ITConfig (USART1, USART_IT_RXNE, ENABLE);
    
    NVIC_ClearPendingIRQ (USART1_IRQn);
    ic.NVIC_IRQChannel = USART1_IRQn;
    ic.NVIC_IRQChannelPreemptionPriority = 10;
    ic.NVIC_IRQChannelSubPriority = 0;
    ic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&ic);
    NVIC_EnableIRQ (USART1_IRQn);
}

void IO_USART_putchar (char ch) {
    while (USART_GetFlagStatus (USART1, USART_FLAG_TXE) == RESET)
        misc_udelay (1);
    USART_SendData (USART1, (uint8_t) ch);
    while (USART_GetFlagStatus (USART1, USART_FLAG_TXE) == RESET)
        misc_udelay (1);
}

void IO_USART_puts (const char *_s) {
    while (*_s)
        IO_USART_putchar (*(_s++));
}

extern void IO_debug (const char *_s) {
  #ifdef SERIAL_CON
    IO_USART_puts ("debug: ");
    IO_USART_puts (_s);
    IO_USART_putchar ('\r');
    IO_USART_putchar ('\n');
  #endif
}

void IO_LCD_init () {
    GPIO_InitTypeDef gp;
    FSMC_NORSRAMInitTypeDef sr;
    FSMC_NORSRAMTimingInitTypeDef rw_timing; 
    FSMC_NORSRAMTimingInitTypeDef w_timing;

    RCC_AHB3PeriphClockCmd (RCC_AHB3Periph_FSMC, ENABLE);

    gp.GPIO_Speed = GPIO_Speed_50MHz;
    gp.GPIO_Mode = GPIO_Mode_AF;
    gp.GPIO_OType = GPIO_OType_PP;

    GPIO_PinAFConfig (GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
    gp.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |\
        GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init (GPIOD, &gp); 

    GPIO_PinAFConfig (GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);
    gp.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |\
        GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |\
        GPIO_Pin_15;     
    GPIO_Init (GPIOE, &gp); 

    GPIO_PinAFConfig (GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig (GPIOG, GPIO_PinSource0, GPIO_AF_FSMC);
    gp.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;    
    GPIO_Init (GPIOG, &gp); 

    rw_timing.FSMC_AddressSetupTime = 0x01;   
    rw_timing.FSMC_AddressHoldTime = 0x00;     
    rw_timing.FSMC_DataSetupTime = 0x0f;  
    rw_timing.FSMC_BusTurnAroundDuration = 0x00;
    rw_timing.FSMC_CLKDivision = 0x00;
    rw_timing.FSMC_DataLatency = 0x00;
    rw_timing.FSMC_AccessMode = FSMC_AccessMode_A;    

    w_timing.FSMC_AddressSetupTime = 0x02;
    w_timing.FSMC_AddressHoldTime = 0x00;        
    w_timing.FSMC_DataSetupTime = 0x05;  
    w_timing.FSMC_BusTurnAroundDuration = 0x00;
    w_timing.FSMC_CLKDivision = 0x00;
    w_timing.FSMC_DataLatency = 0x00;
    w_timing.FSMC_AccessMode = FSMC_AccessMode_A;     

    sr.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    sr.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
    sr.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    sr.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    sr.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    sr.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    sr.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    sr.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    sr.FSMC_WriteOperation = FSMC_WriteOperation_Enable; 
    sr.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    sr.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable; 
    sr.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 
    sr.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    sr.FSMC_ReadWriteTimingStruct = &rw_timing; 
    sr.FSMC_WriteTimingStruct = &w_timing; 
    FSMC_NORSRAMInit (&sr);  

    FSMC_NORSRAMCmd (FSMC_Bank1_NORSRAM4, ENABLE);  
}

void IO_LCD_backlight (uint8_t en_) {
    if (en_)
        GPIO_SetBits (GPIOB, GPIO_Pin_0);
    else
        GPIO_ResetBits (GPIOB, GPIO_Pin_0);
}

void IO_LCD_reset (uint8_t en_) {
    if (! en_)
        GPIO_ResetBits (GPIOC, GPIO_Pin_5);
    else
        GPIO_SetBits (GPIOC, GPIO_Pin_5);
}

uint8_t IO_but1_ () {
    return GPIO_ReadInputDataBit (GPIOA, GPIO_Pin_0);
}

uint8_t IO_but2_ () {
    return GPIO_ReadInputDataBit (GPIOF, GPIO_Pin_11);
}

uint8_t IO_tch_ () {
    return ! GPIO_ReadInputDataBit (GPIOF, GPIO_Pin_10);
}

// bitbanging SPI read
uint16_t IO_tch_SPI_read () {
    uint16_t val = 0;
    for (uint8_t i = 0; i < 12; i++) {
        val <<= 1;
        // SCK 1
        GPIO_SetBits (GPIOB, GPIO_Pin_1);
        misc_udelay (1);
        // SCK 0
        GPIO_ResetBits (GPIOB, GPIO_Pin_1);
        misc_udelay (1);
        // MISO
        if (GPIO_ReadInputDataBit (GPIOF, GPIO_Pin_8))
            val++;
    }
    return val;
}

// bitbanging SPI write
void IO_tch_SPI_write (uint8_t val) {
    // SCK 0
    GPIO_ResetBits (GPIOB, GPIO_Pin_1);
    misc_udelay (1);
    for (uint8_t i = 0; i < 8; i++) {
        if (val & 128) // MOSI 1
            GPIO_SetBits (GPIOF, GPIO_Pin_9);
        else // MOSI 0
            GPIO_ResetBits (GPIOF, GPIO_Pin_9);
        val <<= 1;

        // SCK 0
        GPIO_ResetBits (GPIOB, GPIO_Pin_1);
        misc_udelay (1);
        // SCK 1
        GPIO_SetBits (GPIOB, GPIO_Pin_1);
        misc_udelay (1);
    }
    // SCK 1
    GPIO_SetBits (GPIOB, GPIO_Pin_1);
    misc_udelay (1);
    // SCK 0
    GPIO_ResetBits (GPIOB, GPIO_Pin_1);
    misc_udelay (1);
}

void IO_tch_get_pt (uint16_t *_pt_x, uint16_t *_pt_y) {
    // CS 0
    GPIO_ResetBits (GPIOB, GPIO_Pin_2);

    IO_tch_SPI_write (0xd0);
    *_pt_x = IO_tch_SPI_read ();

    IO_tch_SPI_write (0x90);
    *_pt_y = IO_tch_SPI_read ();

    // CS 1
    GPIO_SetBits (GPIOB, GPIO_Pin_2);
}

