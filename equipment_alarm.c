#define _GNU_SOURCE 1
#include <inttypes.h>
#include <stdio.h>   
#include <stdlib.h>    
#include <stdint.h>  
#include <string.h>  
#include <time.h>
#include <my_global.h>
#include <mysql.h>
#include <curl/curl.h>
#include "paring.h"
#include "equipment_alarm.h"
#include "kwh_params.h"
#include "app.h"

EQ_MAINPOWER_T main_power_equipt[3];
EQ_AC_T        ac_equipt[5];
EQ_CHILLER_T   chiller_equipt[6];
EQ_DOOR_T      door_equipt[2];
EQ_ROOM_T      room_equipt[2];
EQ_PUMP_T      pump_equipt[2];
EQ_LAMP_T      lamp_equipt[10];


EQ_ALARM_T     alarm_buffer[ALARM_BUFFER_SIZE];
uint16_t       alarm_head_ptr, alarm_tail_ptr;

//char* location_alarm = "http://alfademo.ddns.net:3000/api/ths/save";
char* location_alarm = "http://ec2-35-166-240-126.us-west-2.compute.amazonaws.com:3000/api/alarms/save";


uint8_t equipment_count[EQ_MAX]={
  sizeof(main_power_equipt)/sizeof(EQ_MAINPOWER_T),
  sizeof(ac_equipt)/sizeof(EQ_AC_T),
  sizeof(chiller_equipt)/sizeof(EQ_CHILLER_T),
  sizeof(door_equipt)/sizeof(EQ_DOOR_T),
  sizeof(room_equipt)/sizeof(EQ_ROOM_T),
  sizeof(pump_equipt)/sizeof(EQ_PUMP_T),
  sizeof(lamp_equipt)/sizeof(EQ_LAMP_T)
};

int insert_new_alarm_to_buffer( time_t timeStamp, uint16_t equipmentType, uint16_t equipmentId, uint16_t alarmCode, char * values){
  uint16_t local_ptr;
  
  local_ptr = (alarm_head_ptr+1) & ALARM_BUFFER_MASK;
	
  if (local_ptr == alarm_tail_ptr) 
	  return -1; // buffer alarm full
  
  alarm_buffer[alarm_head_ptr].timeStamp = timeStamp;
  alarm_buffer[alarm_head_ptr].equipmentType = equipmentType;
  alarm_buffer[alarm_head_ptr].equipmentId = equipmentId;
  alarm_buffer[alarm_head_ptr].alarmCode = alarmCode;
  memset(alarm_buffer[alarm_head_ptr].value,0,sizeof(alarm_buffer[alarm_head_ptr].value));
  //sprintf(alarm_buffer[alarm_head_ptr].value,"[%s]",values);
  sprintf(alarm_buffer[alarm_head_ptr].value,"%s",values);
  alarm_head_ptr = 	local_ptr;
  return local_ptr;
}

int res_equipment_alarm (char* location, EQ_ALARM_T eq_alarm, int gateway) {
	int ret_val = 0;
	char resData[1000];
 	int len;
	CURL *curl;
	CURLcode res; 
	
	struct tm *tms = localtime(&eq_alarm.timeStamp);

	//len += snprintf(resData, sizeof resData, "Data=");
	len = snprintf(resData, sizeof resData, "{");
	len += snprintf(&resData[len], sizeof(resData) - len, "\"Gateway_id\": %d,",gateway);
	//len += snprintf(&resData[len], sizeof(resData) - len, "&TimeStamp=\"%04d-%02d-%02d %02d:%02d:%02d\"",
	  //tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);
	len += snprintf(&resData[len], sizeof(resData) - len, "\"EquipmentType\": %d,",eq_alarm.equipmentType);
	len += snprintf(&resData[len], sizeof(resData) - len, "\"EquipmentId\": %d,",eq_alarm.equipmentId);
	len += snprintf(&resData[len], sizeof(resData) - len, "\"AlarmCode\": %d,",eq_alarm.alarmCode);
	len += snprintf(&resData[len], sizeof(resData) - len, "\"value\": %s}",eq_alarm.value);
	printf("%s\n", resData);
	
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, resData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK){
		   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
       ret_val = 1;		
	   }
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	
	return ret_val;
}


