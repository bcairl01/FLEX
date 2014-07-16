/// @file  FLEX.c
/// @brief A light-weight blackboarding-type communication protocol for embedded platforms
#include "FLEX.h"


/// @brief 	Switch-case wrapper for an inidividual (constant) packet element.
///			*This was done for code-reuse
#define FLEXBASE_PACKETLOCK(ch,idx,itr,_cond) 	case ch: if( (itr==idx) && (_cond) ) { ++itr; break; }




/// @brief 	Base function for transmitting tree
///	@param 	userp 		pointer to tree data-stream
///	@param 	size 		size of data-stream
///	@param 	_TX			transmit handler
void FLEXBASE_SendTree( uint8_t userp[], uint16_t size, FLEXTXService _TX )
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




/// @brief 	Base function for transmitting updating tree from recieved bytes
///	@param 	userp 		pointer to tree data-stream
///	@param 	size 		size of data-stream
///	@param 	buf_p		recieve buffer
/// @param 	_RXStat		recieve support structure
uint8_t FLEXBASE_UpdateTree( uint8_t userp[], uint16_t size, cbuffer* buf_p, FLEXRXStat_t* _RXStat )
{
	uint8_t byte;
	while( buffer_get(buf_p,&byte) )
	{
		switch(byte)
		{
			/// Header-Locking
			FLEXBASE_PACKETLOCK('F',0UL,_RXStat->head_itr,!_RXStat->data_itr)
			FLEXBASE_PACKETLOCK('L',1UL,_RXStat->head_itr,!_RXStat->data_itr)
			FLEXBASE_PACKETLOCK('E',2UL,_RXStat->head_itr,!_RXStat->data_itr)
			FLEXBASE_PACKETLOCK('X',3UL,_RXStat->head_itr,!_RXStat->data_itr)

			/// Trailer-Locking
			FLEXBASE_PACKETLOCK('<',0UL,_RXStat->tail_itr, _RXStat->data_itr==size)
			FLEXBASE_PACKETLOCK('!',1UL,_RXStat->tail_itr, _RXStat->data_itr==size)
			FLEXBASE_PACKETLOCK('>',2UL,_RXStat->tail_itr, _RXStat->data_itr==size)

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
					_RXStat->checkbuf[_RXStat->data_itr++] 	=  byte;
					_RXStat->checksum 						+= byte;
				}
				else 
				{
					FLEXRXSTAT_RESET(_RXStat);
				}
		}
	}
	return 0UL;
}