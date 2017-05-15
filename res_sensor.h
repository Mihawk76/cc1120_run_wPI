#ifndef __RES_SENSOR_H__
#define __RES_SENSOR_H__

//#include "res_sensor.h"

extern char scoreData[10000];
extern char temp[1000];	


int test (int kwh[]);
int res_R_I (char* location, int32_t CurrentR[6], int nilai ,int device, int gateway);
int res_S_I (char* location, int32_t CurrentS[6], int nilai ,int device, int gateway);
int res_T_I (char* location, int32_t CurrentT[6], int nilai ,int device, int gateway);
int res_R_I_V_PF (char* location,	int32_t VoltR[6], int32_t CurrentR[6], int32_t PowerFactorR[6], int nilai ,int device, int gateway);
int res_S_I_V_PF (char* location,	int32_t VoltS[6], int32_t CurrentS[6], int32_t PowerFactorS[6], int nilai ,int device, int gateway);
int res_T_I_V_PF (char* location,	int32_t VoltT[6], int32_t CurrentT[6], int32_t PowerFactorT[6], int nilai ,int device, int gateway);
int res_R_Watt_Var (char* location,	int32_t WattR[6], int32_t VarR[6], int nilai ,int device, int gateway);
int res_S_Watt_Var (char* location,	int32_t WattS[6], int32_t VarS[6], int nilai ,int device, int gateway);
int res_T_Watt_Var (char* location,	int32_t WattT[6], int32_t VarT[6], int nilai ,int device, int gateway);
int res_R_WattPrd_WattHour (char* location,	int32_t WattPrdR[6], int32_t WattHourR[6], int nilai ,int device, int gateway);
int res_S_WattPrd_WattHour (char* location,	int32_t WattPrdS[6], int32_t WattHourS[6], int nilai ,int device, int gateway);
int res_T_WattPrd_WattHour (char* location,	int32_t WattPrdT[6], int32_t WattHourT[6], int nilai ,int device, int gateway);
int res_R_VarPrd_VarHour (char* location,	int32_t VarPrdR[6], int32_t VarHourR[6], int nilai ,int device, int gateway);
int res_S_VarPrd_VarHour (char* location,	int32_t VarPrdS[6], int32_t VarHourS[6], int nilai ,int device, int gateway);
int res_T_VarPrd_VarHour (char* location,	int32_t VarPrdT[6], int32_t VarHourT[6], int nilai ,int device, int gateway);
int res_kwh_array (char* location,int32_t KwhR[18],int32_t KwhS[18],int32_t KwhT[18],
								int32_t VoltR[18],int32_t VoltS[18],int32_t VoltT[18],int32_t CurrentR[18],
								int32_t CurrentS[18],int32_t CurrentT[18], int nilai ,int device, int gateway);
int  res_kwh_2 (char* location,uint16_t* VoltR,uint16_t VoltS,uint16_t VoltT,uint16_t CurrentR,
								uint16_t CurrentS,uint16_t CurrentT, int nilai,int device, int Channel);
int  res_kwh (char* location,uint16_t VoltR,uint16_t VoltS,uint16_t VoltT,uint16_t CurrentR,
								uint16_t CurrentS,uint16_t CurrentT, int nilai,int device, int Channel);
int  res_th (char* location,uint16_t Th1,uint16_t Th2,uint16_t Th3,uint16_t Humidity, int nilai,int device, int gateway);
int  res_io (char* location,uint8_t * din_array,uint8_t * dout_array, uint16_t io_id, int gateway, int total_channel);
int  res_val_kwh (char* location,  int id, int gateway, int kwh_index);
int trap_th (char* location, int uID, char* gateway_ID, uint16_t th_ID, uint16_t dIn1, uint16_t dIn2
            , uint16_t hummidity, uint16_t th1, uint16_t th2, uint16_t th3, int16_t rssi
            );
int  res_th_2 (char* location,uint16_t Th1,uint16_t Th2,uint16_t Th3,uint16_t Humidity, int nilai,int device);

#endif // __RES_SENSOR_H__
