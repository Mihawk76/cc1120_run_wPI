#include "mac_address.c"
#include <stdio.h>

int main()
	{
	uint16_t gateway_ID = mac_address();
	printf("Gateway id is %d", gateway_ID);
	return 0;
	}