void * alarm_checking(void * arg ) {
  uint8_t    equipment_select=0;
  float      temporary_value;
  uint16_t   temporary_uint;
  time_t     now_ts;
  struct     timespec tspec;
  uint8_t    i;
  char       str_val[32];  
  
  
  while (1) {
	
    clock_gettime(CLOCK_REALTIME, &tspec);
    now_ts  = tspec.tv_sec;

    switch (equipment_select) {
      case EQ_MAINPOWER:
	      for (i=0;i<equipment_count[EQ_MAINPOWER];i++) {
		      /*
		        101	1	Mains_ComDisconnected	FALSE	Critical
		      */
		      if (!main_power_equipt[i].alarmEnable) 
					  continue;
          if((now_ts-*main_power_equipt[i].voltTimeStamp)>main_power_equipt[i].voltTimeOut){
			      if(main_power_equipt[i].alarmState[0]==Mains_ComDisconnected) 
					    continue;
			      //toggle state from connected to disconnected;
			      main_power_equipt[i].alarmState[0]=Mains_ComDisconnected;
			                
            memset(str_val,0,sizeof(str_val));
            insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_ComDisconnected, str_val);
			      printf("\r\n<<<MAIN_POWER%d::DISCONNECTED>>>\r\n",i);
			      continue;
		      }
		  
		      if (main_power_equipt[i].voltTimeStampOld != *main_power_equipt[i].voltTimeStamp){
		        main_power_equipt[i].voltTimeStampOld = *main_power_equipt[i].voltTimeStamp;
		        printf("\r\n<<<MAIN_POWER%d::CONNECTED>>>\r\n",i);
		        /*
		          100	1	Mains_ComConnected	FALSE	Cleared
		        */
		        if(main_power_equipt[i].alarmState[0]==Mains_ComDisconnected){
			        //toggle state from disconnected to connected;
			        main_power_equipt[i].alarmState[0]=Mains_ComConnected;
			                
              memset(str_val,0,sizeof(str_val));
              insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_ComConnected, str_val);
		        }
		  
		        /*
		          111	1	Mains_off	FALSE	Critical	Volt
		        */
		        if (*main_power_equipt[i].volt < 500) {
					    if(main_power_equipt[i].alarmState[1]!=Mains_off) {
						    main_power_equipt[i].alarmState[1]=Mains_off;
						    memset(str_val,0,sizeof(str_val));
						    //sprintf(str_val,"%d.%d", *main_power_equipt[i].volt/10, *main_power_equipt[i].volt%10);
						    sprintf(str_val,"%d", *main_power_equipt[i].volt/10);
						    insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_off, str_val);
					    }
					    continue;
		        }
		  
		        /*
		          110	1	Mains_On	FALSE	Cleared	Volt
		        */
		        else if(main_power_equipt[i].alarmState[1]!=Mains_On) {
					    main_power_equipt[i].alarmState[1]=Mains_On;
              memset(str_val,0,sizeof(str_val));
		          sprintf(str_val,"%d.%d", *main_power_equipt[i].volt/10, *main_power_equipt[i].volt%10);
              insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_On, str_val);
		        }
			
		        /*
		          122	1	Mains_voltage_sag	FALSE	Critical	Volt
		        */
				    temporary_value = *main_power_equipt[i].volt /10;
			
		        if (temporary_value < main_power_equipt[i].voltMin) {
			        if(main_power_equipt[i].alarmState[2]!=Mains_voltage_sag) {
			          main_power_equipt[i].alarmState[2]=Mains_voltage_sag;
			                
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *main_power_equipt[i].volt/10, *main_power_equipt[i].volt%10);
                insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_voltage_sag, str_val);
			        }
		        }
			
		        /*
		          121	1	Mains_over_voltage	FALSE	Critical	Volt
		        */
		        else if (temporary_value > main_power_equipt[i].voltMax) {
			        if(main_power_equipt[i].alarmState[2]!=Mains_over_voltage) {
			          main_power_equipt[i].alarmState[2]=Mains_over_voltage;
			                
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *main_power_equipt[i].volt/10, *main_power_equipt[i].volt%10);
                insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_over_voltage, str_val);
			        }
		        }
			
		        /*
		          120	1	Mains_Norm	FALSE	Cleared	Volt
		        */
		        else {
			        if(main_power_equipt[i].alarmState[2]!=Mains_Norm) {
			          main_power_equipt[i].alarmState[2]=Mains_Norm;
			                
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *main_power_equipt[i].volt/10, *main_power_equipt[i].volt%10);
                insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_Norm, str_val);
			        }
		        }
		
		      }
		  
          if((now_ts-*main_power_equipt[i].currentTimeStamp)>main_power_equipt[i].currentTimeOut){
			      continue;
		      }
		  
		      if (main_power_equipt[i].currentTimeStampOld == *main_power_equipt[i].currentTimeStamp) 
				    continue;
		  
		      main_power_equipt[i].currentTimeStampOld = *main_power_equipt[i].currentTimeStamp;
		  
		      temporary_value = *main_power_equipt[i].current/1000;
		  
		      /*    
		        131	1	Mains_over_current	FALSE	Critical	Current
		      */
		      if (temporary_value > main_power_equipt[i].currentMax) {
			      if(main_power_equipt[i].alarmState[3]!=Mains_over_current) {
			        main_power_equipt[i].alarmState[3]=Mains_over_current;
			                
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", *main_power_equipt[i].current/1000, (*main_power_equipt[i].current%1000)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_over_current, str_val);
			      }
		      }
			
		      /*
		        130	1	Mains_Norm_current	FALSE	Cleared	Current
		      */
		      else {
			      if(main_power_equipt[i].alarmState[3]!=Mains_Norm_current) {
			        main_power_equipt[i].alarmState[3]=Mains_Norm_current;
			              
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", *main_power_equipt[i].current/1000, (*main_power_equipt[i].current%1000)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_MAINPOWER+1, main_power_equipt[i].id, Mains_Norm_current, str_val);
			      }
		      }
		    }
        break;
		
      case EQ_AC:
        for (i=0;i<equipment_count[EQ_AC];i++) {
		      /*
		        212	2	AC Coil Disconnected	FALSE	Critical
		      */
		      if (!ac_equipt[i].alarmEnable) 
						continue;
          if((now_ts-*ac_equipt[i].envTempTimeStamp)>ac_equipt[i].envTempTimeOut){
			      if(ac_equipt[i].alarmState[1]==AcCoilDisconnected) 
						  continue;
					
			      //toggle state from connected to disconnected;
			      ac_equipt[i].alarmState[1]=AcCoilDisconnected;
			                
            memset(str_val,0,sizeof(str_val));
            insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcCoilDisconnected, str_val);
			      continue;
		      }
          				 
          if((now_ts-*ac_equipt[i].coilTempTimeStamp)>ac_equipt[i].coilTempTimeOut){
			      if(ac_equipt[i].alarmState[1]==AcCoilDisconnected) 
						  continue;
					 
			      //toggle state from connected to disconnected;
			      ac_equipt[i].alarmState[1]=AcCoilDisconnected;
			                
            memset(str_val,0,sizeof(str_val));
            insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcCoilDisconnected, str_val);
			      continue;
		      }
		  
		      if (ac_equipt[i].envTempTimeStampOld == *ac_equipt[i].envTempTimeStamp){
			      if (ac_equipt[i].coilTempTimeStampOld == *ac_equipt[i].coilTempTimeStamp){
			        continue;
            }
		      } 
		  
		      ac_equipt[i].envTempTimeStampOld = *ac_equipt[i].envTempTimeStamp;
		      ac_equipt[i].coilTempTimeStampOld = *ac_equipt[i].coilTempTimeStamp;
		  
          temporary_uint = abs(*ac_equipt[i].envTemp - *ac_equipt[i].coilTemp);
		      temporary_value = temporary_uint / 100;
		  
		      /*
		        211	2	AC Coil Hi-Temp	FALSE	Minor	( EnvTemp - CoilTemp )
		      */
		      if (temporary_value > ac_equipt[i].deltaTempMax) {
			      if(ac_equipt[i].alarmState[1]!=AcCoilHiTemp) {
			        ac_equipt[i].alarmState[1]=AcCoilHiTemp;
			                
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", temporary_uint/100, (temporary_uint%100)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcCoilHiTemp, str_val);
			      }
		      }
			
		      /*
		        210	2	AC Coil Norm-Temp	FALSE	Cleared	( EnvTemp - CoilTemp )
		      */
		      else {
			      if(ac_equipt[i].alarmState[1]!=AcCoilNormTemp) {
			        ac_equipt[i].alarmState[1]=AcCoilNormTemp;
			              
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", *ac_equipt[i].current/1000, (*ac_equipt[i].current%1000)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcCoilNormTemp, str_val);
			      }
		      }
		  
		      /*
		       222	2	AC Disconnected Current	FALSE	Critical
		      */
          if((now_ts-*ac_equipt[i].currentTimeStamp)>ac_equipt[i].currentTimeOut){
			      if(ac_equipt[i].alarmState[2]==AcDisconnectedCurrent) 
						  continue;
					 
			      //toggle state from connected to disconnected;
			      ac_equipt[i].alarmState[2]=AcDisconnectedCurrent;
			                
            memset(str_val,0,sizeof(str_val));
            insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcDisconnectedCurrent, str_val);
			      continue;
		      }
		  
		      if (ac_equipt[i].currentTimeStampOld == *ac_equipt[i].currentTimeStamp) 
					  continue;
		  
		      ac_equipt[i].currentTimeStampOld = *ac_equipt[i].currentTimeStamp;
		  
		      temporary_value = *ac_equipt[i].current/1000;
		  
		      /*
		        221	2	AC Hi-Current	FALSE	Minor	Current
		      */
		      if (temporary_value > ac_equipt[i].currentMax) {
			      if(ac_equipt[i].alarmState[2]!=AcHiCurrent) {
			        ac_equipt[i].alarmState[2]=AcHiCurrent;
			                
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", *ac_equipt[i].current/1000, (*ac_equipt[i].current%1000)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcHiCurrent, str_val);
			      }
		      }
			
		      /*
		        220	2	AC Norm-Current	FALSE	Cleared	Current
		      */
		      else {
			      if(ac_equipt[i].alarmState[2]!=AcNormCurrent) {
			        ac_equipt[i].alarmState[2]=AcNormCurrent;
			              
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", *ac_equipt[i].current/1000, (*ac_equipt[i].current%1000)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcNormCurrent, str_val);
			      }
		      }
		  
		  
		      /*
		        231	2	AC IR Disconnected	FALSE	Critical	
		      */
          if((now_ts-*ac_equipt[i].iRTimeStamp)>ac_equipt[i].iRTimeOut){
			      if(ac_equipt[i].alarmState[3]==AcIrDisconnected) 
						  continue;
					 
			      //toggle state from connected to disconnected;
			      ac_equipt[i].alarmState[3]=AcIrDisconnected;
			                
            memset(str_val,0,sizeof(str_val));
            insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcIrDisconnected, str_val);
			      continue;
		      }
		  
		      if (ac_equipt[i].iRTimeStampOld != *ac_equipt[i].iRTimeStamp){
		        ac_equipt[i].iRTimeStampOld = *ac_equipt[i].iRTimeStamp;
		  
		        /*
		          230	2	AC IR Connected	FALSE	Cleared
		        */
		        if(ac_equipt[i].alarmState[3]==AcIrDisconnected){
			      //toggle state from disconnected to connected;
			        ac_equipt[i].alarmState[3]=AcIrConnected;
			                
              memset(str_val,0,sizeof(str_val));
              insert_new_alarm_to_buffer(now_ts, EQ_AC+1, ac_equipt[i].id, AcIrConnected, str_val);
		        }
		      }
		  
		    }
        break;
		
		
      case EQ_CHILLER:
        // for (i=0;i<equipment_count[EQ_CHILLER];i++) {
			
		    // }
        break;
		
      case EQ_DOOR:
        for (i=0;i<equipment_count[EQ_DOOR];i++) {
		      /*
		        403	4	Door ComDisconnected	FALSE	Critical	
		      */
		      if (!door_equipt[i].alarmEnable) continue;
          if((now_ts-*door_equipt[i].doorOpenTimeStamp)>door_equipt[i].doorOpenTimeOut){
			      if(door_equipt[i].alarmState[0]==DoorComDisconnected) continue;
			      //toggle state from connected to disconnected;
			      door_equipt[i].alarmState[0]=DoorComDisconnected;
			                
            memset(str_val,0,sizeof(str_val));
            insert_new_alarm_to_buffer(now_ts, EQ_DOOR+1, door_equipt[i].id, DoorComDisconnected, str_val);
			      continue;
		      }
		  
		      if (door_equipt[i].doorOpenTimeStampOld != *door_equipt[i].doorOpenTimeStamp){
		        door_equipt[i].doorOpenTimeStampOld = *door_equipt[i].doorOpenTimeStamp;
		  
		        /*
		          401	4	Door Open > x Min	FALSE	Warning	DoorOpen, DoorOpenTime
		        */
		        if(*door_equipt[i].doorOpen == door_equipt[i].doorOpenLogic){
			        if(*door_equipt[i].doorOpenTime < door_equipt[i].doorOpenTimeMax) continue;
			        if(door_equipt[i].alarmState[0]==DoorOpenXmin) continue;
			        door_equipt[i].alarmState[0]=DoorOpenXmin;
			                
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d,%d", *door_equipt[i].doorOpen, *door_equipt[i].doorOpenTime);
              insert_new_alarm_to_buffer(now_ts, EQ_DOOR+1, door_equipt[i].id, DoorOpenXmin, str_val);
		        }
			
		        /*
		          400	4	Door Closed	FALSE	Cleared	DoorOpen
		        */
			      else {
			        if(door_equipt[i].alarmState[0]==DoorClosed) continue;
			        door_equipt[i].alarmState[0]=DoorClosed;
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d", *door_equipt[i].doorOpen);
              insert_new_alarm_to_buffer(now_ts, EQ_DOOR+1, door_equipt[i].id, DoorClosed, str_val);
			      }
		      }
		    }	  
        break;
		
      case EQ_ROOM:
        for (i=0;i<equipment_count[EQ_ROOM];i++) {
		      /*
		        503	5	Room Disconnect Temp	FALSE	Critical	
		      */
		      if (!room_equipt[i].alarmEnable) continue;
          if((now_ts-*room_equipt[i].tempTimeStamp)>room_equipt[i].tempTimeOut){
			      if(room_equipt[i].alarmState[0]!=RoomDisconnectTemp){
			        //toggle state from connected to disconnected;
			        room_equipt[i].alarmState[0]=RoomDisconnectTemp;
			                
              memset(str_val,0,sizeof(str_val));
              insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomDisconnectTemp, str_val);
			      }
		      }
		  
		      else if (room_equipt[i].tempTimeStampOld != *room_equipt[i].tempTimeStamp){
		        room_equipt[i].tempTimeStampOld = *room_equipt[i].tempTimeStamp;
			
		        temporary_value = *room_equipt[i].temp / 100;
		  
		        /*
		          502	5	Room Lo-Temp	FALSE	Minor	Temp
		        */
		        if(temporary_value < room_equipt[i].tempMin){
			        if(room_equipt[i].alarmState[0]!=RoomLoTemp){
				        room_equipt[i].alarmState[0]=RoomLoTemp;  
				
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *room_equipt[i].temp/100, (*room_equipt[i].temp%100)/10);
                insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomLoTemp, str_val);
			        }
		        }

		        /*
		          501	5	Room Hi-Temp	FALSE	Minor	Temp
		        */
		        else if(temporary_value > room_equipt[i].tempMax){
			        if(room_equipt[i].alarmState[0]!=RoomHiTemp){
				        room_equipt[i].alarmState[0]=RoomHiTemp;  
				
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *room_equipt[i].temp/100, (*room_equipt[i].temp%100)/10);
                insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomHiTemp, str_val);
			        }
		        }
			
		        /*
		          500	5	Room_Norm_temp	FALSE	Cleared	Temp
		        */
			      else {
			        if(room_equipt[i].alarmState[0]==RoomNormTemp) continue;
			          room_equipt[i].alarmState[0]=RoomNormTemp;
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *room_equipt[i].temp/100, (*room_equipt[i].temp%100)/10);
                insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomNormTemp, str_val);
			      }
		      }
		  

		  
		      /*
		        512	5	Room Disconnected Humid	FALSE	Critical	
		      */
          if((now_ts-*room_equipt[i].humidityTimeStamp)>room_equipt[i].humidityTimeOut){
			      if(room_equipt[i].alarmState[1]!=RoomDisconnectedHumid){
			        //toggle state from connected to disconnected;
			        room_equipt[i].alarmState[1]=RoomDisconnectedHumid;
			                
              memset(str_val,0,sizeof(str_val));
              insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomDisconnectedHumid, str_val);
			      }
		      }
		  
		      else if (room_equipt[i].humidityTimeStampOld != *room_equipt[i].humidityTimeStamp){
		        room_equipt[i].humidityTimeStampOld = *room_equipt[i].humidityTimeStamp;
			
		        temporary_value = *room_equipt[i].humidity / 100;
		  
		        /*
		          511	5	Room Hi-Humid	FALSE	Minor	Humidity
		        */
		        if(temporary_value > room_equipt[i].humidityMax){
			        if(room_equipt[i].alarmState[1]!=RoomHiHumid){
				        room_equipt[i].alarmState[1]=RoomHiHumid;  
				
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d.%d", *room_equipt[i].humidity/100, (*room_equipt[i].humidity%100)/10);
                insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomHiHumid, str_val);
			        }
		        }

		        /*
		          510	5	Room Norm-Humid	FALSE	Cleared	Humidity
		        */
			      else {
			        if(room_equipt[i].alarmState[1]==RoomNormHumid) continue;
			        room_equipt[i].alarmState[1]=RoomNormHumid;
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d.%d", *room_equipt[i].humidity/100, (*room_equipt[i].humidity%100)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, room_equipt[i].id, RoomNormHumid, str_val);
			      }
		      }
		    }
        break;
		
      case EQ_PUMP:
        // for (i=0;i<equipment_count[EQ_PUMP];i++) {
			
		    // }
        break;
		
      case EQ_LAMP:
        for (i=0;i<equipment_count[EQ_LAMP];i++) {
		      /*
		        712	7	Lamp_ContDisconnected	FALSE	Major		
		      */
		      if (!lamp_equipt[i].alarmEnable) continue;
          if((now_ts-*lamp_equipt[i].lampOnTimeStamp)>lamp_equipt[i].lampOnTimeOut){
			      if(lamp_equipt[i].alarmState[1]!=LampContDisconnected){
			        //toggle state from connected to disconnected;
			        lamp_equipt[i].alarmState[1]=LampContDisconnected;
			                
              memset(str_val,0,sizeof(str_val));
              insert_new_alarm_to_buffer(now_ts, EQ_LAMP+1, lamp_equipt[i].id, LampContDisconnected, str_val);
				      continue;
			      }
		      }
		  
		      if (lamp_equipt[i].lampOnTimeStampOld != *lamp_equipt[i].lampOnTimeStamp){
		        lamp_equipt[i].lampOnTimeStampOld = *lamp_equipt[i].lampOnTimeStamp;
			
		        /*
		          711	7	Lamp_On	FALSE	Warning	LampOn
		        */
		        if(*lamp_equipt[i].lampOn == lamp_equipt[i].lampOnLogic){
			        if(lamp_equipt[i].alarmState[1]!=LampOn){
				        lamp_equipt[i].alarmState[1]=LampOn;  
				
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d", *lamp_equipt[i].lampOn);
                insert_new_alarm_to_buffer(now_ts, EQ_LAMP+1, lamp_equipt[i].id, LampOn, str_val);
			        }
		        }

		        /*
		          710	7	Lamp_Off	FALSE	Cleared	LampOn
		        */
			      else {
			        if(lamp_equipt[i].alarmState[1]==RoomNormTemp) continue;
			        lamp_equipt[i].alarmState[1]=RoomNormTemp;
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d", *lamp_equipt[i].lampOn);
              insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, lamp_equipt[i].id, RoomNormTemp, str_val);
			      }
		      }
		  
		      /*
		        703	7	Lamp_ComDisconnected	FALSE	Critical		
		      */
          if((now_ts-*lamp_equipt[i].currentTimeStamp)>lamp_equipt[i].currentTimeOut){
			      if(lamp_equipt[i].alarmState[0]!=LampComDisconnected){
			        //toggle state from connected to disconnected;
			        lamp_equipt[i].alarmState[0]=LampComDisconnected;
			                
              memset(str_val,0,sizeof(str_val));
              insert_new_alarm_to_buffer(now_ts, EQ_LAMP+1, lamp_equipt[i].id, LampComDisconnected, str_val);
				      continue;
			      }
		      }
		  
		      if (lamp_equipt[i].currentTimeStampOld != *lamp_equipt[i].currentTimeStamp){
		        lamp_equipt[i].currentTimeStampOld = *lamp_equipt[i].currentTimeStamp;
			
			      temporary_value = *lamp_equipt[i].current/1000;
			
		        /*
		          702	7	Lamp_Off_Abnormal	FALSE	Minor	LampOn, Current
		        */
		        if((*lamp_equipt[i].lampOn != lamp_equipt[i].lampOnLogic)&&(temporary_value>lamp_equipt[i].currentMin)){
			        if(lamp_equipt[i].alarmState[0]!=LampOffAbnormal){
				        lamp_equipt[i].alarmState[0]=LampOffAbnormal;  
				
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d,%d.%d", *lamp_equipt[i].lampOn,*lamp_equipt[i].current/1000,(*lamp_equipt[i].current%1000)/10);
                insert_new_alarm_to_buffer(now_ts, EQ_LAMP+1, lamp_equipt[i].id, LampOffAbnormal, str_val);
			        }
		        }
			
		        /*
		          701	7	Lamp_On_Abnormal	FALSE	Minor	LampOn, Current
		        */
		        else if((*lamp_equipt[i].lampOn == lamp_equipt[i].lampOnLogic)&&(temporary_value>lamp_equipt[i].currentMax)){
			        if(lamp_equipt[i].alarmState[0]!=LampOnAbnormal){
				        lamp_equipt[i].alarmState[0]=LampOnAbnormal;  
				
                memset(str_val,0,sizeof(str_val));
			          sprintf(str_val,"%d,%d.%d", *lamp_equipt[i].lampOn,*lamp_equipt[i].current/1000,(*lamp_equipt[i].current%1000)/10);
                insert_new_alarm_to_buffer(now_ts, EQ_LAMP+1, lamp_equipt[i].id, LampOnAbnormal, str_val);
			        }
		        }

		        /*
		          700	7	Lamp_normal	FALSE	Cleared	LampOn, Current
		        */
			      else {
			        if(lamp_equipt[i].alarmState[0]==LampNormal) continue;
			        lamp_equipt[i].alarmState[0]=LampNormal;
              memset(str_val,0,sizeof(str_val));
			        sprintf(str_val,"%d,%d.%d", *lamp_equipt[i].lampOn,*lamp_equipt[i].current/1000,(*lamp_equipt[i].current%1000)/10);
              insert_new_alarm_to_buffer(now_ts, EQ_ROOM+1, lamp_equipt[i].id, LampNormal, str_val);
			      }
		      }
		    }
        break;
		
	    case EQ_MAX:
	      equipment_select = EQ_MAINPOWER-1;
		    break;
	  }
	
	  if (++equipment_select>=EQ_MAX) 
      equipment_select = EQ_MAINPOWER;
  
    sleep(1);
  }
  
}

