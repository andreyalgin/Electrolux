
#include "UART_Bridge.h"

static Bridge_context_TypeDef Bridge_context[UART_BRIDGE_COUNT];

static void UART_RX_Error(Bridge_context_TypeDef* bridge_context){
	if(bridge_context->error != NULL){
		bridge_context->error(bridge_context->user_context);
	}
}

static void UART_TX_Error(Bridge_context_TypeDef* bridge_context){
	if(bridge_context->error != NULL){
		bridge_context->error(bridge_context->user_context);
	}
}

static int32_t UART_TX_Tail_Callback(Bridge_context_TypeDef* bridge_context){
	
	bridge_context->double_buffer[0].state = STATE_EMPTY;
	bridge_context->double_buffer[1].state = STATE_EMPTY;
	
	if(bridge_context->callback != NULL){
		bridge_context->callback(bridge_context->user_context);
	}
	
	return 0;
}

static int32_t UART_TX_Callback(Bridge_buffer_TypeDef* bridge_buffer){
	
	bridge_buffer->state = STATE_EMPTY;
	
	return 0;
}

static int32_t UART_RX_Callback(Bridge_context_TypeDef* bridge_context){
	int32_t sts;
	uint8_t toggle;
	
	toggle = user_context->toggle;
	
	bridge_context->len -= bridge_context->delta;
	bridge_context->double_buffer[toggle].state = STATE_FULL;
	
	if(bridge_context->len == 0){
		UART_Set_TX_Callback(bridge_context->output, (UART_Callback_TypeDef)UART_TX_Tail_Callback, (UART_Error_TypeDef)UART_TX_Error, bridge_context);
	}
	
	sts = UART_SendBytes(bridge_context->output, user_context->double_buffer[toggle].data, bridge_context->delta);
	
	if(sts < 0){
		return sts; // UART_Bridge error
	}
	
	toggle = !toggle;
	
	if(user_context->double_buffer[toggle].state != STATE_EMPTY){
		return -1;
	}
	
	bridge_context->toggle = toggle;
	bridge_context->delta = min(bridge_context->len, UART_BRIDGE_BLOCK_SIZE);
	
	if(bridge_context->delta == 0){
		return 0;
	}
	
	sts = UART_ReceiveBytes(bridge_context->input, bridge_context->double_buffer[toggle].data, bridge_context->delta);
	
	if(sts < 0){
		return sts; // UART_Bridge error
	}
	
	return 0;
}


void UART_Bridge_Set_Callback(UART_Bridge_Num n, UART_Bridge_Callback_TypeDef callback, UART_Bridge_Error_TypeDef error, void* user_context){
	Bridge_context_TypeDef* bridge_context = &Bridge_context[n];
	
	bridge_context->callback = callback;
	bridge_context->error = error;
	bridge_context->user_context = user_context;
}

int32_t UART_Bridge(UART_Bridge_Num n, UART_Num input, UART_Num output, uint32_t len){
	Bridge_context_TypeDef* bridge_context = &Bridge_context[n];
	int32_t sts;

	bridge_context->double_buffer[0].state = STATE_EMPTY;
	bridge_context->double_buffer[1].state = STATE_EMPTY;
	
	bridge_context->input = input;
	bridge_context->output = output;
	
	bridge_context->len = len;
	bridge_context->delta = min(len, UART_BRIDGE_BLOCK_SIZE);
	
	bridge_context->toggle = 0;
	
	UART_Set_TX_Callback(output, (UART_Callback_TypeDef)UART_TX_Callback, (UART_Error_TypeDef)UART_TX_Error, bridge_context);
	
	UART_Set_RX_Callback(input, (UART_Callback_TypeDef)UART_RX_Callback, (UART_Error_TypeDef)UART_RX_Error, bridge_context);
	sts = UART_ReceiveBytes(input, bridge_context->double_buffer[bridge_context->toggle].data, bridge_context->delta);
	
	return sts;
}
