
#include "UART.h"
#include "UART_Bridge.h"

#define UART_BRIDGE_DATA_LENGTH (448)

typedef struct{
	uint8_t out_data[UART_BRIDGE_DATA_LENGTH];
	uint8_t in_data[UART_BRIDGE_DATA_LENGTH];
}UART_Bridge_User_Context_TypeDef;

static UART_Bridge_User_Context_TypeDef User_context;

static void UART1_Init();
static void UART2_Init();
static void UART3_Init();
static void UART4_Init();

static void UART_Bridge_Error(User_context_TypeDef* user_context){
	Debug("UART_Bridge error: double buffer overflow\n");

}

static void UART_Error(User_context_TypeDef* user_context){
	Debug("UART error: something went wrong\n");

}

static void UART_Bridge_Callback(User_context_TypeDef* user_context){
	Debug("UART_Bridge: data transmission completed !\n");

}

static int32_t UART_TX_Callback(UART_Bridge_User_Context_TypeDef user_context){
	Debug("UART_TX: data sending completed !\n");
	
	return 0;
}

static int32_t UART_RX_Callback(UART_Bridge_User_Context_TypeDef user_context){
	Debug("UART_RX: data receiving completed !\n");
	
	return 0;
}

static int32_t Data_Compare(uint8_t* sample1, uint8_t* sample2, uint32_t len){
	while(len--){
		if(*sample1++ != *sample2++){
			return -1;
		}
	}
	
	return 0;
}

int main(){
	int32_t sts;

	UART1_Init();
	UART2_Init();
	UART3_Init();
	UART4_Init();
	
	// UART3_TX --> UART1_RX Testing
	UART_Set_RX_Callback(UART1, (UART_Callback_TypeDef)UART_RX_Callback, (UART_Error_TypeDef)UART_Error);
	sts = UART_ReceiveBytes(UART1, User_context.in_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("UART1_ReceiveBytes error\n");
	}
	
	UART_Set_TX_Callback(UART3, (UART_Callback_TypeDef)UART_TX_Callback, (UART_Error_TypeDef)UART_Error);
	sts = UART_SendBytes(UART3, User_context.out_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("UART3_SendBytes error\n");
	}
	
	sts = Data_Compare(User_context.out_data, User_context.in_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("Data_Compare error\n");
	}
	
	
	// UART2_TX --> UART4_RX Testing
	UART_Set_RX_Callback(UART4, (UART_Callback_TypeDef)UART_RX_Callback, (UART_Error_TypeDef)UART_Error);
	sts = UART_ReceiveBytes(UART4, User_context.in_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("UART4_ReceiveBytes error\n");
	}
	
	UART_Set_TX_Callback(UART2, (UART_Callback_TypeDef)UART_TX_Callback, (UART_Error_TypeDef)UART_Error);
	sts = UART_SendBytes(UART2, User_context.out_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("UART2_SendBytes error\n");
	}
	
	
	// UART3_TX --> [UART1_RX ~~> UART2_TX] --> UART4_RX Bridge testing
	UART_Bridge_Set_Callback((UART_Bridge_Callback_TypeDef)UART_Bridge_Callback, (UART_Bridge_Error_TypeDef)UART_Bridge_Error, &User_context);
	sts = UART_Bridge(UART_BRIDGE1, UART1, UART2, UART_BRIDGE_DATA_LENGTH);
	
	// Passing data through bridge
	UART_Set_RX_Callback(UART4, (UART_Callback_TypeDef)UART_RX_Callback, (UART_Error_TypeDef)UART_Error);
	sts = UART_ReceiveBytes(UART4, User_context.in_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("UART4_ReceiveBytes error\n");
	}
	
	UART_Set_TX_Callback(UART3, (UART_Callback_TypeDef)UART_TX_Callback, (UART_Error_TypeDef)UART_Error);
	sts = UART_SendBytes(UART3, User_context.out_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("UART3_SendBytes error\n");
	}
	
	sts = Data_Compare(User_context.out_data, User_context.in_data, UART_BRIDGE_DATA_LENGTH);
	
	if(sts < 0){
		Debug("Data_Compare error\n");
	}

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
		Debug("UART1_Init error\n");
	}
}

static void UART2_Init(){
	UART_Init_TypeDef UART_InitStructure;
	int32_t sts;
	
	UART_InitStructure.UART_BaudRate = 115200;
	UART_InitStructure.UART_WordLength = UART_WordLength8b;
	UART_InitStructure.UART_StopBits = UART_StopBits1;
	UART_InitStructure.UART_Parity =  UART_Parity_No;
	UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE;
	
	sts = UART_Init(UART2, &UART_InitStructure);
	
	if(sts < 0){
		Debug("UART2_Init error\n");
	}
}

static void UART3_Init(){
	UART_Init_TypeDef UART_InitStructure;
	int32_t sts;
	
	UART_InitStructure.UART_BaudRate = 9600;
	UART_InitStructure.UART_WordLength = UART_WordLength8b;
	UART_InitStructure.UART_StopBits = UART_StopBits1;
	UART_InitStructure.UART_Parity =  UART_Parity_No;
	UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE;
	
	sts = UART_Init(UART3, &UART_InitStructure);
	
	if(sts < 0){
		Debug("UART3_Init error\n");
	}
}

static void UART4_Init(){
	UART_Init_TypeDef UART_InitStructure;
	int32_t sts;
	
	UART_InitStructure.UART_BaudRate = 115200;
	UART_InitStructure.UART_WordLength = UART_WordLength8b;
	UART_InitStructure.UART_StopBits = UART_StopBits1;
	UART_InitStructure.UART_Parity =  UART_Parity_No;
	UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE;
	
	sts = UART_Init(UART4, &UART_InitStructure);
	
	if(sts < 0){
		Debug("UART4_Init error\n");
	}
}
