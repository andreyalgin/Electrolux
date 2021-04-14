#ifndef UART_BRIDGE_
#define UART_BRIDGE_

#ifdef __cplusplus
extern "C" {
#endif

#include "UART.h"

#define UART_BRIDGE_BLOCK_SIZE (128)

/**
	\brief Callback function pointer type definition
	\param[in] user_context passed in UART bridge callback setup functions
	\return void
*/
typedef void (*UART_Bridge_Callback_TypeDef)(void* user_context);

/**
	\brief Error function type definition
	\param[in] user_context passed in UART bridge callback setup functions
	\return void
*/
typedef UART_Error_TypeDef UART_Bridge_Error_TypeDef;

///UART Bridge context index enumeration
typedef enum{
	UART_BRIDGE1,		///< UART_BRIDGE1 context index
	UART_BRIDGE2,		///< UART_BRIDGE2 context index
	UART_BRIDGE3,		///< UART_BRIDGE3 context index
	UART_BRIDGE4,		///< UART_BRIDGE4 context index
	UART_BRIDGE_COUNT	///< Total number of UART bridges
}UART_Bridge_Num;

///UART bridge buffer states enumeration
typedef enum{
	STATE_EMPTY,		///< buffer data is sent
	STATE_FULL			///< buffer contains unsent data
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
	
	uint8_t toggle; // double buffer toggle
}Bridge_context_TypeDef;


/**
	\brief Setups user callback and error functions passing user context pointer to it
	\warning Library doesn't care about user context pointer content
	\param[in] n UART bridge context index
	\param[in] callback user callback function pointer
	\param[in] error user error function pointer
	\param[in] user_context pointer to user data
	\return void
*/
void UART_Bridge_Set_Callback(UART_Bridge_Num n, UART_Bridge_Callback_TypeDef callback, UART_Bridge_Error_TypeDef error, void* user_context);

/**
	\brief Setups selected UART bridge module
	\param[in] n UART bridge context index
	\param[in] input UART input module context index
	\param[in] output UART output module context index
	\return 0 - success, -1 - error
*/
int32_t UART_Bridge(UART_Bridge_Num n, UART_Num input, UART_Num output, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif