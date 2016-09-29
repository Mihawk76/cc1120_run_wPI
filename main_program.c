#include "mac_address.c"
#include <stdio.h>
#include "res_sensor.c"

int main()
	{
	char* location = "http://10.42.0.1/post_array.php";
	uint16_t gateway_ID = mac_address();
	uint16_t humidity = 2000;
	uint16_t temp1 = 3004;
	uint16_t temp2 = 3100;
	uint16_t temp3 = 3300;
	uint16_t cc1120_TH_ID = 1234;
	int channel = 10;
	printf("Gateway id is %d\n", gateway_ID);
	uint16_t kwh[18];
	int i;
	uint16_t t = 50;
	for (i=0;i<=18;i++)
	{
		kwh[i]=t;
		t=t-1;
	}
	//res_kwh_2 (location, kwh, 4004, 2000, 1003, 4004, 2000, 14, cc1120_TH_ID, channel);
	res_kwh_array(location, kwh, kwh, kwh, kwh, kwh, kwh, 14, 1002); 
	return 0;
	}