void clear_equipment_alarm (void) {
  uint8_t i, equipt;
  struct timespec ts;
					
  clock_gettime(CLOCK_REALTIME, &ts);
    
  for(equipt=0;equipt<EQ_MAX;equipt++) {
	  switch (equipt) {
	    case EQ_MAINPOWER:
	      for (i=0;i<equipment_count[EQ_MAINPOWER];i++) {
	        main_power_equipt[i].id = 0;
		      memset(main_power_equipt[i].label,0, sizeof(main_power_equipt[i].label));
		      memset(main_power_equipt[i].position,0, sizeof(main_power_equipt[i].position));
		      main_power_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&main_power_equipt[i].alarmLastSend,0, sizeof(main_power_equipt[i].alarmLastSend));
		      memset((uint8_t*)&main_power_equipt[i].alarmState,0, sizeof(main_power_equipt[i].alarmState));
		      memset(main_power_equipt[i].voltId,0, sizeof(main_power_equipt[i].voltId));
		      main_power_equipt[i].voltMin = 0;
		      main_power_equipt[i].voltMax = 9999;
          main_power_equipt[i].voltTimeOut = 0;
          main_power_equipt[i].voltTimeStampOld = ts.tv_sec;
		      memset(main_power_equipt[i].currentId,0, sizeof(main_power_equipt[i].currentId));
		      main_power_equipt[i].currentMin = 0;
		      main_power_equipt[i].currentMax = 9999;
		      main_power_equipt[i].currentTimeOut=0;
		      main_power_equipt[i].currentTimeStampOld = ts.tv_sec;
	      }
	      break;
	 
	    case EQ_AC:
	      for (i=0;i<equipment_count[EQ_AC];i++) {
		      ac_equipt[i].id = 0;
		      memset(ac_equipt[i].label,0, sizeof(ac_equipt[i].label));
		      memset(ac_equipt[i].position,0, sizeof(ac_equipt[i].position));
		      ac_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&ac_equipt[i].alarmLastSend,0, sizeof(ac_equipt[i].alarmLastSend));
		      memset((uint8_t*)&ac_equipt[i].alarmState,0, sizeof(ac_equipt[i].alarmState));
		      memset(ac_equipt[i].brandnModel,0, sizeof(ac_equipt[i].brandnModel));
		      memset(ac_equipt[i].coilTempId,0, sizeof(ac_equipt[i].coilTempId));
		      ac_equipt[i].coilTempMin = 0;
		      ac_equipt[i].coilTempMax = 9999;
          ac_equipt[i].coilTempTimeOut = 0;
          ac_equipt[i].coilTempTimeStampOld = ts.tv_sec;
		      memset(ac_equipt[i].envTempId,0, sizeof(ac_equipt[i].envTempId));
		      ac_equipt[i].envTempMin = 0;
		      ac_equipt[i].envlTempMax = 9999;
		      ac_equipt[i].envTempTimeOut=0;
		      ac_equipt[i].envTempTimeStampOld = ts.tv_sec;
		      ac_equipt[i].deltaTempMin = 0;
		      ac_equipt[i].deltaTempMax = 9999;
		      memset(ac_equipt[i].currentId,0, sizeof(ac_equipt[i].currentId));
		      ac_equipt[i].currentMin = 0;
		      ac_equipt[i].currentMax = 9999;
		      ac_equipt[i].currentTimeOut=0;
		      ac_equipt[i].currentTimeStampOld = ts.tv_sec;
		      memset(ac_equipt[i].iRId,0, sizeof(ac_equipt[i].iRId));
		      ac_equipt[i].iRTimeOut=0;
		      ac_equipt[i].iRTimeStampOld = ts.tv_sec;
	      }
	      break;
	 
	    case EQ_CHILLER:
	      for (i=0;i<equipment_count[EQ_CHILLER];i++) {
	        chiller_equipt[i].id = 0;
		      memset(chiller_equipt[i].label,0, sizeof(chiller_equipt[i].label));
		      memset(chiller_equipt[i].position,0, sizeof(chiller_equipt[i].position));
		      chiller_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&chiller_equipt[i].alarmLastSend,0, sizeof(chiller_equipt[i].alarmLastSend));
		      memset((uint8_t*)&chiller_equipt[i].alarmState,0, sizeof(chiller_equipt[i].alarmState));
		      memset(chiller_equipt[i].tempId,0, sizeof(chiller_equipt[i].tempId));
		      chiller_equipt[i].tempMin = 0;
		      chiller_equipt[i].tempMax = 9999;
          chiller_equipt[i].tempTimeOut = 0;
          chiller_equipt[i].tempTimeStampOld = ts.tv_sec;
		      memset(chiller_equipt[i].currentId,0, sizeof(chiller_equipt[i].currentId));
		      chiller_equipt[i].currentMin = 0;
		      chiller_equipt[i].currentMax = 9999;
		      chiller_equipt[i].currentTimeOut=0;
		      chiller_equipt[i].currentTimeStampOld = ts.tv_sec;
	      }
	      break;
		
	    case EQ_DOOR:
	      for (i=0;i<equipment_count[EQ_DOOR];i++) {
	        door_equipt[i].id = 0;
		      memset(door_equipt[i].label,0, sizeof(door_equipt[i].label));
		      memset(door_equipt[i].position,0, sizeof(door_equipt[i].position));
		      door_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&door_equipt[i].alarmLastSend,0, sizeof(door_equipt[i].alarmLastSend));
		      memset((uint8_t*)&door_equipt[i].alarmState,0, sizeof(door_equipt[i].alarmState));
		      memset(door_equipt[i].doorOpenId,0, sizeof(door_equipt[i].doorOpenId));
		      door_equipt[i].doorOpenLogic = 1;
		      door_equipt[i].doorOpenTimeMax = 65535;
		      door_equipt[i].doorOpenTimeOut=0;
		      door_equipt[i].doorOpenTimeStampOld = ts.tv_sec;
	      }
	      break;
		
	    case EQ_ROOM:
	      for (i=0;i<equipment_count[EQ_ROOM];i++) {
	        room_equipt[i].id = 0;
		      memset(room_equipt[i].label,0, sizeof(room_equipt[i].label));
		      memset(room_equipt[i].position,0, sizeof(room_equipt[i].position));
		      room_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&room_equipt[i].alarmLastSend,0, sizeof(room_equipt[i].alarmLastSend));
		      memset((uint8_t*)&room_equipt[i].alarmState,0, sizeof(room_equipt[i].alarmState));
		      memset(room_equipt[i].tempId,0, sizeof(room_equipt[i].tempId));
		      room_equipt[i].tempMin = 0;
		      room_equipt[i].tempMax = 9999;
          room_equipt[i].tempTimeOut = 0;
          room_equipt[i].tempTimeStampOld = ts.tv_sec;
		      room_equipt[i].humidityMin = 0;
		      room_equipt[i].humidityMax = 9999;
		      room_equipt[i].humidityTimeOut=0;
		      room_equipt[i].humidityTimeStampOld = ts.tv_sec;
	      }
	      break;
		
	    case EQ_PUMP:
	      for (i=0;i<equipment_count[EQ_PUMP];i++) {
	        pump_equipt[i].id = 0;
		      memset(pump_equipt[i].label,0, sizeof(pump_equipt[i].label));
		      memset(pump_equipt[i].position,0, sizeof(pump_equipt[i].position));
		      pump_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&pump_equipt[i].alarmLastSend,0, sizeof(pump_equipt[i].alarmLastSend));
		      memset((uint8_t*)&pump_equipt[i].alarmState,0, sizeof(pump_equipt[i].alarmState));
		      memset(pump_equipt[i].currentId,0, sizeof(pump_equipt[i].currentId));
		      pump_equipt[i].currentMin = 0;
		      pump_equipt[i].currentMax = 9999;
          pump_equipt[i].currentTimeOut = 0;
          pump_equipt[i].currentTimeStampOld = ts.tv_sec;
	      }
	      break;
		
	    case EQ_LAMP:
	      for (i=0;i<equipment_count[EQ_LAMP];i++) {
	        lamp_equipt[i].id = 0;
		      memset(lamp_equipt[i].label,0, sizeof(lamp_equipt[i].label));
		      memset(lamp_equipt[i].position,0, sizeof(lamp_equipt[i].position));
		      lamp_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&lamp_equipt[i].alarmLastSend,0, sizeof(lamp_equipt[i].alarmLastSend));
		      memset((uint8_t*)&lamp_equipt[i].alarmState,0, sizeof(lamp_equipt[i].alarmState));
		      memset(lamp_equipt[i].currentId,0, sizeof(lamp_equipt[i].currentId));
		      lamp_equipt[i].currentMin = 0;
		      lamp_equipt[i].currentMax = 9999;
          lamp_equipt[i].currentTimeOut = 0;
          lamp_equipt[i].currentTimeStampOld = ts.tv_sec;
		      memset(lamp_equipt[i].lampOnId,0, sizeof(lamp_equipt[i].lampOnId));
		      lamp_equipt[i].lampOnLogic = 1;
		      lamp_equipt[i].lampOnTimeOut=0;
		      lamp_equipt[i].lampOnTimeStampOld = ts.tv_sec;
	      }
	      break;
				
		  default:
			  break;
	  }
    
  }
  printf("ALL Equipment Cleared!\r\n");
}

