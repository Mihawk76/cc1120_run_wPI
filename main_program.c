#include "mac_address.c"
#include <stdio.h>

int main()
	{
	uint16_t gateway_ID = mac_address();
	printf("Gateway id is %04X", gateway_ID);
	return 0;
	}
