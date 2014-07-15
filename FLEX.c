
#include "FLEX.h"





void FLEXBASE_SendTree( uint8_t userp[], uint16_t size, flexTXService _TX )
{
	uint8_t checksum = 0;

	_TX('F');
	_TX('L');
	_TX('E');
	_TX('X');

	while(size--)
	{
		checksum += *userp; 
		_TX(*(userp++));
	}

	_TX('<');
	_TX('!');
	_TX('>');
	_TX(checksum);
}




#define PACKET_LOCK(ch,idx,itr,_cond) 	case ch: if( (itr==idx) && (_cond) ) { ++itr; break; }

uint8_t FLEXBASE_RecvTree( uint8_t userp[], uint16_t size, flexRXService _RX, flexRXStat_t* _RXStat )
{
	uint8_t byte = _RX(); 
	switch(byte)
	{
		PACKET_LOCK('F',0UL,_RXStat->head_itr,!_RXStat->data_itr)
		PACKET_LOCK('L',1UL,_RXStat->head_itr,!_RXStat->data_itr)
		PACKET_LOCK('E',2UL,_RXStat->head_itr,!_RXStat->data_itr)
		PACKET_LOCK('X',3UL,_RXStat->head_itr,!_RXStat->data_itr)

		PACKET_LOCK('<',0UL,_RXStat->tail_itr, _RXStat->data_itr==size)
		PACKET_LOCK('!',1UL,_RXStat->tail_itr, _RXStat->data_itr==size)
		PACKET_LOCK('>',2UL,_RXStat->tail_itr, _RXStat->data_itr==size)

		default:
			if( (_RXStat->data_itr == size) && (_RXStat->tail_itr == 3UL ) && (_RXStat->checksum == byte) )
			{
				FLEXRXSTAT_COPY (_RXStat,userp);
				FLEXRXSTAT_RESET(_RXStat);
				return 1UL;
			}
			else 
			if( (_RXStat->head_itr == 4UL ) && (_RXStat->tail_itr == 0UL ) )
			{
				_RXStat->temp_buf[_RXStat->data_itr++] 	=  byte;
				_RXStat->checksum 						+= byte;
			}
			else 
			{
				FLEXRXSTAT_RESET(_RXStat);
			}
	}
	return 0UL;
}