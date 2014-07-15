#include "FLEX.h"
#include "stdio.h"

FLEXTREE_BEGIN(TEST_a);
	FLEXTREE_VAR(double  	,a_float);
	FLEXTREE_VAR(float 		,b_float[5UL]);
	FLEXTREE_VAR(double 	,c_float[5UL]);
	FLEXTREE_VAR(uint8_t 	,d_char);
	FLEXTREE_VAR(uint8_t 	,e_char);
FLEXTREE_END(TEST_a);


FLEXTREE_BEGIN(TEST_b);
	FLEXTREE_VAR(double  	,a_float);
	FLEXTREE_VAR(float 		,b_float[5UL]);
	FLEXTREE_VAR(double 	,c_float[5UL]);
	FLEXTREE_VAR(uint8_t 	,d_char);
	FLEXTREE_VAR(uint8_t 	,e_char);
FLEXTREE_END(TEST_b);


static uint8_t  temp_buf[1000];
static uint16_t temp_itr 	= 0;
static uint16_t temp_itr_r 	= 0;
void tx_test(uint8_t byte)
{
	temp_buf[temp_itr++] = byte;
	//printf("%c",byte);
}


uint8_t rx_test()
{
	return temp_buf[temp_itr_r++];
}


int main(void)
{
	FLEXTREE_GETVAR(TEST_a,a_float) = 0.2f;
	FLEXTREE_GETVAR(TEST_a,d_char)  = 'F';
	FLEXTREE_GETVAR(TEST_a,e_char)  = 'L';

	while(1)
	{
		temp_itr = temp_itr_r = 0;
		FLEXTREE_GETVAR(TEST_a,a_float)+=0.1f;
		//printf("  Size: %d\n", FLEXTREE_GETSIZE(TEST_a));
		//printf("  Size: %d\n", FLEXTREE_GETSIZE(TEST_b));

		FLEXTREE_SEND(TEST_a,tx_test); //printf("  Size: %d\n", FLEXTREE_GETSIZE(TEST_a));
	
				
		for( uint8_t idx=0; idx < temp_itr; idx++ )
		{
			FLEXTREE_RECV(TEST_b,rx_test);
		}
		
		printf("%f\n",FLEXTREE_GETVAR(TEST_b,a_float));
	}
	return 0UL;
}