void get_main_power_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{ 
  char select[100];     
  MYSQL *con = mysql_init(NULL);
  MYSQL_ROW row;
  uint8_t phase, channel;
  struct timespec ts;
					
  int a=0; 

  if (con == NULL) {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, dbname, 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    
	
  sprintf(select,"select * from %s", nm_table); 
  if (mysql_query(con,select)) { 
      finish_with_error(con);
  }


  MYSQL_RES *result = mysql_store_result(con);  
  if (result == NULL) {
    finish_with_error(con);
  }

  clock_gettime(CLOCK_REALTIME, &ts);
/* 
int num_fields = mysql_num_fields(result);  
while ((row = mysql_fetch_row(result)))
{
   unsigned long *lengths;
   lengths = mysql_fetch_lengths(result);
   for(a = 0; a < num_fields; a++)
   {
       printf("[%.*s] ", (int) lengths[a],
              row[a] ? row[a] : "NULL");
   }
   printf("\n");
}

  phases[0].vrms = 111;
  phases[1].vrms = 333;
  phases[2].vrms = 555;
  phases[0].irmsa = 666;
  phases[1].irmsa = 777;
  phases[2].irmsa = 888;
   */
   
  for (a=0;a<(sizeof(phase_ts)/sizeof(time_t));a++) {
	  phase_ts[a] = ts.tv_sec;
  }
  
  a = 0;
  while ((row = mysql_fetch_row(result)))
  {	

	  main_power_equipt[a].id = atoi(row[0]?row[0]:"0");
	  snprintf(main_power_equipt[a].label, sizeof main_power_equipt[a].label, "%s", row[1]);
	  phase   = atoi(row[2]?row[2]:"0");
	  channel = atoi(row[3]?row[3]:"0");
	
	  main_power_equipt[a].volt = &phases[phase].vrms;
	  main_power_equipt[a].current = &phases[phase].irmsa+channel;
	  main_power_equipt[a].voltTimeStamp = &phase_ts[0];
	  main_power_equipt[a].currentTimeStamp = &phase_ts[1+channel];
	
	  main_power_equipt[a].voltMin = atof(row[4]?row[4]:"180");
	  main_power_equipt[a].voltMax = atof(row[5]?row[5]:"240");
	  main_power_equipt[a].currentMin = atof(row[6]?row[6]:"1");
    main_power_equipt[a].currentMax = atof(row[7]?row[7]:"30");

    main_power_equipt[a].voltTimeOut = atoi(row[8]?row[8]:"0");
    main_power_equipt[a].currentTimeOut = atoi(row[9]?row[9]:"0");
	 
	  snprintf(main_power_equipt[a].voltId, sizeof main_power_equipt[a].voltId, "P%d-C%d", phase, channel);
	  snprintf(main_power_equipt[a].currentId, sizeof main_power_equipt[a].currentId, "P%d-C%d", phase, channel);
		  
    main_power_equipt[a].voltTimeStampOld = ts.tv_sec;
    main_power_equipt[a].currentTimeStampOld = ts.tv_sec;
	  main_power_equipt[a].alarmState[0]=Mains_ComConnected;
	  main_power_equipt[a].alarmState[1]=Mains_On;
	  main_power_equipt[a].alarmState[2]=Mains_Norm;
	  main_power_equipt[a].alarmState[3]=Mains_Norm_current;
	  main_power_equipt[a].alarmState[4]=Norm_Gnd_Voltage;
    main_power_equipt[a].alarmEnable = 1;	
		
    // printf("PWR %d: %d, %s, %d, %d, %f [%d]%d\r\n",a, main_power_equipt[a].id, main_power_equipt[a].label, phase, channel, main_power_equipt[a].voltMin, *main_power_equipt[a].volt,*main_power_equipt[a].current);
	  a++;
  }
 
  mysql_close(con);
}

uint8_t get_num_of_phase(char * phase){
	if (strcmp(phase,"S")==0) return 1;
	if (strcmp(phase,"T")==0) return 2;
	return 0;
}

void get_ac_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{ 
  char select[256];     
  MYSQL *con = mysql_init(NULL);
  MYSQL_ROW row;
  uint8_t phase, channel;
  struct timespec ts;
  uint16_t temp_id;
  uint16_t ir_id;
  int a=0; 

  if (con == NULL) {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, dbname, 0, NULL, 0) == NULL) {
    finish_with_error(con);
  }    
	
  sprintf(select,"select * from %s", nm_table); 
  if (mysql_query(con,select)) { 
    finish_with_error(con);
  }


  MYSQL_RES *result = mysql_store_result(con);  
  if (result == NULL) {
    finish_with_error(con);
  }

  clock_gettime(CLOCK_REALTIME, &ts);
  
   
  for (a=0;a<TH_NODES_MAX;a++) {
	  th_nodes[a].ts = ts.tv_sec;
  }
  
  for (a=0;a<(sizeof(ir_config)/sizeof(IR_CONFIG));a++) {
	  ir_config[a].ts = ts.tv_sec;
  }
  
  a = 0;
  while ((row = mysql_fetch_row(result))) {
		
	  ac_equipt[a].id = atoi(row[0]?row[0]:"0");
	  snprintf(ac_equipt[a].label, sizeof ac_equipt[a].label, "%s", row[1]);
	
	  phase   = get_num_of_phase(row[2]?row[2]:"0");
	  channel = atoi(row[7]?row[7]:"1");
	  temp_id = atoi(row[6]?row[6]:"1");
	  ir_id   = atoi(row[8]?row[8]:"1");
	
	  if(ir_id>=(sizeof(ir_config)/sizeof(IR_CONFIG))) ir_id = 0;
	
	  ac_equipt[a].coilTemp = &th_nodes[temp_id].median_t1;
	  ac_equipt[a].envTemp = &th_nodes[temp_id].th_set;
	  ac_equipt[a].current = &phases[phase].irmsa+channel;
	  ac_equipt[a].coilTempTimeStamp = &th_nodes[temp_id].ts;
	  ac_equipt[a].envTempTimeStamp = &th_nodes[temp_id].ts;
	  ac_equipt[a].currentTimeStamp = &phase_ts[1+channel];
	  ac_equipt[a].iRTimeStamp = &ir_config[ir_id].ts;
	
	  ac_equipt[a].deltaTempMin = atof(row[19]?row[19]:"1");
	  ac_equipt[a].deltaTempMax = atof(row[20]?row[20]:"4");
	  ac_equipt[a].currentMin = atof(row[21]?row[21]:"1");
    ac_equipt[a].currentMax = atof(row[22]?row[22]:"3");

    ac_equipt[a].coilTempTimeOut = atoi(row[23]?row[23]:"600");
	  ac_equipt[a].envTempTimeOut = atoi(row[23]?row[23]:"600");
    ac_equipt[a].currentTimeOut = atoi(row[24]?row[24]:"300");
	  ac_equipt[a].iRTimeOut      = atoi(row[24]?row[24]:"300");
	 
	  snprintf(ac_equipt[a].coilTempId, sizeof ac_equipt[a].coilTempId, "T%d", temp_id);
	  snprintf(ac_equipt[a].envTempId, sizeof ac_equipt[a].envTempId, "T%d", temp_id);
	  snprintf(ac_equipt[a].currentId, sizeof ac_equipt[a].currentId, "P%d-C%d", phase, channel);
	  snprintf(ac_equipt[a].iRId, sizeof ac_equipt[a].iRId, "Ir%d", ir_id);
		  
    ac_equipt[a].coilTempTimeStampOld = ts.tv_sec;
    ac_equipt[a].envTempTimeStampOld = ts.tv_sec;
	  ac_equipt[a].currentTimeStampOld = ts.tv_sec;
	  ac_equipt[a].iRTimeStampOld      = ts.tv_sec;
	
	  ac_equipt[a].alarmState[0]=AcNormal;
	  ac_equipt[a].alarmState[1]=AcCoilNormTemp;
	  ac_equipt[a].alarmState[2]=AcNormCurrent;
	  ac_equipt[a].alarmState[3]=AcIrConnected;
    ac_equipt[a].alarmEnable = 1;	

	  a++;
  }
   mysql_close(con);
}

