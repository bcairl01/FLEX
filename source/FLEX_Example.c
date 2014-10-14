#include "FLEX.h"
#include "stdio.h"



/// On one micro-controller, you define a FLEXTree corresponding to 
///	value you would like to transmit to Micro-controller 'B'
/// Some tree that will be changed on MCU_A_TX and updated on MCU_B_RX
FLEXTREE_BEGIN(MCU_A_TX);
	FLEXTREE_VAR(float  	,a_float);
	FLEXTREE_VAR(float  	,more_floats[10]);
	FLEXTREE_VAR(double  	,a_double);
	FLEXTREE_VAR(char 		,a_char);
	FLEXTREE_VAR(int 		,a_int);
	FLEXTREE_VAR(long 		,a_long);
FLEXTREE_END(MCU_A_TX);





/// On another micro-controller, you defined a FLEXTree corresponding to 
///	value you are recieving from Micro-controller 'A'. 
/// Some tree that will be updated on MCU_B_RX and changed on MCU_A_TX
FLEXTREE_BEGIN(MCU_B_RX);
	FLEXTREE_VAR(float  	,a_float);
	FLEXTREE_VAR(float  	,more_floats[10]);
	FLEXTREE_VAR(double  	,a_double);
	FLEXTREE_VAR(char 		,a_char);
	FLEXTREE_VAR(int 		,a_int);
	FLEXTREE_VAR(long 		,a_long);
FLEXTREE_END(MCU_B_RX);




/// MCU-B recieves a byte and puts it in its internal ring-buffer
void MCU_B_recieve_MCU_A(uint8_t byte)
{
	FLEXTREE_RXBYTE(MCU_B_RX,byte);
}



/// Simulates "A" sending to "B" and B recieving
void MCU_A_transmit_MCU_B(uint8_t byte)
{
	// This would be handled by an interrupt on MCU-B
	MCU_B_recieve_MCU_A(uint8_t byte);
}





int main(void)
{
	/// Setup MCU_A_TX's recieve stack to be 0 bytes long; this tree is being transmitted
	FLEXTREE_SETUP(MCU_A_TX,0);
	
	/// Setup MCU_B_RX's recieve stack to be 1000 bytes long
	FLEXTREE_SETUP(MCU_B_RX,100);


	while( FLEXTREE_GETVAR(MCU_B_RX,a_float) < 10.0f )
	{
		/// Change a variable on MCU_A_TX
		FLEXTREE_GETVAR(MCU_A_TX,a_float)+=0.01f;

		/// Send the tree to MCU_B_RX
		FLEXTREE_SEND(MCU_A_TX,transmit_to_MCU_B_RX);
	


		/// Update MCU_B_RX
		FLEXTREE_UPDATE(MCU_B_RX);
		
		///Check the corresponding variabel on MCU_B_RX
		printf("%f\n",FLEXTREE_GETVAR(MCU_B_RX,a_float));
	}

	/// Clean up MCU_A_TX's memory
	FLEXTREE_CLEAR(MCU_A_TX);

	/// Clean up MCU_B_RX's memory 
	FLEXTREE_CLEAR(MCU_B_RX);
	return 0UL;
}