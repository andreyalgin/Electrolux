#ifndef UART_BRIDGE_
#define UART_BRIDGE_

#ifdef __cplusplus
extern "C" {
#endif

#include "UART.h"

#define UART_BRIDGE_BLOCK_SIZE (128)

typedef void (*UART_Bridge_Callback_TypeDef)(void* user_context);
typedef UART_Error_TypeDef UART_Bridge_Error_TypeDef;

typedef enum{
	UART_BRIDGE1,
	UART_BRIDGE2,
	UART_BRIDGE3,
	UART_BRIDGE4,
	UART_BRIDGE_COUNT
}UART_Bridge_Num;

typedef enum{
	STATE_EMPTY,
	STATE_FULL
}State;

typedef struct{
	uint8_t data[UART_BRIDGE_BLOCK_SIZE];
	State state;
}Bridge_buffer_TypeDef;

typedef struct{
	Bridge_buffer_TypeDef double_buffer[2];
	
	UART_Num input;
	UART_Num output;
	
	uint32_t len;
	uint32_t delta;
	
	UART_Bridge_Callback_TypeDef callback;
	UART_Bridge_Error_TypeDef error;
	void* user_context;
	
	uint8_t toggle;
}Bridge_context_TypeDef;


void UART_Bridge_Set_Callback(UART_Bridge_Num n, UART_Bridge_Callback_TypeDef callback, UART_Bridge_Error_TypeDef error, void* user_context);
int32_t UART_Bridge(UART_Bridge_Num n, UART_Num input, UART_Num output, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif