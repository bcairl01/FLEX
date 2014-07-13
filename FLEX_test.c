#include "FLEX.h"
#include "stdio.h"

FLEXTREE_BEGIN(TEST_a);
	FLEXTREE_VAR(double  	,a_float);
	FLEXTREE_VAR(float 		,b_float);
	FLEXTREE_VAR(float 		,c_float);
	FLEXTREE_VAR(uint8_t 	,d_char);
	FLEXTREE_VAR(uint8_t 	,e_char);
FLEXTREE_END(TEST_a);


FLEXTREE_BEGIN(TEST_b);
	FLEXTREE_VAR(double  	,a_float);
	FLEXTREE_VAR(float 		,b_float);
	FLEXTREE_VAR(float 		,c_float);
	FLEXTREE_VAR(uint8_t 	,d_char);
	FLEXTREE_VAR(uint8_t 	,e_char);
FLEXTREE_END(TEST_b);


void tx_test(uint8_t byte)
{
	printf("%c",byte);
}


int main(void)
{
	FLEXTREE_GETVAR(TEST_a,a_float) = 0.2f;
	FLEXTREE_GETVAR(TEST_a,b_float) = 0.3f;
	FLEXTREE_GETVAR(TEST_a,c_float) = 0.4f;
	FLEXTREE_GETVAR(TEST_a,d_char)  = 5;
	FLEXTREE_GETVAR(TEST_a,e_char)  = 5;

	while(1)
	{
		FLEXTREE_GETVAR(TEST_a,a_float)+=0.1f;

		for( uint8_t idx=0; idx < FLEXTREE_GETSIZE(TEST_a); idx++ )
		{
			FLEXTREE_GETBUF(TEST_b)[idx] = FLEXTREE_GETBUF(TEST_a)[idx];
		}

		//FLEXTREE_SEND(TEST_a,tx_test); printf("  Size: %d\n", FLEXTREE_GETSIZE(TEST_a));
		//FLEXTREE_SEND(TEST_b,tx_test); printf("  Size: %d\n", FLEXTREE_GETSIZE(TEST_b));
		printf("%f\n",FLEXTREE_GETVAR(TEST_b,a_float));
	}
	return 0UL;
}