void load_equipment_config (void) {
  uint8_t i, equipt;
  struct timespec ts;
					
  clock_gettime(CLOCK_REALTIME, &ts);
    
  for(equipt=0;equipt<EQ_MAX;equipt++) {
	  switch (equipt) {
	    case EQ_MAINPOWER:
	      for (i=0;i<equipment_count[EQ_MAINPOWER];i++) {
	        main_power_equipt[i].id = 0;
		      memset(main_power_equipt[i].label,0, sizeof(main_power_equipt[i].label));
		      memset(main_power_equipt[i].position,0, sizeof(main_power_equipt[i].position));
		      main_power_equipt[i].alarmEnable = 0;
		      memset((uint8_t*)&main_power_equipt[i].alarmLastSend,0, sizeof(main_power_equipt[i].alarmLastSend));
		      memset((uint8_t*)&main_power_equipt[i].alarmState,0, sizeof(main_power_equipt[i].alarmState));
		      memset(main_power_equipt[i].voltId,0, sizeof(main_power_equipt[i].voltId));
		      main_power_equipt[i].voltMin = 0;
		      main_power_equipt[i].voltMax = 9999;
          main_power_equipt[i].voltTimeOut = 0;
          main_power_equipt[i].voltTimeStampOld = ts.tv_sec;
		      memset(main_power_equipt[i].currentId,0, sizeof(main_power_equipt[i].currentId));
		      main_power_equipt[i].currentMin = 0;
		      main_power_equipt[i].currentMax = 9999;
		      main_power_equipt[i].currentTimeOut=0;
		      main_power_equipt[i].currentTimeStampOld = ts.tv_sec;
	      }
	      break;
    
    }
    printf("ALL Equipment Initialized!\r\n");
  }
}

