#include "mac_address.c"
#include <stdio.h>
//#include "res_sensor.c"
#include "paring.c"

char location[] = "http://192.168.88.19:1616/dcms/rest/alfa";
int main()
	{
		//uint16_t gateway_ID = mac_address();
		int humidity = 3004;
		int temp1 = 2005;
		int temp2 = 3005;
		int temp3 = 3005;
		int rssi = -10;
		int i;
		//trap_kwh(location, 10, gateway_ID, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19)
		//trap_th(location, 10, gateway_ID, 1245,1,2,1356,3005,5,6,7);
		//res_th (location, 3003, 3005, 34000, 4000, 11, gateway_ID);
		//trap_th (location, 1, gateway_ID, 0x1003, 0, 0, humidity, temp1, temp2, temp3, rssi);
		/*get_id_pairing("localhost","root","satunol10","paring","th",1001);
		for(i=0;i<=total_pairing;i++)
		{
			printf("%02X %d %d\n", pairing_id[i], pairing_id[i], i+1);
		}
		get_id_pairing("localhost","root","satunol10","paring","th",1002);
		for(i=0;i<=total_pairing;i++)
		{
			printf("%02X %d %d\n", pairing_id[i], pairing_id[i], i+1);
		}*/
		get_id("localhost","root","satunol10","paring","main");
		printf("%d\n",mysql_id);
		//printf("Gateway id is %d", gateway_ID);
		return 0;
	}
