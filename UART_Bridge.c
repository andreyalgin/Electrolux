
#include "UART_Bridge.h"

static Bridge_context_TypeDef Bridge_context[UART_BRIDGE_COUNT];

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
		UART_Set_TX_Callback(bridge_context->destination, (UART_Callback_TypeDef)UART_TX_Tail_Callback, bridge_context->error, bridge_context);
	}
	
	sts = UART_SendBytes(bridge_context->destination, user_context->double_buffer[toggle].data, bridge_context->delta);
	
	if(sts < 0){
		return sts;
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
	
	sts = UART_ReceiveBytes(bridge_context->source, bridge_context->double_buffer[toggle].data, bridge_context->delta);
	
	if(sts < 0){
		return sts;
	}
	
	return 0;
}


void UART_Bridge_Set_Callback(UART_Bridge_Callback_TypeDef callback, UART_Bridge_Error_TypeDef error, void* user_context){
	Bridge_context.callback = callback;
	Bridge_context.error = error;
	Bridge_context.user_context = user_context;
}

int32_t UART_Bridge(UART_Bridge_Num n, UART_Num source, UART_Num destination, uint32_t len){
	int32_t sts;

	Bridge_context[n].double_buffer[0].state = STATE_EMPTY;
	Bridge_context[n].double_buffer[1].state = STATE_EMPTY;
	
	Bridge_context[n].source = source;
	Bridge_context[n].destination = destination;
	
	Bridge_context[n].len = len;
	Bridge_context[n].delta = min(len, UART_BRIDGE_BLOCK_SIZE);
	
	Bridge_context[n].toggle = 0;
	
	UART_Set_TX_Callback(destination, (UART_Callback_TypeDef)UART_TX_Callback, error, &Bridge_context[n]);
	
	UART_Set_RX_Callback(source, (UART_Callback_TypeDef)UART_RX_Callback, error, &Bridge_context[n]);
	sts = UART_ReceiveBytes(source, User_context[n].double_buffer[User_context.toggle].data, Bridge_context[n].delta);
	
	return sts;
}
