#ifndef __EQUIPMENT_ALATM_H__
#define __EQUIPMENT_ALATM_H__

enum EQUIPMENT_CODE_E{
  EQ_MAINPOWER = 0,
  EQ_AC,
  EQ_CHILLER,
  EQ_DOOR,
  EQ_ROOM,
  EQ_PUMP,
  EQ_LAMP,
  
  EQ_MAX
};

enum ALARM_CODE_E{
  Mains_ComConnected = 100,	
  Mains_ComDisconnected = 101,
  Mains_On = 110,
  Mains_off = 111,
  Mains_Norm = 120,
  Mains_over_voltage = 121,
  Mains_voltage_sag = 122,
  Mains_Norm_current = 130,
  Mains_over_current = 131,
  Norm_Gnd_Voltage = 140,
  High_Gnd_Voltage = 141,
  
  AcNormal = 200,
  AcAbnormal = 201,
  AcCoilNormTemp = 210,
  AcCoilHiTemp = 211,
  AcCoilDisconnected = 212,
  AcNormCurrent = 220,
  AcHiCurrent = 221,
  AcDisconnectedCurrent = 222,
  AcIrConnected = 230,	
  AcIrDisconnected = 231,

  ChillerNormTemp = 300,
  ChillerHiTemp = 301,
  ChillerDisconnectedTemp = 302,
  ChillerNormCurrent = 310,
  ChillerHiCurrent = 311,
  ChillerDisconnectedCurrent = 312,
  ChillerNormal = 320,
  ChillerAbnormal = 321,	

  DoorClosed = 400,
  DoorOpenXmin = 401,
  DoorVandal = 402,
  DoorComDisconnected = 403,	

  RoomNormTemp = 500,
  RoomHiTemp = 501,
  RoomLoTemp = 502,
  RoomDisconnectTemp = 503,
  RoomNormHumid = 510,
  RoomHiHumid = 511,
  RoomDisconnectedHumid = 512,
  
  PumpNormal = 600,
  PumpAbnormal = 601,
  PumpSmallLeak = 602,
  PumpComDisconnected = 603,
  
  LampNormal = 700,
  LampOnAbnormal = 701,
  LampOffAbnormal = 702,
  LampComDisconnected = 703,
  LampOff = 710,
  LampOn = 711,
  LampContDisconnected = 712	

};


typedef struct {
   uint16_t id;
   char     label[16];
   char     position[32];
   char     alarmEnable;
   uint16_t alarmLastSend[5];
   uint16_t alarmState[5];
   char     voltId[16];
   int32_t  * volt;
   float    voltMin;
   float    voltMax;
   uint16_t voltTimeOut;
   time_t   * voltTimeStamp;
   time_t   voltTimeStampOld;
   char     currentId[16];
   int32_t  * current;
   float    currentMin;
   float    currentMax;
   uint16_t currentTimeOut;
   time_t   * currentTimeStamp;
   time_t   currentTimeStampOld;
}EQ_MAINPOWER_T;


typedef struct {
   uint16_t id;
   char     label[16];
   char     position[32];
   char     alarmEnable;
   uint16_t alarmLastSend[4];
   uint16_t alarmState[4];
   char     brandnModel[32];
   char     coilTempId[16];
   uint16_t * coilTemp;
   float    coilTempMin;
   float    coilTempMax;
   uint16_t coilTempTimeOut;
   time_t   * coilTempTimeStamp;
   time_t   coilTempTimeStampOld;
   float    deltaTempMin;
   float    deltaTempMax;
   char     currentId[16];
   int32_t  * current;
   float    currentMin;
   float    currentMax;
   uint16_t currentTimeOut;
   time_t   * currentTimeStamp;     // last update timestamp
   time_t   currentTimeStampOld;    // last update timestamp
   char     iRId[16];
   uint16_t iRTimeOut;
	 uint8_t  * set_temp;
	 uint8_t  * default_temp;
   time_t   * iRTimeStamp;          // last update timestamp
   time_t   iRTimeStampOld;         // last update timestamp
	 uint32_t start_operation;
	 uint32_t end_operation;

}EQ_AC_T;


