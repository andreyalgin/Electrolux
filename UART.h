/**
	FILE UART.h
*/

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

#define UART_MESSAGE_QUEUE_LENGTH (10)


/**
	\brief Callback function pointer type definition
	\param[in] user_context passed in UART callback set functions
	\return 0 - success, -1 - error, error function will be called
*/
typedef int32_t (*UART_Callback_TypeDef)(void* user_context);

/**
	\brief Error function type definition
	\param[in] user_context passed in UART callback setup functions
	\return void
*/
typedef void (*UART_Error_TypeDef)(void* user_context);

/**
	\brief UART context enumeration
*/
typedef enum{
	UART1,		///< UART1 context index
	UART2,		///< UART2 context index
	UART3,		///< UART3 context index
	UART4,		///< UART4 context index
	UART_COUNT 	///< Total number of UARTs
}UART_Num;

/**
	\brief  UART Init Structure type definition
*/
typedef struct{
  uint32_t UART_BaudRate;				/**<	This member configures the UART communication baud rate.
												The baud rate is computed using the following formula:
												- IntegerDivider = ((UARTCLK) / (16 * (UART_InitStruct->UART_BaudRate)))
												- FractionalDivider = ((IntegerDivider - (IntegerDivider)) * 64) + 0.5 */
  uint16_t UART_WordLength;				/**<	Specifies the number of data bits transmitted or received in a frame.
												This parameter is one of UART_Word_Length values. */
  uint16_t UART_StopBits;				/**<	Specifies the number of stop bits transmitted.
												This parameter is one of UART_Stop_Bits values. */
  uint16_t UART_Parity;					/**<	Specifies the parity mode.
												This parameter is one of UART_Parity values. */
  uint16_t UART_FIFOMode;				/**<	Specifies the FIFO mode.
												This parameter is one of UART_FIFO_Mode values. */
  uint16_t UART_HardwareFlowControl;	/**<	Specifies whether the hardware flow control mode is enabled or disabled.
												This parameter is one of UART_Hardware_Flow_Control values. */
}UART_Init_TypeDef;


/**
	\brief Sends continuous sequence of bytes. Non-blocking
	\warning Internal message queue is limited by UART_MESSAGE_QUEUE_LENGTH
	\param[in] UARTn
						UART context index
	\param[in] addr
						Byte sequence start address
	\param[in] len
						Byte sequence length
	\return 0 - success, -1 - error, message queue is full
*/
int32_t UART_SendBytes(UART_Num UARTn, void* addr, uint32_t len);		// Non-blocking. It has internal message queue. Returns error if message queue is full.

/**
	\brief Sends continuous sequence of repeating bytes. Non-blocking
	\warning Internal message queue is limited by UART_MESSAGE_QUEUE_LENGTH
	\param[in] UARTn
						UART context index
	\param[in] addr
						Byte address
	\param[in] len
						Byte sequence length
	\return 0 - success, -1 - error, message queue is full
*/
int32_t UART_SendByte(UART_Num UARTn, void* addr, uint32_t len);		// Optionally. I also added separate function to send repeating sequence of the same bytes

/**
	\brief Receives continuous sequence of bytes. Non-blocking
	\warning Internal message queue is limited by UART_MESSAGE_QUEUE_LENGTH
	\param[in] UARTn
						UART context index
	\param[in] addr
						Receiving buffer address
	\param[in] len
						Byte sequence length
	\return 0 - success, -1 - error, message queue is full
*/
int32_t UART_ReceiveBytes(UART_Num UARTn, void* addr, uint32_t len);

/**
	\brief Receives continuous sequence of bytes to the same memory address. Non-blocking
	\warning Internal message queue is limited by UART_MESSAGE_QUEUE_LENGTH
	\param[in] UARTn
						UART context index
	\param[in] addr
						Receiving buffer address
	\param[in] len
						Byte sequence length
	\return 0 - success, -1 - error, message queue is full
*/
int32_t UART_ReceiveByte(UART_Num UARTn, void* addr, uint32_t len);

/**
	\brief Sets user callback and error functions for output transactions passing user context pointer to it
	\warning Library doesn't care about user context pointer content
	\param[in] UARTn UART context index
	\param[in] callback user callback function pointer
	\param[in] error user error function pointer
	\param[in] user_context pointer to user data
	\return void
*/
void UART_Set_TX_Callback(UART_Num UARTn, UART_Callback_TypeDef callback, UART_Error_TypeDef error, void* user_context);

/**
	\brief Sets user callback and error functions for input transactions passing user context pointer to it
	\warning Library doesn't care about user context pointer content
	\param[in] UARTn UART context index
	\param[in] callback user callback function pointer
	\param[in] error user error function pointer
	\param[in] user_context pointer to user data
	\return void
*/
void UART_Set_RX_Callback(UART_Num UARTn, UART_Callback_TypeDef callback, UART_Error_TypeDef error, void* user_context);

/**
	\brief Initializes selected UART module
	\param[in] UARTn UART context index
	\param[in] Init UART initialization structure
	\return 0 - success, -1 - error, can't initialize selected UART module
*/
int32_t UART_Init(UART_Num UARTn, UART_Init_TypeDef* Init);

#ifdef __cplusplus
}
#endif

#endif