void get_door_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{ 
  char select[100];     
  MYSQL *con = mysql_init(NULL);
  MYSQL_ROW row;
  uint8_t io_id;
  struct timespec ts;
	int channel;
					
  int a=0; 

  if (con == NULL) {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, dbname, 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    
	
  sprintf(select,"select id, door_name, io_id, i_channel, open_logic, max_threshold, doorTimeOut from %s", nm_table); 
  if (mysql_query(con,select)) { 
      finish_with_error(con);
  }


  MYSQL_RES *result = mysql_store_result(con);  
  if (result == NULL) {
    finish_with_error(con);
  }

  clock_gettime(CLOCK_REALTIME, &ts);
/* 
int num_fields = mysql_num_fields(result);  
while ((row = mysql_fetch_row(result)))
{
   unsigned long *lengths;
   lengths = mysql_fetch_lengths(result);
   for(a = 0; a < num_fields; a++)
   {
       printf("[%.*s] ", (int) lengths[a],
              row[a] ? row[a] : "NULL");
   }
   printf("\n");
}

  phases[0].vrms = 111;
  phases[1].vrms = 333;
  phases[2].vrms = 555;
  phases[0].irmsa = 666;
  phases[1].irmsa = 777;
  phases[2].irmsa = 888;
   */
   
  for (a=0;a<(sizeof(io_config)/sizeof(IO_CONFIG));a++) {
	  io_config[a].ts = ts.tv_sec;
  }
  
  a = 0;
  while ((row = mysql_fetch_row(result)))
  {	

	  door_equipt[a].id = atoi(row[0]?row[0]:"0");
	  snprintf(door_equipt[a].label, sizeof door_equipt[a].label, "%s", row[1]);
	  io_id   = atoi(row[2]?row[2]:"0");
	  channel = atoi(row[3]?row[3]:"0");
	
	  door_equipt[a].doorOpen = &io_config[io_id].input[channel];
		door_equipt[a].doorOpenTime = &io_config[io_id].input_timeCount[channel]; 
	  door_equipt[a].doorOpenTimeStamp = &io_config[io_id].ts;
	
	  door_equipt[a].doorOpenLogic = atoi(row[4]?row[4]:"1");
	  door_equipt[a].doorOpenTimeMax = atof(row[5]?row[5]:"180");

    door_equipt[a].doorOpenTimeOut = atoi(row[6]?row[6]:"300");
	 
    door_equipt[a].doorOpenTimeStampOld = ts.tv_sec;
	  door_equipt[a].alarmState[0]=DoorClosed;
    door_equipt[a].alarmEnable = 1;
		
    // printf("PWR %d: %d, %s, %d, %d, %f [%d]%d\r\n",a, main_power_equipt[a].id, main_power_equipt[a].label, phase, channel, main_power_equipt[a].voltMin, *main_power_equipt[a].volt,*main_power_equipt[a].current);
	  a++;
  }
 
  mysql_close(con);
}

void * alarm_sending(void * arg) {
	uint16_t i;
	
//	printf("TEST ALARM LIB\r\n");
	while (1){
		if (alarm_head_ptr == alarm_tail_ptr) {
			sleep(1);
	    continue;
	  }
	  
	  i = alarm_tail_ptr;
		if (res_equipment_alarm(location_alarm, alarm_buffer[i], gateway_ID)==0) {
			i = (i+1) & ALARM_BUFFER_MASK;
      alarm_tail_ptr = i;
	  }
	  sleep(1);
	  
	}
}

