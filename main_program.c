#include "mac_address.c"
#include <stdio.h>
#include "res_sensor.c"

int main()
	{
	char location[] = "http://52.43.48.93/post.php";
	uint16_t gateway_ID = mac_address();
	uint16_t humidity = 2000;
	uint16_t temp1 = 3004;
	uint16_t temp2 = 3100;
	uint16_t temp3 = 3300;
	uint16_t cc1120_TH_ID = 1234;
	printf("Gateway id is %d", gateway_ID);
	res_th (location, temp1, temp2, temp3, humidity, 11, cc1120_TH_ID);
	return 0;
	}