typedef struct {
   uint16_t id;
   char     label[16];
   char     position[32];
   char     alarmEnable;
   uint16_t alarmLastSend[3];
   uint16_t alarmState[3];
   char     tempId[16];
   uint16_t * temp;
   float    tempMin;
   float    tempMax;
   uint16_t tempTimeOut;
   time_t   * tempTimeStamp;
   time_t   tempTimeStampOld;
   char     currentId[16];
   int32_t  * current;
   float    currentMin;
   float    currentMax;
   uint16_t currentTimeOut;
   time_t   * currentTimeStamp;        // last update timestamp
   time_t   currentTimeStampOld;
}EQ_CHILLER_T;


typedef struct {
   uint16_t id;
   char     label[16];
   char     position[32];
   char     alarmEnable;
   uint16_t alarmLastSend[1];
   uint16_t alarmState[1];
   char     doorOpenId[16];
   char     doorOpenLogic;
   uint8_t  * doorOpen;
   uint32_t * doorOpenTime;
   uint32_t doorOpenTimeMax;
   uint32_t doorOpenTimeOut;
   time_t   * doorOpenTimeStamp;
   time_t   doorOpenTimeStampOld;
}EQ_DOOR_T;


typedef struct {
   uint16_t id;
   char     label[16];
   char     position[32];
   char     alarmEnable;
   uint16_t alarmLastSend[2];
   uint16_t alarmState[2];
   char     tempId[16];
   uint16_t * temp;
   float    tempMin;
   float    tempMax;
   uint16_t tempTimeOut;
   time_t   * tempTimeStamp;
   time_t   tempTimeStampOld;
   uint16_t * humidity;
   float    humidityMin;
   float    humidityMax;
   uint16_t humidityTimeOut;
   time_t   * humidityTimeStamp;
   time_t   humidityTimeStampOld;
}EQ_ROOM_T;

typedef struct {
   uint16_t id;
   char     label[16];
   char     position[32];
   char     alarmEnable;
   uint16_t alarmLastSend[2];
   uint16_t alarmState[2];
   char     currentId[16];
   int32_t  * current;
	 time_t   pumpOnTimeStamp;
   float    currentMin;
   float    currentMax;
	 uint16_t pumpOnTimeMax;             // in minute
   uint16_t currentTimeOut;
   time_t   * currentTimeStamp;        // last update timestamp
   time_t   currentTimeStampOld;
}EQ_PUMP_T;


typedef struct {
   uint16_t id;
 	 char*    brand;
   char     alarmEnable;
   uint16_t alarmLastSend[2];
   uint16_t alarmState[2];
	 uint16_t io_id;
   int32_t  * current;
   float    currentMin;
   float    currentMax;
   uint16_t currentTimeOut;
   time_t   * currentTimeStamp;        // last update timestamp
   time_t   currentTimeStampOld;
   uint8_t  lampOnLogic;
   uint8_t  * lampOn;
	 uint8_t  lampOnLast;
   uint16_t lampOnTimeOut;
   time_t   * lampOnTimeStamp;        // last update timestamp
   time_t   lampOnTimeStampOld;
	 uint32_t start_operation;
	 uint32_t end_operation;
 //  char     lampOnTimer[16][8];
}EQ_LAMP_T;


typedef struct {
  time_t timeStamp;
  uint16_t equipmentType;
  uint16_t equipmentId;	
  uint16_t alarmCode;
  char value[32];
}EQ_ALARM_T;

#define ALARM_BUFFER_SIZE 128
#define ALARM_BUFFER_MASK (ALARM_BUFFER_SIZE-1)

extern EQ_MAINPOWER_T main_power_equipt[3];
extern EQ_AC_T        ac_equipt[5];
extern EQ_CHILLER_T   chiller_equipt[6];
extern EQ_DOOR_T      door_equipt[5];
extern EQ_ROOM_T      room_equipt[2];
extern EQ_PUMP_T      pump_equipt[2];
extern EQ_LAMP_T      lamp_equipt[10];

void * alarm_sending(void * arg);
void clear_equipment_alarm (void);
void * alarm_checking(void * arg );
void get_main_power_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_ac_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_door_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_lamp_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_room_cfg(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);

#endif