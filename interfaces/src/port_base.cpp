#include "port_base.h"

uint32_t* getBitWordAddr ( void* periphRegAddr, const uint32_t periphRegBit ) {
	uint32_t	bitWordAddr;
	bitWordAddr		=		0x42000000;
	bitWordAddr		+=		( ( uint32_t )periphRegAddr - 0x40000000 ) * 32;
	bitWordAddr		+=		periphRegBit * 4;
	return ( uint32_t* )bitWordAddr;
}