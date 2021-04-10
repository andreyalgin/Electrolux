
#include "UART.h"

#define UART_BLOCK_SIZE (128)


static void UART1_Init();
static void UART2_Init();


typedef enum{
	STATE_EMPTY,
	STATE_FULL
}State;

typedef struct{
	uint8_t data[UART_BLOCK_SIZE];
	State state;
}User_buffer_TypeDef;

typedef struct{
	User_buffer_TypeDef double_buffer[2];
	uint8_t toggle;
}User_context_TypeDef;


User_context_TypeDef User_context;


uint32_t UART2_TX_Callback(User_buffer_TypeDef* user_buffer){
	
	user_buffer->state = STATE_EMPTY;
	
	return 0;
}

uint32_t UART1_RX_Callback(User_context_TypeDef* user_context){
	uint8_t toggle;
	
	toggle = user_context->toggle;

	user_context->double_buffer[toggle].state = STATE_FULL;
	
	UART_SendBytes(UART2, user_context->double_buffer[toggle].data, UART_BLOCK_SIZE, (UART_Callback_TypeDef)UART2_TX_Callback, &user_context->double_buffer[toggle]);
	
	toggle = !toggle;
	user_context->toggle = toggle;
	
	if(user_context->double_buffer[toggle].state != STATE_EMPTY){
		Debug("UART1_RX_Callback error: double buffer overflow\n");
		
		return 0;
	}
	
	UART_ReceiveBytes(UART1, user_context->double_buffer[toggle].data, UART_BLOCK_SIZE, (UART_Callback_TypeDef)UART1_RX_Callback, user_context);
	
	return 0;
}

int main(){

	UART1_Init();
	UART2_Init();
	
	User_context.double_buffer[0].state = STATE_EMPTY;
	User_context.double_buffer[1].state = STATE_EMPTY;
	User_context.toggle = 0;
	
	UART_ReceiveBytes(UART1, User_context.double_buffer[User_context.toggle].data, UART_BLOCK_SIZE, (UART_Callback_TypeDef)UART1_RX_Callback, &User_context);

	while(1){
		
	}
}

static void UART1_Init(){
	UART_Init_TypeDef UART_InitStructure;
	int32_t sts;
	
	UART_InitStructure.UART_BaudRate = 9600;
	UART_InitStructure.UART_WordLength = UART_WordLength8b;
	UART_InitStructure.UART_StopBits = UART_StopBits1;
	UART_InitStructure.UART_Parity =  UART_Parity_No;
	UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE;
	
	sts = UART_Init(UART1, &UART_InitStructure);
	
	if(sts < 0){
		Debug("UART1_Init error: can't initialize UART1\n");
	}
}

static void UART2_Init(){
	UART_Init_TypeDef UART_InitStructure;
	int sts;
	
	UART_InitStructure.UART_BaudRate = 115200;
	UART_InitStructure.UART_WordLength = UART_WordLength8b;
	UART_InitStructure.UART_StopBits = UART_StopBits1;
	UART_InitStructure.UART_Parity =  UART_Parity_No;
	UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE;
	
	sts = UART_Init(UART2, &UART_InitStructure);
	
	if(sts < 0){
		Debug("UART2_Init error: can't initialize UART2\n");
	}
}