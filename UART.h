#ifndef UART_
#define UART_

#ifdef __cplusplus
extern "C" {
#endif

#define UART_WordLength5b                   ((uint16_t)0x0000)
#define UART_WordLength6b                   ((uint16_t)0x0020)
#define UART_WordLength7b                   ((uint16_t)0x0040)
#define UART_WordLength8b                   ((uint16_t)0x0060)

#define UART_StopBits1                      ((uint16_t)0x0000)
#define UART_StopBits2                      ((uint16_t)0x0008)

#define UART_Parity_No                      ((uint16_t)0x0000)
#define UART_Parity_Even                    ((uint16_t)0x0006)
#define UART_Parity_Odd                     ((uint16_t)0x0002)
#define UART_Parity_1                       ((uint16_t)0x0082)
#define UART_Parity_0                       ((uint16_t)0x0086)

#define UART_FIFO_OFF                       ((uint16_t)0x0000)
#define UART_FIFO_ON                        ((uint16_t)0x0010)

#define UART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define UART_HardwareFlowControl_CTSEn      ((uint16_t)0x8000)
#define UART_HardwareFlowControl_RTSEn      ((uint16_t)0x4000)
#define UART_HardwareFlowControl_Out2       ((uint16_t)0x2000)
#define UART_HardwareFlowControl_Out1       ((uint16_t)0x1000)
#define UART_HardwareFlowControl_RTS        ((uint16_t)0x0800)
#define UART_HardwareFlowControl_DTR        ((uint16_t)0x0400)
#define UART_HardwareFlowControl_RXE        ((uint16_t)0x0200)
#define UART_HardwareFlowControl_TXE        ((uint16_t)0x0100)
#define UART_HardwareFlowControl_LBE        ((uint16_t)0x0080)

#define UART_MessageQueue_Length (10)


typedef uint32_t (* UART_Callback_TypeDef)(void* user_context);
typedef uint32_t (* UART_Handler_TypeDef)(uint32_t UARTn, void* argument, uint32_t len, UART_Callback_TypeDef callback, void* user_context);

typedef enum{
	UART1,
	UART2,
	UART_COUNT
}UART_Num;

typedef struct{
	void* addr;
	uint32_t len;
	UART_Callback_TypeDef callback;
	UART_Handler_TypeDef handler;	/*	Don't confuse it with interrupt handler or callback. It'll be called inside interrupt handler.
										It stores function pointer to user initially called send/receive function 
										to continue transmission with the same behaviour (for instance: UART_SendBytes or UART_SendByte)
									*/
}UART_Message_TypeDef;

typedef struct{
	uint32_t current;	// Current message index in circle message queue
	uint32_t last;		// Last message index in circle message queue

	UART_Message_TypeDef message[UART_MessageQueue_Length];
}UART_MessageQueue_TypeDef;

typedef struct{
	UART_TypeDef *UARTx;		// Pointer to device registers in periphery memory region
	
	UART_MessageQueue_TypeDef RX;
	UART_MessageQueue_TypeDef TX;
}UART_Context_TypeDef;

typedef struct{
  uint32_t UART_BaudRate; 
  uint16_t UART_WordLength;
  uint16_t UART_StopBits;
  uint16_t UART_Parity;
  uint16_t UART_FIFOMode;
  uint16_t UART_HardwareFlowControl;
}UART_InitTypeDef;


extern UART_Context_TypeDef UART_Context[]; // Defined and initialized in UART.c


int32_t UART_SendBytes(UART_Num UARTn, void* addr, uint32_t len, UART_Callback_TypeDef callback, void* user_context);		// Non-blocking. It has internal message queue. Returns error if message queue is full.
int32_t UART_SendByte(UART_Num UARTn, void* addr, uint32_t len, UART_Callback_TypeDef callback, void* user_context);		// Optionally. I also added separate function to send repeating sequence of the same bytes

int32_t UART_ReceiveBytes(UART_Num UARTn, void* addr, uint32_t len, UART_Callback_TypeDef callback, void* user_context);	// Non-blocking. It has internal message queue. Returns error if message queue is full.
int32_t UART_ReceiveByte(UART_Num UARTn, void* addr, uint32_t len, UART_Callback_TypeDef callback, void* user_context);		// Optionally. I also added separate function to receive bytes to the same memory address (to skip it)

void UART_SetCallback(UART_Num UARTn, UART_Callback_TypeDef callback, void* user_context);									// I prefer to pass user callback and context within transfer functions

int32_t UART_Init(UART_Num UARTn, UART_InitTypeDef* Init);

#ifdef __cplusplus
}
#endif

#endif