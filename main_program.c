#include "mac_address.c"
#include <stdio.h>
#include "res_sensor.c"

char location[] = "http://192.168.10.117/post_trap.php";
int main()
	{
		uint16_t gateway_ID = mac_address();
		//trap_kwh(location, 10, gateway_ID, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19);
		trap_th(location, 10, gateway_ID, 1245,1,2,1356,3005,5,6,7);
	//printf("Gateway id is %d", gateway_ID);
	return 0;
	}
