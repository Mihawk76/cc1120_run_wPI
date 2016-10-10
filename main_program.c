#include "mac_address.c"
#include <stdio.h>
#include "res_sensor.c"

int main()
	{
	//char* location = "http://10.42.0.1/post.php";
	//char* location = "http://52.43.48.93/post.php";
	char* location = "http://192.168.88.19:1616/dcms/rest/alfa";
	//uint16_t gateway_ID = mac_address();
  char* gateway_ID = "EM24010101";
	uint16_t humidity = 2000;
	uint16_t temp1 = 3124;
	uint16_t temp2 = 3100;
	uint16_t temp3 = 3300;
	uint16_t cc1120_TH_ID = 1234;
	uint16_t gateway = 1002;
	int channel = 10;
	//printf("Gateway id is %d\n", gateway_ID);
	int32_t kwh[18];
	int i;
	int32_t t = 50;
	for (i=0;i<=18;i++)
	{
		kwh[i]=t;
		t=t-1;
	}
  //trap_th(location, 10, gateway_ID, 1245,1,2,1356,3005,5,6,7);
    trap_kwh_array(location, gateway_ID
                , kwh, kwh, kwh 
                , kwh, kwh, kwh 
                , kwh, kwh, kwh 
                , kwh, kwh, kwh 
                , kwh, kwh, kwh 
                , kwh, kwh, kwh, kwh);
	//res_kwh_2 (location, kwh, 4004, 2000, 1003, 4004, 2000, 14, cc1120_TH_ID, channel);
	/*res_kwh_array(location, kwh, kwh, kwh, 
								kwh, kwh, kwh, kwh, 
								kwh, kwh, 14, 1002, gateway);*/
	//res_th (location, temp1, temp2, temp3, humidity, 11, cc1120_TH_ID, gateway);
	//res_th_2 (location, temp1, temp2, temp3, humidity, 11, cc1120_TH_ID);
	return 0;
	}
