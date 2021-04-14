/**
	FILE UART_Bridge.h
*/

#ifndef UART_BRIDGE_
#define UART_BRIDGE_

#ifdef __cplusplus
extern "C" {
#endif

#include "UART.h"

#define UART_BRIDGE_BLOCK_SIZE (128)

/**
	\brief Callback function pointer type definition
	\param[in] user_context passed in UART bridge callback set functions
	\return void
*/
typedef void (*UART_Bridge_Callback_TypeDef)(void* user_context);

/**
	\brief Error function type definition
	\param[in] user_context passed in UART bridge callback set functions
	\return void
*/
typedef UART_Error_TypeDef UART_Bridge_Error_TypeDef;

/**
	\brief UART Bridge context index enumeration
*/
typedef enum{
	UART_BRIDGE1,		///< UART_BRIDGE1 context index
	UART_BRIDGE2,		///< UART_BRIDGE2 context index
	UART_BRIDGE3,		///< UART_BRIDGE3 context index
	UART_BRIDGE4,		///< UART_BRIDGE4 context index
	UART_BRIDGE_COUNT	///< Total number of UART bridges
}UART_Bridge_Num;

/**
	\brief UART bridge buffer states enumeration
*/
typedef enum{
	STATE_EMPTY,		///< buffer data is sent
	STATE_FULL			///< buffer contains unsent data
}State;

/**
	\brief UART bridge buffer type definition
*/
typedef struct{
	uint8_t data[UART_BRIDGE_BLOCK_SIZE];	///<	data block
	State state;							///<	state of current buffer	
}Bridge_buffer_TypeDef;

/**
	\brief UART bridge context type definition
*/
typedef struct{
	Bridge_buffer_TypeDef double_buffer[2];	///<	double_buffer array with data blocks
	
	UART_Num input;							///<	Input UART context index in current UART bridge session
	UART_Num output;						///<	Output UART context index in current UART bridge session
	
	uint32_t len;							///<	Expected data length passing through bridge
	uint32_t delta;							///<	A portion of data in single data block
	
	UART_Bridge_Callback_TypeDef callback;	///<	User callback function pointer
	UART_Bridge_Error_TypeDef error;		///<	User error function pointer
	void* user_context;						///<	User context data pointer
	
	uint8_t toggle;							/**<	double_buffer toggle.
													Switches between zero and first double_buffer index*/
}Bridge_context_TypeDef;


/**
	\brief Sets user callback and error functions passing user context pointer to it
	\warning Library doesn't care about user context pointer content
	\param[in] n UART bridge context index
	\param[in] callback user callback function pointer
	\param[in] error user error function pointer
	\param[in] user_context pointer to user data
	\return void
*/
void UART_Bridge_Set_Callback(UART_Bridge_Num n, UART_Bridge_Callback_TypeDef callback, UART_Bridge_Error_TypeDef error, void* user_context);

/**
	\brief Sets selected UART bridge module
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