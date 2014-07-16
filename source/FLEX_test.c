#include "FLEX.h"
#include "stdio.h"


/// Some tree that will be changed on MCU_A and updated on MCU_B
FLEXTREE_BEGIN(MCU_A);
	FLEXTREE_VAR(float  	,a_float);
	FLEXTREE_VAR(float  	,more_floats[10]);
	FLEXTREE_VAR(double  	,a_double);
	FLEXTREE_VAR(char 		,a_char);
	FLEXTREE_VAR(int 		,a_int);
	FLEXTREE_VAR(long 		,a_long);
FLEXTREE_END(MCU_A);



/// Some tree that will be updated on MCU_B and changed on MCU_A
FLEXTREE_BEGIN(MCU_B);
	FLEXTREE_VAR(float  	,a_float);
	FLEXTREE_VAR(float  	,more_floats[10]);
	FLEXTREE_VAR(double  	,a_double);
	FLEXTREE_VAR(char 		,a_char);
	FLEXTREE_VAR(int 		,a_int);
	FLEXTREE_VAR(long 		,a_long);
FLEXTREE_END(MCU_B);




/// Simulates "A" sending to "B"
void transmit_to_MCU_B(uint8_t byte)
{
	FLEXTREE_RXBYTE(MCU_B,byte);
}



int main(void)
{
	/// Setup MCU_A's recieve stack to be 1000 bytes long
	FLEXTREE_SETUP(MCU_A,1000);
	
	/// Setup MCU_B's recieve stack to be 1000 bytes long
	FLEXTREE_SETUP(MCU_B,100);


	while( FLEXTREE_GETVAR(MCU_B,a_float) < 10.0f )
	{
		/// Change a variable on MCU_A
		FLEXTREE_GETVAR(MCU_A,a_float)+=0.01f;

		/// Send the tree to MCU_B
		FLEXTREE_SEND(MCU_A,transmit_to_MCU_B);
	


		/// Update MCU_B
		FLEXTREE_UPDATE(MCU_B);
		
		///Check the corresponding variabel on MCU_B
		printf("%f\n",FLEXTREE_GETVAR(MCU_B,a_float));
	}

	/// Clean up MCU_A's memory
	FLEXTREE_CLEAR(MCU_A);

	/// Clean up MCU_B's memory 
	FLEXTREE_CLEAR(MCU_B);
	return 0UL;
}