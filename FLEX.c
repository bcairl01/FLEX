
#include "FLEX.h"

void FLEXBASE_SendTree( uint8_t userp[], uint16_t size, const uint8_t head[4UL], const uint8_t tail[4UL], flexTXService _TX )
{
	for( uint16_t idx = 0; idx < 4UL; idx++ )
		_TX(head[idx]);
	
	while(size--)
		_TX(*(userp++));

	for( uint16_t idx = 0; idx < 4UL; idx++ )
		_TX(tail[idx]);
}