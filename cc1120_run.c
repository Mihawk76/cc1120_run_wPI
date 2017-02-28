	/*
	* SPI testing utility (using spidev driver)
	*
	* Copyright (c) 2007  MontaVista Software, Inc.
	* Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
	*a
	* This program is free software; you can redistribute it and/or modify
	* it under the terms of the GNU General Public License as published by
	* the Free Software Foundation; either version 2 of the License.
	*
	* Cross-compile with cross-gcc -I/path/to/cross-kernel/include
	*/

#include <inttypes.h>
#include <math.h>

#include <wiringPi.h>  
#include <wiringPiSPI.h>  
#include <stdio.h>    
#include <stdlib.h>    
#include <stdint.h>  
#include <string.h>  
#include <time.h>
#include <syslog.h>

#include "cc112x_easy_link_reg_config.h"
#include "mac_address.c"

#define STATUS_CLEARED 0
#define STATUS_UPDATED 1

#include "kwh_params.c"
#include "crc16.c"
#include "res_sensor.c"
#include "paring.c"
//#include "read_int.c"
	
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
	
#define ISR_ACTION_REQUIRED 1
#define ISR_IDLE            0
#define RX_FIFO_ERROR       0x11
	
#define CC1120_RST  31
#define CC1120_SSEL 5
#define CC1120_MOSI 12
#define CC1120_MISO 13
#define CC1120_SCLK 14
		
#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
	
#define SENSOR_TONLY    10010
#define SENSOR_BUSDUCT  10020
#define SENSOR_DINONLY  10030
#define VALUE_INVALID   10040

#define TH_NODES_MAX 20

uint8_t Change_freq_ir[] = { 0x0A, 0x06, 0x01, 0x00, 0x42, 0x3D, 0x15, 0x02, 0x00, 0x01, 0x01};
uint8_t ir_command_save[8][100];
uint8_t Panasonic_temp[][100] = {
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xBF ,0x01 ,0xA2 ,0x01 ,0x08 ,0x05 ,0xA2 ,0x0D ,0x01 ,0x40 ,
                          0xBB ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xCF ,0x2A ,0x01 ,0x00 ,0xD6 ,0x0D ,
                          0x01 ,0x40 ,0xBB ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x04 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xB3 ,0x3A ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xCA ,0x01 ,0x9F ,0x01 ,0xFD ,0x04 ,0xBE ,0x0D ,0x01 ,0x40 ,
                          0x9F ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xB3 ,0x2A ,0x01 ,0x00 ,0xDA ,0x0D ,
                          0x01 ,0x40 ,0xB8 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x44 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xF3 ,0x3B ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xC6 ,0x01 ,0x9E ,0x01 ,0xFF ,0x04 ,0xC0 ,0x0D ,0x01 ,0x40 ,
                          0x9E ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xB3 ,0x2A ,0x01 ,0x00 ,0xF4 ,0x0D ,
                          0x01 ,0x40 ,0x9F ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x24 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0x8B ,0x37 ,0x83},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xC6 ,0x01 ,0x9E ,0x01 ,0xFF ,0x04 ,0xC0 ,0x0D ,0x01 ,0x40 ,
                          0x9E ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xB3 ,0x2A ,0x01 ,0x00 ,0xF4 ,0x0D ,
                          0x01 ,0x40 ,0x9F ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x24 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0x8B ,0x37 ,0x83},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xBC ,0x01 ,0xA7 ,0x01 ,0x0A ,0x05 ,0xA1 ,0x0D ,0x01 ,0x40 ,
                          0xBC ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xD2 ,0x2A ,0x01 ,0x00 ,0xBD ,0x0D ,
                          0x01 ,0x40 ,0xD4 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x14 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xAB ,0x3D ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xA6 ,0x01 ,0xBD ,0x01 ,0x1D ,0x05 ,0x88 ,0x0D ,0x01 ,0x40 ,
                          0xD5 ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xEA ,0x2A ,0x01 ,0x00 ,0xBC ,0x0D ,
                          0x01 ,0x40 ,0xD5 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x54 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xEB ,0x2E ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xBD ,0x01 ,0xA4 ,0x01 ,0x0A ,0x05 ,0xA1 ,0x0D ,0x01 ,0x40 ,
                          0xBC ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xD0 ,0x2A ,0x01 ,0x00 ,0xD6 ,0x0D ,
                          0x01 ,0x40 ,0xBB ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x34 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0x9B ,0x2C ,0x86},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xCA ,0x01 ,0xA2 ,0x01 ,0xFD ,0x04 ,0xA5 ,0x0D ,0x01 ,0x40 ,
                          0xB9 ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xCE ,0x2A ,0x01 ,0x00 ,0xD8 ,0x0D ,
                          0x01 ,0x40 ,0xBA ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x74 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xDB ,0x2F ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xBB ,0x01 ,0xAA ,0x01 ,0x0B ,0x05 ,0x89 ,0x0D ,0x01 ,0x40 ,
                          0xD4 ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xD2 ,0x2A ,0x01 ,0x00 ,0xBE ,0x0D ,
                          0x01 ,0x40 ,0xD4 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x0C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xBB ,0x2D ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xCC ,0x01 ,0xA3 ,0x01 ,0xFB ,0x04 ,0xA4 ,0x0D ,0x01 ,0x40 ,
                          0xBA ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xD0 ,0x2A ,0x01 ,0x00 ,0xD7 ,0x0D ,
                          0x01 ,0x40 ,0xBB ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x4C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xFB ,0x32 ,0x82},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0x92 ,0x01 ,0xD0 ,0x01 ,0x36 ,0x05 ,0x76 ,0x0D ,0x01 ,0x40 ,
                          0xE9 ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xEC ,0x2A ,0x01 ,0x00 ,0xA9 ,0x0D ,
                          0x01 ,0x40 ,0xE8 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x2C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0x87 ,0x33 ,0x83},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xBB ,0x01 ,0xB3 ,0x01 ,0x0F ,0x05 ,0xA1 ,0x0D ,0x01 ,0x40 ,
                          0xBC ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xD2 ,0x2A ,0x01 ,0x00 ,0xBE ,0x0D ,
                          0x01 ,0x40 ,0xD4 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x6C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xC7 ,0x2D ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xA2 ,0x01 ,0xBF ,0x01 ,0x24 ,0x05 ,0x89 ,0x0D ,0x01 ,0x40 ,
                          0xD6 ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xEA ,0x2A ,0x01 ,0x00 ,0xBC ,0x0D ,
                          0x01 ,0x40 ,0xD5 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x1C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xA7 ,0x33 ,0x82},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xD3 ,0x01 ,0xA3 ,0x01 ,0xF4 ,0x04 ,0xA2 ,0x0D ,0x01 ,0x40 ,
                          0xBB ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xD0 ,0x2A ,0x01 ,0x00 ,0xD6 ,0x0D ,
                          0x01 ,0x40 ,0xBB ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x5C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0xE7 ,0x30 ,0x85},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xE1 ,0x01 ,0x9C ,0x01 ,0xE3 ,0x04 ,0xC2 ,0x0D ,0x01 ,0x40 ,
                          0x9B ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xC9 ,0x2A ,0x01 ,0x00 ,0xDE ,0x0D ,
                          0x01 ,0x40 ,0xB3 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x9C ,0x3C ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0x97 ,0x2E ,0x81},
{0x40 ,0x11 ,0x01 ,0x00 ,0x42 ,0x3D ,0x15 ,0x00 ,0x00 ,0x37 ,
                          0xA1 ,0x01 ,0xD3 ,0x01 ,0x23 ,0x05 ,0x78 ,0x0D ,0x01 ,0x40 ,
                          0xE5 ,0x06 ,0x40 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,0x00 ,0x00 ,
                          0x00 ,0x60 ,0x09 ,0x80 ,0xE3 ,0x2A ,0x01 ,0x00 ,0xC3 ,0x0D ,
                          0x01 ,0x40 ,0xD0 ,0x06 ,0x98 ,0xC0 ,0x40 ,0x04 ,0x07 ,0x20 ,
                          0x00 ,0x1C ,0x64 ,0x01 ,0xFE ,0x00 ,0x00 ,0x70 ,0x07 ,0x00 ,
                          0x00 ,0x81 ,0x00 ,0x00 ,0x4B ,0x28 ,0x81}};

typedef enum _CC1120_COMM_CMD {
	COMM_JOINT = 1,
	COMM_SCAN,
	COMM_UNJOINT,
	COMM_INFO,
	COMM_SYNC,
	COMM_ADD,
	COMM_QUERY,

	COMM_JOINT_RPL = 0x81,
	COMM_SCAN_RPL,
	COMM_UNJOINT_RPL,
	COMM_INFO_RPL,
	COMM_SYNC_RPL,
	COMM_ADD_RPL,
	COMM_QUERY_RPL,

	COMM_SEND_IR = 0x11,
	COMM_VALUES_GET,
	COMM_ADE_GET,
	COMM_ADE_SET,
	COMM_UTILS_SET,
	
	COMM_GW_TO_TH = 0x41,
	
	COMM_SEND_IR_RPL = 0x91,
	COMM_VALUES_RPL,
	COMM_ADE_GET_RPL,
	COMM_ADE_SET_RPL,
	COMM_UTILS_RPL,
	
	COMM_TH_TO_GW = 0xC1,
	
} CC1120_COMM_CMD;



const char th_type_str[4][7]={
  "T-Only\0",
  "T-BDuc\0",
  "D-Only\0",
  "T-Humi\0"
};

typedef struct {
	uint8_t id;
	uint8_t status;
	uint8_t type;
	uint8_t loop_h;
	uint8_t loop_t1;
	uint8_t loop_t2;
	uint8_t loop_t3;
	uint16_t past_h[3];
	uint16_t past_t1[3];
	uint16_t past_t2[3];
	uint16_t past_t3[3];
	uint16_t median_h;
	uint16_t median_t1;
	uint16_t median_t2;
	uint16_t median_t3;
	uint8_t din1;
	uint8_t din2;
	uint8_t batt;
	uint8_t tx_rssi;
	uint32_t tx_counter;
	time_t ts;  // time stamp
	uint16_t ir_id;
	uint8_t th_set;
  char* ac_type;
}TH_NODE_T;

typedef struct {
	int start_hour;
	int close_hour;
}STORE;

STORE Pondok_Pinang;
uint8_t t_wakeup_interval = 3;
uint8_t th_wakeup_interval = 6;
uint8_t ds_wakeup_interval = 3;

uint8_t txBuffer[256];
uint8_t rxBuffer[256];
uint16_t packetCounter = 0;
uint8_t scan_key = 0x00;
uint8_t add_type = 0x02; //add type as new node
uint8_t index_node = 0x00; //temp index node 
uint8_t wakeup_hold = 0x05; //wake up hold in 100ms
uint16_t cc1120_TH_ID;
int TOTAL_TH_ID;
uint16_t cc1120_TH_ID_Selected[TH_NODES_MAX] = { 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint16_t cc1120_IR_ID_Selected[TH_NODES_MAX] = { 0x3D42, 0x3D42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t cc1120_TH_SET[TH_NODES_MAX] = { 29, 29, 29, 29, 29, 29, 29, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char* AC_TYPE[TH_NODES_MAX] = { "Panasonic","Panasonic","Panasonic","Panasonic","Panasonic","Panasonic","Panasonic","Panasonic"};

TH_NODE_T th_nodes[TH_NODES_MAX];
uint32_t cc1120_KWH_ID;
int cc1120_TH_Listed = 6;
uint8_t cc1120_TH_Node;
int flag_ir = 0;
uint16_t gateway_ID;
uint16_t kwh_ID;
uint16_t mac_address_gateway;
int freq_main;
uint8_t remChannel;
int kwh_loop = 0;
//int infrared_loop = 15;//7 
int infrared_loop = 4;
int loop_th_id = 0;
uint8_t pktCmdx;
int16_t rssi = 0;

int32_t replyDly;

//char* location = "http://35.160.141.229/post.php";
//char* location = "http://52.43.48.93/dcms/rest/alfa";
//char* location = "http://192.168.88.19:1616/dcms/rest/alfa";
//char* location = "http://10.42.0.1/post.php";
//char* location = "http://35.166.240.126:3000/api/ths";
//char* location_kwh = "http://35.166.240.126:3000/api/WattPrd_Is";
char* location = "http://alfademo.ddns.net:3000/api/ths/save";
char* location_kwh = "http://alfademo.ddns.net:3000/api/WattPrd_Is/save";
char* location_wattR = "http://35.160.141.229:3000/api/RWatts";
char* location_wattS = "http://35.160.141.229:3000/api/SWatts";
char* location_wattT = "http://35.160.141.229:3000/api/TWatts";
char* gateway_trap_id = "EM24010101";
FILE *f;

void cc1120_service(void);
void res_service(void);
void poll_kwh_service(void);

uint8_t tbuff_kwh_poll[256];

#define ADE_NODE_TYPE 0x14	
/*******************************************************************************
 * @fn          trxReadWriteBurstSingle
 *
 * @brief       When the address byte is sent to the SPI slave, the next byte
 *              communicated is the data to be written or read. The address
 *              byte that holds information about read/write -and single/
 *              burst-access is provided to this function.
 *
 *              Depending on these two bits this function will write len bytes to
 *              the radio in burst mode or read len bytes from the radio in burst
 *              mode if the burst bit is set. If the burst bit is not set, only
 *              one data byte is communicated.
 *
 *              NOTE: This function is used in the following way:
 *
 *              TRXEM_SPI_BEGIN();
 *              while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);
 *              ...[Depending on type of register access]
 *              trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len);
 *              TRXEM_SPI_END();
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @return      int
 */

static int trxReadWriteBurstSingle(uint8_t addr, uint8_t *pData, uint16_t len)
{
  int ret;
  uint8_t temp[len];

  /* Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave*/
  if(addr&RADIO_READ_ACCESS)
  {
    memset(temp,0x00,len);
	wiringPiSPIDataRW(0, pData, len);
	ret = pData[len-1];
  }
  else
  {
	memcpy(temp,pData,len);
	wiringPiSPIDataRW(0, temp, len);
	ret = temp[len-1];
  }
  
  
  return ret;
}
	
	
uint8_t trxSpiCmdStrobe(uint8_t cmd)
{
  
  /* Pull CS_N low */
  digitalWrite(CC1120_SSEL,  LOW) ; 
  
  uint8_t tx[] = {
	cmd, 0,
  };
     
  wiringPiSPIDataRW(0, tx, ARRAY_SIZE(tx)); 
  
 
  /* Pull CS_N high */
  digitalWrite(CC1120_SSEL,  HIGH) ; 

  /* return the status byte value */
  return(tx[1]);
}


/*******************************************************************************
 * @fn          trx8BitRegAccess
 *
 * @brief       This function performs a read or write from/to a 8bit register
 *              address space. The function handles burst and single read/write
 *              as specfied in addrByte. Function assumes that chip is ready.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       addrByte - address byte of register.
 * @param       pData    - data array
 * @param       len      - Length of array to be read(TX)/written(RX)
 *
 * output parameters
 *
 * @return      chip status
 */
uint8_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len)
{
  uint8_t d = accessType|addrByte;
  uint8_t tx[] = {d};

  /* Pull CS_N low*/
  digitalWrite(CC1120_SSEL,  LOW);

  wiringPiSPIDataRW( 0, tx, ARRAY_SIZE(tx));
  trxReadWriteBurstSingle( d, pData, len);
 
  /* Pull CS_N high */
  digitalWrite( CC1120_SSEL,  HIGH) ;
  
  /* return the status byte value */
  return pData[0];
}

/******************************************************************************
 * @fn          trx16BitRegAccess
 *
 * @brief       This function performs a read or write in the extended adress
 *              space of CC112X.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       extAddr - Extended register space address = 0x2F.
 * @param       regAddr - Register address in the extended address space.
 * @param       *pData  - Pointer to data array for communication
 * @param       len     - Length of bytes to be read/written from/to radio
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t trx16BitRegAccess(uint8_t accessType, uint8_t extAddr, uint8_t regAddr, uint8_t *pData, uint8_t len)
{
  /* Pull CS_N low*/
  digitalWrite(CC1120_SSEL,  LOW) ;
 
  uint8_t d = accessType|extAddr;
  uint8_t tx[] = {d, regAddr};

  wiringPiSPIDataRW(0, tx, ARRAY_SIZE(tx));
  trxReadWriteBurstSingle( d, pData, len);

 
  /* Pull CS_N high*/
  digitalWrite(CC1120_SSEL,  HIGH) ;

  /* return the status byte value */
  return pData[0];
}



/******************************************************************************
 * @fn          cc112xSpiReadReg
 *
 * @brief       Read value(s) from config/status/extended radio register(s).
 *              If len  = 1: Reads a single register
 *              if len != 1: Reads len register values in burst mode 
 *
 * input parameters
 *
 * @param       addr   - address of first register to read
 * @param       *pData - pointer to data array where read bytes are saved
 * @param       len   - number of bytes to read
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiReadReg(uint16_t addr, uint8_t *pData, uint8_t len)
{
  uint8_t tempExt  = (uint8_t)(addr>>8);
  uint8_t tempAddr = (uint8_t)(addr & 0x00FF);
  uint8_t rc=0;
  
  /* Checking if this is a FIFO access -> returns chip not ready  */
  if((CC112X_SINGLE_TXFIFO<=tempAddr)&&(tempExt==0)) return STATUS_CHIP_RDYn_BM;
  
  /* Decide what register space is accessed */
  if(!tempExt)
  {
    rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),tempAddr,pData,len);
  }
  else if (tempExt == 0x2F)
  {
    rc = trx16BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),tempExt,tempAddr,pData,len);
  }
  return (rc);
}

/******************************************************************************
 * @fn          cc112xSpiWriteReg
 *
 * @brief       Write value(s) to config/status/extended radio register(s).
 *              If len  = 1: Writes a single register
 *              if len  > 1: Writes len register values in burst mode 
 *
 * input parameters
 *
 * @param       addr   - address of first register to write
 * @param       *pData - pointer to data array that holds bytes to be written
 * @param       len    - number of bytes to write
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiWriteReg(uint16_t addr, uint8_t *pData, uint8_t len)
{
  uint8_t tempExt  = (uint8_t)(addr>>8);
  uint8_t tempAddr = (uint8_t)(addr & 0x00FF);
  uint8_t rc=0;
  
  /* Checking if this is a FIFO access - returns chip not ready */
  if((CC112X_SINGLE_TXFIFO<=tempAddr)&&(tempExt==0)) return STATUS_CHIP_RDYn_BM;
  	
  /* Decide what register space is accessed */  
  if(!tempExt)
  {
    rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),tempAddr,pData,len);
  }
  else if (tempExt == 0x2F)
  {
    rc = trx16BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),tempExt,tempAddr,pData,len);
  }
  return (rc);
}



/*******************************************************************************
*   @fn         cc112xregConfig
*
*   @brief      Write register settings as given by SmartRF Studio found in
*               cc112x_easy_link_reg_config.h
*
*   @param      freq - frequency channel (0-80)
*
*   @return     none
*/
#define CC112X_FREQ_BASE 0x00668000l   // frequency base is 410Mhz
#define CC112X_FREQ_STEP 0x4000        // frequency step is 1Mhz

void cc112xregConfig(uint8_t freq) {

    uint8_t writeByte;
    uint16_t i;
	uint32_t freq_code;
	freq_code = CC112X_FREQ_BASE + ((uint16_t)freq * CC112X_FREQ_STEP);// original code

    // Reset radio
    trxSpiCmdStrobe(CC112X_SRES);
    
	delay(100);
	
	trxSpiCmdStrobe(CC112X_SRES);
	
    // Write registers to radio
	for(i = 0; i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) {
		writeByte = preferredSettings[i].data;
		if (preferredSettings[i].addr == CC112X_FREQ2) writeByte = (freq_code >> 16) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ1) writeByte = (freq_code >> 8) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ0) writeByte = freq_code & 0xff;
		cc112xSpiWriteReg(preferredSettings[i].addr, &writeByte, 1);
    }
}

/*******************************************************************************
*   @fn         registerConfigChek
*
*   @brief      Write register settings as given by SmartRF Studio found in
*               cc112x_easy_link_reg_config.h
*
*   @param      none
*
*   @return     int (0: all registers config match, 1: one or more registers config didn't match)
*/
int cc112xConfigChek(uint8_t freq) {

    uint8_t readByte; 
	  uint8_t cmpByte;
	  //buf[20];
    uint16_t i;
   	uint32_t freq_code;
	
    freq_code = CC112X_FREQ_BASE + ((uint16_t)freq * CC112X_FREQ_STEP);


    // Read registers to radio
    for(i = 0; i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) {
        readByte = 0x00;
		cmpByte = preferredSettings[i].data;
        cc112xSpiReadReg(preferredSettings[i].addr, &readByte, 1);
		if (preferredSettings[i].addr == CC112X_FREQ2) cmpByte = (freq_code >> 16) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ1) cmpByte = (freq_code >> 8) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ0) cmpByte = freq_code & 0xff;
		if (readByte != cmpByte) return 1;
    }
	return 0;
	
}




/*******************************************************************************
*   @fn         manualCalibration
*
*   @brief      Calibrates radio according to CC112x errata
*
*   @param      none
*
*   @return     none
*/
#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2

static void manualCalibration(void) {

    uint8_t original_fs_cal2;
    uint8_t calResults_for_vcdac_start_high[3];
    uint8_t calResults_for_vcdac_start_mid[3];
    uint8_t marcstate;
    uint8_t writeByte;
		//FILE *writeFile = fopen("/home/data.log", "a");
		//if (writeFile == NULL)
		//{
    	//printf("Error opening file!\n");
    	//exit(1);
		//}

    // 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 2) Start with high VCDAC (original VCDAC_START + 2):
    cc112xSpiReadReg(CC112X_FS_CAL2, &original_fs_cal2, 1);
    writeByte = original_fs_cal2 + VCDAC_START_OFFSET;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 3) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
		printf("marcstate: %02x\r\n", marcstate);
		//fprintf(writeFile, "marcstate: %02x\r\n", marcstate);

    } while (marcstate != 0x41);


    // 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with 
    //    high VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2,
                     &calResults_for_vcdac_start_high[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_high[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_high[FS_CHP_INDEX], 1);

    // 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 6) Continue with mid VCDAC (original VCDAC_START):
    writeByte = original_fs_cal2;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 7) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
		printf("marcstate: %02x\r\n", marcstate);
		//fprintf(writeFile, "marcstate: %02x\r\n", marcstate);
    } while (marcstate != 0x41);

    // 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained 
    //    with mid VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2, 
                     &calResults_for_vcdac_start_mid[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_mid[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_mid[FS_CHP_INDEX], 1);

    // 9) Write back highest FS_VCO2 and corresponding FS_VCO
    //    and FS_CHP result
    if (calResults_for_vcdac_start_high[FS_VCO2_INDEX] >
        calResults_for_vcdac_start_mid[FS_VCO2_INDEX]) {
        writeByte = calResults_for_vcdac_start_high[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    } else {
        writeByte = calResults_for_vcdac_start_mid[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    }
		//fclose(writeFile);
}

/*******************************************************************************
 * @fn          cc112xSpiWriteTxFifo
 *
 * @brief       Write pData to radio transmit FIFO.
 *
 * input parameters
 *
 * @param       *pData - pointer to data array that is written to TX FIFO
 * @param       len    - Length of data array to be written
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiWriteTxFifo(uint8_t *pData, uint8_t len)
{
  uint8_t rc;
  rc = trx8BitRegAccess(0x00,CC112X_BURST_TXFIFO, pData, len);
  return (rc);
}

/*******************************************************************************
 * @fn          cc112xSpiReadRxFifo
 *
 * @brief       Reads RX FIFO values to pData array
 *
 * input parameters
 *
 * @param       *pData - pointer to data array where RX FIFO bytes are saved
 * @param       len    - number of bytes to read from the RX FIFO
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiReadRxFifo(uint8_t * pData, uint8_t len)
{
  uint8_t rc;
  rc = trx8BitRegAccess(0x00,CC112X_BURST_RXFIFO, pData, len);
  return (rc);
}

/******************************************************************************
 * @fn      cc112xGetTxStatus(void)
 *          
 * @brief   This function transmits a No Operation Strobe (SNOP) to get the 
 *          status of the radio and the number of free bytes in the TX FIFO.
 *          
 *          Status byte:
 *          
 *          ---------------------------------------------------------------------------
 *          |          |            |                                                 |
 *          | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (free bytes in the TX FIFO |
 *          |          |            |                                                 |
 *          ---------------------------------------------------------------------------
 *
 *
 * input parameters
 *
 * @param   none
 *
 * output parameters
 *         
 * @return  rfStatus_t 
 *
 */
uint8_t cc112xGetTxStatus(void)
{
    return(trxSpiCmdStrobe(CC112X_SNOP));
}

/******************************************************************************
 *
 *  @fn       cc112xGetRxStatus(void)
 *
 *  @brief   
 *            This function transmits a No Operation Strobe (SNOP) with the 
 *            read bit set to get the status of the radio and the number of 
 *            available bytes in the RXFIFO.
 *            
 *            Status byte:
 *            
 *            --------------------------------------------------------------------------------
 *            |          |            |                                                      |
 *            | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (available bytes in the RX FIFO |
 *            |          |            |                                                      |
 *            --------------------------------------------------------------------------------
 *
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 * @return    rfStatus_t 
 *
 */
uint8_t cc112xGetRxStatus(void)
{
    return(trxSpiCmdStrobe(CC112X_SNOP | RADIO_READ_ACCESS));
}

/******************************************************************************
 *
 *  @fn       cc112x_hw_rst(void)
 *
 *  @brief   
 *            Reset cc112x by reset pin.
 *            
 *
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 * @return    none
 *
 */
void cc112x_hw_rst(void) {
  digitalWrite(CC1120_RST,  LOW) ;
  delay(50);  
  digitalWrite(CC1120_RST,  HIGH) ;
}

/******************************************************************************
 *
 *  @fn       cc112xpwrConfig(uint8_t pwrTx)
 *
 *  @brief   
 *            Set Tx Power
 *            
 *
 *
 * input parameters
 *
 * @param     pwrTx - Tx power value
 *
 * output parameters
 *         
 * @return    none
 *
 */
void cc112xpwrConfig(uint8_t pwrTx) {
    uint8_t writeByte;
	writeByte = pwrTx;
    cc112xSpiWriteReg(CC112X_PA_CFG2, &writeByte, 1);
}

/******************************************************************************
 *
 *  @fn       uint8_t wait_exp_val(uint16_t addr, uint8_t exp_val)
 *
 *  @brief   
 *            Waiting until register value and expected value is match
 *            
 *
 * input parameters
 *
 * @param     addr    - register address
 * @param     exp_val - expected value
 *
 * output parameters
 *         
 *
 * @return   register value;
 *
 */
uint8_t wait_exp_val(uint16_t addr, uint8_t exp_val) {
	uint8_t temp_byte;
	do {
		cc112xSpiReadReg(addr, &temp_byte, 1);
	} while (temp_byte != exp_val);
	return temp_byte;
}


/******************************************************************************
 *
 *  @fn       cc112x_init(uint8_t freq, uint8_t pwr)
 *
 *  @brief   
 *            Initialize/configure CC112x with given freq channel & tx power
 *            
 *
 * input parameters
 *
 * @param     freq    - frequency channel
 * @param     pwr     - tx power
 *
 * output parameters
 *         
 *
 * @return   none
 *
 */
void  cc112x_init(uint8_t freq, uint8_t pwr)
{	
	uint16_t ii;
	ii = 0;
	do {
		cc112xregConfig(freq);
		if (( ii++ % 5) == 0) {
			/*** CC112X HW Reset ***/
			cc112x_hw_rst();
			printf("CC1120 HW Reset.\r\n");
			
		}
		if ( ii > 15 ) {
		  printf("CC1120 can not init.\r\n");
		  exit(1);
		}
		
	} while (cc112xConfigChek(freq));
	
	if (pwr>0x3e) pwr = 0x3e;
	cc112xpwrConfig(0x7f-pwr);
	
	manualCalibration();

	memset(txBuffer,0,sizeof(txBuffer));
	memset(rxBuffer,0,sizeof(rxBuffer));
	
	trxSpiCmdStrobe(CC112X_SIDLE);
	
	// Flush RX FIFO
	trxSpiCmdStrobe(CC112X_SFRX);
	
	// Flush TX FIFO
	trxSpiCmdStrobe(CC112X_SFTX);

	wait_exp_val(CC112X_MARCSTATE, 0x41);
	
	// Set radio in RX
	trxSpiCmdStrobe(CC112X_SRX);
			
}

/******************************************************************************
 *
 *  @fn       uint8_t send_packet(uint8_t * sendBuffer)
 *
 *  @brief   
 *            Send packet data from given buffer (sendBuffer)
 *			  the first byte of sendBuffer is data length
 *            data length must > 0
 *
 * input parameters
 *
 * @param     sendBuffer   - Buffer of data send
 *
 * output parameters
 *         
 *
 * @return   send result status, 0 = success, another value = fail
 *
 */
uint8_t send_packet(uint8_t * sendBuffer) {
	uint8_t temp_byte;
	uint8_t len,i;
	len = *sendBuffer;
	if (len == 0) return 0;
	
	cc112xSpiReadReg(CC112X_MARCSTATE, &temp_byte, 1);
	if (temp_byte != 0x6d) 
		return 0;
	
	cc112xSpiReadReg(CC112X_RSSI0, &temp_byte, 1);
	if (temp_byte & 0x04) {
		len =  0; // if RSSI present from another 
		return 0;
	}
	
    printf("SendPacket: ");
    for (i=1;i<len;i++) {
	  printf("0x%02X ",sendBuffer[i]);
	}
	printf("0x%02X\r\n", sendBuffer[i]);

	
	trxSpiCmdStrobe(CC112X_SIDLE);
	wait_exp_val(CC112X_MARCSTATE, 0x41);

	trxSpiCmdStrobe(CC112X_SFTX);					/*  SFTX    - Flush the TX FIFO buffer. */	
	trxSpiCmdStrobe(CC112X_SFRX);					/*  SFRX    - Flush the RX FIFO buffer. */

	temp_byte = 0x06;
	cc112xSpiWriteReg(CC112X_IOCFG2, &temp_byte, 1);
	
	// Write packet to TX FIFO
	cc112xSpiWriteTxFifo(sendBuffer, len+1);

	// Strobe TX to send packet
	trxSpiCmdStrobe(CC112X_STX);
	do {
		//os_dly_wait (1);
		cc112xSpiReadReg(CC112X_MARC_STATUS1, &temp_byte, 1);
		//printf("marcstate: %02x\r\n", marcState);
	} while (( temp_byte) == 0);
	
	if ( temp_byte & 0x40) *sendBuffer = 0;
	return temp_byte;
}
// middle filter function
uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c)
{
 uint16_t middle;

 if ((a <= b) && (a <= c))
 {
   middle = (b <= c) ? b : c; 
 }
 else if ((b <= a) && (b <= c))
 {
   middle = (a <= c) ? a : c; 
 }
 else 
 {
   middle = (a <= b) ? a : b; 
 }
 return middle;
}

int processPacket(uint8_t *bufferin, uint8_t *bufferout, uint8_t len) {
    uint8_t temp_8;
    uint8_t i,ii;
    uint16_t temp_16;
    uint32_t temp_32;
    uint8_t pktCmd;
    uint8_t srcAddr;
	
    struct timespec spec;
    long   ms; // Milliseconds

    uint32_t interval_between_node;
    uint32_t current_stamp;
    uint32_t next_stamp, interval_between_node_max;
	
    int ret = 0;

    uint8_t rssiRx;
	
    pktCmd = *bufferin;
    i = 0;

    switch (pktCmd) {

	    case	COMM_TH_TO_GW:
				//Tx.. Data
				//frame: CMD, NodeID, humidity, temp1, temp2, temp3, carry, rssirx, dinBat count,  cs
				//         1       1         1      1      1      1      1     1       0,5   2.5    1
				// length data always 12 byte, reject it if different
				if(len!=12) break;
					 
				// calculate checksum
				temp_8 = 0;
				for(ii=0;ii<len-1;ii++) {
					temp_8 += bufferin[ii];
				}

				// compare calculated checksum vs given checksum
				if (temp_8 !=  bufferin[ii]) {
					printf(" Invalid TH data Received\r\n");
					break;
				}


				srcAddr = bufferin[1]; // get destination address	
		
				i = srcAddr-1;
				if (i>=TH_NODES_MAX) break; //overleap Source address
		
				//Get Tx Counter of TH 	
				temp_32 = bufferin[8] & 0x0f;
				temp_32 <<= 16;
				memcpy((uint8_t*) &temp_32, (uint8_t*) &bufferin[9], 2);
				th_nodes[i].tx_counter = temp_32+1;
				th_nodes[i].tx_counter &=0x000fffff;

				//Get humidity data and th type 
				temp_16 = bufferin[6] & 0xc0;
				temp_16 <<= 2;
				temp_16 += bufferin[2];
				temp_16 *= 10;
			
				// if data < SENSOR_TONLY,  mean humidity data is valid
				if (temp_16 < SENSOR_TONLY) th_nodes[i].type = 3; 			
				else th_nodes[i].type = ((temp_16&0xffff)/10)-1001; //store Th Type
		
				temp_16 &= 0x3fff ;
				if (temp_16>=SENSOR_TONLY) temp_16=0x2a2a;
				if (th_nodes[i].loop_h==0xff) { //initialize past buffer
					th_nodes[i].past_h[1] = th_nodes[i].past_h[2] = temp_16;
					th_nodes[i].loop_h = 0;		
				}
				th_nodes[i].past_h[th_nodes[i].loop_h] = temp_16; //store humidity value
		
			
				//Get Temp1 data
				temp_16 = bufferin[6] & 0x30;
				temp_16 <<= 4;
				temp_16 += bufferin[3];
				temp_16 *= 10;
				temp_16 &= 0x3fff ;
				if (temp_16>=VALUE_INVALID) temp_16=0x2a2a;
				if (th_nodes[i].loop_t1==0xff) { //initialize past buffer
					th_nodes[i].past_t1[1] = th_nodes[i].past_t1[2] = temp_16;
					th_nodes[i].loop_t1 = 0;		
				}
				th_nodes[i].past_t1[th_nodes[i].loop_t1] = temp_16; //store temp1 value

		
				//Get Temp2 data		
				temp_16 = bufferin[6] & 0x0C;
				temp_16 <<= 6;
				temp_16 += bufferin[4];
				temp_16 *= 10;
				temp_16 &= 0x3fff ;
				if (temp_16>=VALUE_INVALID) temp_16=0x2a2a;
				if (th_nodes[i].loop_t2==0xff) { //initialize past buffer
					th_nodes[i].past_t2[1] = th_nodes[i].past_t2[2] = temp_16;
					th_nodes[i].loop_t2 = 0;		
				}
				th_nodes[i].past_t2[th_nodes[i].loop_t2] = temp_16; //store temp2 value

				//Get Temp3 data		
				temp_16 = bufferin[6] & 0x03;
				temp_16 <<= 8;
				temp_16 += bufferin[5];
				temp_16 *= 10;
				temp_16 &= 0x3fff ;
				if (temp_16>=VALUE_INVALID) temp_16=0x2a2a;
				if (th_nodes[i].loop_t3==0xff) { //initialize past buffer
					th_nodes[i].past_t3[1] = th_nodes[i].past_t3[2] = temp_16;
					th_nodes[i].loop_t3 = 0;		
				}
				th_nodes[i].past_t3[th_nodes[i].loop_t3] = temp_16; //store temp3 value

				//Get battery status
				ii = *(uint8_t *)(bufferin+8);		// Get Batt Status
				ii >>= 4; 
				ii &= 0x03;
				th_nodes[i].batt = ii; //Batt Status value

				//Get Din1 status
				ii = *(uint8_t *)(bufferin+8); // Get Din1 Status
				ii >>= 6;
				ii &= 0x01;
				th_nodes[i].din1 = ii;

				//Get Din2 status
				ii = *(uint8_t *)(bufferin+8); // Get Din2 Status
				ii >>= 7;
				ii &= 0x01;
				th_nodes[i].din2 = ii;

				rssiRx    = bufferin[len];
				th_nodes[i].tx_rssi = bufferin[7];

				/* without median filter
				th_nodes[i].median_h = th_nodes[i].past_h[th_nodes[i].loop_h];
				th_nodes[i].median_t1 = th_nodes[i].past_t1[th_nodes[i].loop_t1];
				th_nodes[i].median_t2 = th_nodes[i].past_t2[th_nodes[i].loop_t2];
				th_nodes[i].median_t3 = th_nodes[i].past_t3[th_nodes[i].loop_t3];
				*/
		
				/* with median filter*/
				th_nodes[i].median_h = middle_of_3(th_nodes[i].past_h[0], th_nodes[i].past_h[1], th_nodes[i].past_h[2]);
				th_nodes[i].median_t1 = middle_of_3(th_nodes[i].past_t1[0], th_nodes[i].past_t1[1], th_nodes[i].past_t1[2]);
				th_nodes[i].median_t2 = middle_of_3(th_nodes[i].past_t2[0], th_nodes[i].past_t2[1], th_nodes[i].past_t2[2]);
				th_nodes[i].median_t3 = middle_of_3(th_nodes[i].past_t3[0], th_nodes[i].past_t3[1], th_nodes[i].past_t3[2]);
		
				th_nodes[i].loop_h++;
				if (th_nodes[i].loop_h>=3) th_nodes[i].loop_h = 0;	
				th_nodes[i].loop_t1++;
				if (th_nodes[i].loop_t1>=3) th_nodes[i].loop_t1 = 0;
				th_nodes[i].loop_t2++;
				if (th_nodes[i].loop_t2>=3) th_nodes[i].loop_t2 = 0;
				th_nodes[i].loop_t3++;
				if (th_nodes[i].loop_t3>=3) th_nodes[i].loop_t3 = 0;

				if ( th_nodes[i].id != srcAddr ) {
					printf(" UnReg %s ID:%02d", th_type_str[th_nodes[i].type], srcAddr);
				}
				else { 
					printf(" Reg   %s ID:%02d", th_type_str[th_nodes[i].type], srcAddr);
					th_nodes[i].status = STATUS_UPDATED;
				}

				clock_gettime(CLOCK_REALTIME, &spec);
		
				th_nodes[i].ts  = spec.tv_sec;
				ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds

				current_stamp = (th_nodes[i].ts * 100) + (ms/10); //convert to 10* ms 
		
				if   (th_nodes[i].median_h == 0x2a2a)  printf(" H -n.a-");
				else  printf(" H~%02d.%02d", th_nodes[i].median_h/100,th_nodes[i].median_h%100);
				if   (th_nodes[i].median_t1 == 0x2a2a)  printf(" 1 -n.a-");
				else  printf(" 1~%02d.%02d", th_nodes[i].median_t1/100, th_nodes[i].median_t1%100);
				if   (th_nodes[i].median_t2 == 0x2a2a)  printf(" 2 -n.a-");
				else  printf(" 2~%02d.%02d", th_nodes[i].median_t2/100, th_nodes[i].median_t2%100);
				if   (th_nodes[i].median_t3 == 0x2a2a)  printf(" 3 -n.a-");
				else  printf(" 3~%02d.%02d", th_nodes[i].median_t3/100, th_nodes[i].median_t3%100);
				printf(" %s %s", (th_nodes[i].din1==0) ? "On " : "Off", (th_nodes[i].din2==0) ? "On " : "Off");
				printf(" B~%s Tx:%03d\r\n", (th_nodes[i].batt==3) ? "Full" : "Low ", th_nodes[i].tx_rssi-102);

				switch(th_nodes[i].type) {
					case 0:  //SENSOR_TONLY:
						bufferout[4] = t_wakeup_interval;
						interval_between_node = (t_wakeup_interval*6000) / TH_NODES_MAX;
						next_stamp = ((current_stamp / (t_wakeup_interval*6000))+1) * (t_wakeup_interval*6000);
						break;

					case 1:  //SENSOR_BUSDUCT:
						bufferout[4] = ds_wakeup_interval;
						interval_between_node = (ds_wakeup_interval*6000) / TH_NODES_MAX;
						next_stamp = ((current_stamp / (ds_wakeup_interval*6000))+1) * (ds_wakeup_interval*6000);
						break;

					default:
						bufferout[4] = th_wakeup_interval;
						interval_between_node = (th_wakeup_interval*6000) / TH_NODES_MAX;
						next_stamp = ((current_stamp / (th_wakeup_interval*6000))+1) * (th_wakeup_interval*6000);
						break;
				}
				if (interval_between_node > 600) interval_between_node_max = 600;
				else interval_between_node_max = interval_between_node;
				
				next_stamp = next_stamp + (interval_between_node_max*i);
				next_stamp = next_stamp - current_stamp;
					
				if (next_stamp>=((interval_between_node*TH_NODES_MAX)+((interval_between_node*TH_NODES_MAX)/2))) 
					next_stamp -= (interval_between_node*TH_NODES_MAX); 
				else if (next_stamp <= interval_between_node) 
					next_stamp += (interval_between_node*TH_NODES_MAX); 

				bufferout[1] = COMM_GW_TO_TH;
				bufferout[2] = srcAddr;	
				bufferout[3] = remChannel; 
				memset(bufferout+5,0,2);
				memcpy(bufferout+5, (uint8_t*) &next_stamp, 2);
				memset(bufferout+7,0,3);
				memcpy(bufferout+7, (uint8_t*) &current_stamp, 3);
				//	printf("ID: %d Stamp :%d, NextWakeup: %d\r\n",srcAddr,current_stamp, next_stamp);


		
				bufferout[10] = 0;  // init checksum
				// calculate checksum
				for(ii=1;ii<10; ii++) {
					bufferout[10] += bufferout[ii];
				}
				bufferout[0] =  10;
				replyDly = 10000;
				
				syslog(LOG_INFO, "TH ID: %04X TH_ID_Selected:%04X\n", srcAddr, cc1120_TH_ID_Selected[i]);

				syslog(LOG_INFO, "Humidity : %d Temp 1 : %d Temp2 : %d Temp 3 : %d Din1 : %d Din2 : %d rssi : %d\n",
						 th_nodes[i].median_h, th_nodes[i].median_t1, th_nodes[i].median_t2, th_nodes[i].median_t3, th_nodes[i].din1, th_nodes[i].din2, rssiRx);
				printf("Gateway Id %d\n", gateway_ID);
				syslog(LOG_INFO, "Gateway Id %d\n", gateway_ID);
				break;
			
	    case COMM_VALUES_RPL:
		  temp_32 = 0;
		  temp_16 = crc16((uint8_t *) bufferin, 0xffff, len-2);
		  memcpy((uint8_t *) &temp_32, (uint8_t *) (bufferin+(len-2)), 2);
		  //printf("\r\nMasuk %04X:%04X %s\r\n", temp_16, temp_32, (temp_32==temp_16)?"CS OK":"CS ERROR");
		  if (temp_32!=temp_16) break;
		  get_params_value((uint8_t *) &bufferin[7], bufferin[6], len);
		  pktCmdx = bufferin[6];
		  pktCmdx++;
		  if(pktCmdx>12) pktCmdx = 0;
		  //printf("\r\nMasuk %d\r\n", pktCmdx);
	      break;
	}
		
		return ret;
}



/******************************************************************************
 *
 *  @fn       cc112x_run(void)
 *
 *  @brief   
 *            Runs as service, this function will handle data Tx & Rx
 *			  received data will store to rxBuffer
 *            and txBuffer for send data
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 *
 * @return   none
 *
 */
void cc112x_run(void)
{
	uint8_t temp_byte;
	uint8_t rx_byte = 0;
	//fprintf(f,"The start of the loop\n");
	time_t t;
	time (&t);
	struct tm * timeinfo = localtime (&t);
	openlog("cc1120Log", LOG_PID|LOG_CONS, LOG_USER);
	//scanning kwh and then adding them
	/*if ( kwh_loop <= 10){
		printf("Sending KWH data\n");
		syslog(LOG_INFO, "Sending KWH data\n");
		txBuffer[0] = 15; //length packet data
		txBuffer[1] = 0x02; //command code 
		*(uint16_t*)&txBuffer[2] =  gateway_ID; //(2 byte)
		*(uint16_t*)&txBuffer[4] = 0x0000; //(2 byte) id harusnya cuman 4 
		txBuffer[6] = 0xFF; 
		txBuffer[7] = 0xFF; //rssi
		txBuffer[8] = 0xFF; //sensor number
		txBuffer[9] = 0xFF; //radio channel
		txBuffer[10] = 0x14; //node type = ADE
		txBuffer[11] = 0x07;//in sec wakeup (2 byte)
		txBuffer[12] = 0x61;//in sec next wakeup (2 byte)
		txBuffer[13] = 0x28;
		txBuffer[14] = 0x1B;
		txBuffer[15] = 0x01;
		printf ("txbuffer ");
		syslog(LOG_INFO, "txbuffer ");
		for (i=0;i<=15;i++) {
			printf("%02X ", txBuffer[i]);
			//syslog(LOG_INFO, "%02X ", txBuffer[i]);
		}
		kwh_loop++;
		sleep(1);
	}*/
		// Infinite loop
	
	//fprintf(f,"Before spi read reg\n");
	/*	fprintf(f, "Before spi read reg %04d-%02d-%02d %02d:%02d:%02d\n", 
		timeinfo->tm_year+1900,
		timeinfo->tm_mon+1,
		timeinfo->tm_mday,
		timeinfo->tm_hour,
		timeinfo->tm_min,
		timeinfo->tm_sec
		);*/
	cc112xSpiReadReg(CC112X_MARC_STATUS1, &temp_byte, 1);
	if (( temp_byte == 0x07)||( temp_byte == 0x08)){
		// Flush TX FIFO
		trxSpiCmdStrobe(CC112X_SFTX);
		
		printf("trxspicmdstrobe is excecuted %04d-%02d-%02d %02d:%02d:%02d", 
		timeinfo->tm_year+1900,
		timeinfo->tm_mon+1,
		timeinfo->tm_mday,
		timeinfo->tm_hour,
		timeinfo->tm_min,
		timeinfo->tm_sec
		);
	}
	else if (( temp_byte == 0x09)||( temp_byte == 0x0a)){
		// Flush RX FIFO
		trxSpiCmdStrobe(CC112X_SFRX);
	}
	else if( temp_byte&0x80 ) {
		// Read number of bytes in RX FIFO
		cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rx_byte, 1);

		// Check that we have bytes in FIFO
		if(rx_byte != 0) {

			// Read MARCSTATE to check for RX FIFO error
			cc112xSpiReadReg(CC112X_MARCSTATE, &temp_byte, 1);

			// Mask out MARCSTATE bits and check if we have a RX FIFO error
			if((temp_byte & 0x1F) == RX_FIFO_ERROR) {
				// Flush RX FIFO
				trxSpiCmdStrobe(CC112X_SFRX);
			} 
			else {
				rx_byte &= 0x7F;
				// Read n bytes from RX FIFO
				cc112xSpiReadRxFifo(rxBuffer, rx_byte);

				// Check CRC ok (CRC_OK: bit7 in second status byte)
				// This assumes status bytes are appended in RX_FIFO
				// (PKT_CFG1.APPEND_STATUS = 1)
				// If CRC is disabled the CRC_OK field will read 1
				temp_byte = 0;
				if(rxBuffer[rx_byte - 1] & 0x80) {
					//cc112xSpiReadReg(CC112X_MARCSTATE, &temp_byte, 1);
					printf("After there is data on rx buffer %04d-%02d-%02d %02d:%02d:%02d\n", 
					timeinfo->tm_year+1900,
					timeinfo->tm_mon+1,
					timeinfo->tm_mday,
					timeinfo->tm_hour,
					timeinfo->tm_min,
					timeinfo->tm_sec
					);
					// Update packet counter
					//struct tm * timeinfo = gmtime (&t);
					packetCounter++;
					printf("%05d: %04d-%02d-%02d %02d:%02d:%02d - Received %d %s RSSI=%d.\r\n", 
					packetCounter,
					timeinfo->tm_year+1900,
					timeinfo->tm_mon+1,
					timeinfo->tm_mday,
					timeinfo->tm_hour,
					timeinfo->tm_min,
					timeinfo->tm_sec,
					//timeinfo->tm_ms,
					rx_byte, (rx_byte>1) ? "bytes" : "byte",rxBuffer[rx_byte - 2]-102);
				 				  
					processPacket((uint8_t*)&rxBuffer[1], txBuffer, rx_byte-3);
					
				}
			}
		}
		//fprintf(f,"After the whole data is processed\n");
	}
	else if( temp_byte == 0) {
					//os_dly_wait (1);
		//if(replyDly) replyDly--;
		//if (replyDly>0) return;
		if (txBuffer[0]==0) {
			if (tbuff_kwh_poll[0]==0) return;
			memcpy(txBuffer, tbuff_kwh_poll, tbuff_kwh_poll[0]+1);
			tbuff_kwh_poll[0] = 0;
		}
			
 		send_packet(txBuffer);
		//fprintf(f,"After send packet buffer\n");
		return;
	}
	
	trxSpiCmdStrobe(CC112X_SIDLE);
	
	wait_exp_val(CC112X_MARCSTATE, 0x41);
		
			
    // Set radio back in RX
    trxSpiCmdStrobe(CC112X_SRX);
		closelog();
}


void res_service( void)
{
  int i;
	
  while(1) {
	
    for (i=0;i<TH_NODES_MAX;i++) {
	   if (th_nodes[i].id!= (i+1)) continue;
	   if (th_nodes[i].status == STATUS_CLEARED) continue;
	   //fprintf(f, "counter:%d TH_ID_Selected:%04X\n", cc1120_TH_ID, cc1120_TH_ID_Selected[i]);
	   //res_th (location, th_nodes[i].median_t1, th_nodes[i].median_t2, th_nodes[i].median_t3, th_nodes[i].median_h, 11, srcAddr, mac_address_gateway);
		 if (th_nodes[i].median_t2 != 10794){
				th_nodes[i].median_t1 = th_nodes[i].median_t2;
			}
		 if (th_nodes[i].median_t3 != 10794){
				th_nodes[i].median_t1 = th_nodes[i].median_t3;
			}
	   res_th (location, th_nodes[i].median_t1, th_nodes[i].median_t2, th_nodes[i].median_t3, th_nodes[i].median_h, 11, th_nodes[i].id, gateway_ID);
	   th_nodes[i].status = STATUS_CLEARED;
	   //trap_th(location, Oid, gateway_trap_id, cc1120_TH_ID, dIn1, dIn2, humidity, median_temp , temp2, temp3, rssi);
	   //printf("Humidity : %d Temp 1 : %d Temp2 : %d Temp 3 : %d Din1 : %d Din2 : %d rssi : %d\n",
       //humidity, median_temp, temp2, temp3, dIn1, dIn2, rssi);
	}
	for (i=0;i<13;i++) {
	   if (phase_flags[i]== STATUS_CLEARED) continue;
		//	if(i > 1 && i < 6){
	   res_val_kwh (location_kwh, kwh_ID, gateway_ID, i);
		//}
		if(i==0){
			printf("data kwh\n");
		}
	   phase_flags[i]= STATUS_CLEARED;
	}

  }
  
}

void poll_kwh_service( void)
{
//  int i;
  struct timespec spec;
	Pondok_Pinang.start_hour = 11;
	Pondok_Pinang.close_hour = 23;	
	int hour;
	int min;
	int sec;
//  long   ms; // Milliseconds	
  
  while(1) {
	  
	clock_gettime(CLOCK_REALTIME, &spec);
	time_t t = time(NULL);
	struct tm *tm_struct = localtime(&t);
	hour = tm_struct->tm_hour;
	hour = 10;
	min = tm_struct->tm_min;
	sec = tm_struct->tm_sec;
	// send data every 5 minutes
	if ( min % 5 == 0 && sec == 0)
	{
		loop_th_id = 0;
		//flag_ir=0;
		//printf ("min is %d\n", min);
	}
	//printf("tim %d:%d\n", hour, min);
//	current_stamp = spec.tv_sec;

//	th_nodes[i].ts  = spec.tv_sec;
//	ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
//	current_stamp = (th_nodes[i].ts * 100) + (ms/10); //convert to 10* ms


	
	if ((spec.tv_sec%(t_wakeup_interval*60))<(6*20)) continue;
	if (tbuff_kwh_poll[0]) continue;
// add here if want to add ir kontrollerA
	int i =0;
	//printf("size %d\n", sizeof Change_freq_ir);
	
	if (infrared_loop > 0 && loop_th_id < TOTAL_TH_ID  && flag_ir==1)
	{
		//add kontrol infrared
		/*for(i=0;i<(sizeof Change_freq_ir);i++)
    {
      tbuff_kwh_poll[i] = Change_freq_ir[i];
    }*/
		int suhu_real = th_nodes[loop_th_id].th_set;
		if ( hour < Pondok_Pinang.start_hour || hour > Pondok_Pinang.close_hour)
		{
			suhu_real = 31;
		}
		int suhu_code =  suhu_real- 16;
	  printf("suhu code %d\n", suhu_code);	
		for(i=0;i<=68;i++)
    {
      tbuff_kwh_poll[i] = ir_command_save[loop_th_id][i];
      //tbuff_kwh_poll[i] = Panasonic_temp[suhu_code][i];
      //txBuffer[i] = Panasonic_temp[15][i];
      //printf("%02X ", txBuffer[i]);
    }
		*(uint16_t*)&tbuff_kwh_poll[4] =  th_nodes[loop_th_id].ir_id;
			infrared_loop--;
			if (infrared_loop == 0){
				infrared_loop = 4;
				loop_th_id++;
			}
			printf("infrared %d loop_th_id %d\n", infrared_loop, loop_th_id);
	}
	if (/*infrared_loop ==  0 && */loop_th_id >= TOTAL_TH_ID)
	{
		tbuff_kwh_poll[1] = COMM_VALUES_GET;
		memcpy((uint8_t *)&tbuff_kwh_poll[2], (uint8_t *)&gateway_ID, 2);
		memcpy((uint8_t *)&tbuff_kwh_poll[4], (uint8_t *)&kwh_ID, 2);
		tbuff_kwh_poll[6] = ADE_NODE_TYPE;
		tbuff_kwh_poll[7] = pktCmdx;
		tbuff_kwh_poll[0] = 7;
		//printf("\r\nKeluar: %d\r\n", pktCmdx);
	}
	sleep(1);

  }
  
}

void cc1120_service( void)
{
  freq_main = 1;
  //freq_main = 0;
  pktCmdx = 0;
  //freq_th = 23;
  remChannel = freq_main; 
  int i;
  
  //freq_main = 0;
  gateway_ID = 0;
	get_id("localhost","root","satunol10","paring","main");
	gateway_ID = mysql_id;
	printf(" gateway %d\n", gateway_ID);
  kwh_ID = 0x67C9;
  //mac_address_gateway = read_ints();
  //setup gpio pin to spi function
  wiringPiSetup();
  
  for(i=0; i<TH_NODES_MAX; i++) {
	th_nodes[i].id = cc1120_TH_ID_Selected[i];
	th_nodes[i].ir_id = cc1120_IR_ID_Selected[i];
	th_nodes[i].th_set = cc1120_TH_SET[i];
	th_nodes[i].ac_type = AC_TYPE[i];
	th_nodes[i].status = STATUS_CLEARED;
	th_nodes[i].loop_h = 0xff;
	th_nodes[i].loop_t1 = 0xff;
	th_nodes[i].loop_t2 = 0xff;
	th_nodes[i].loop_t3 = 0xff;
  }
  
	for (i=0;i<TH_NODES_MAX;i++)
	{
		if (th_nodes[i].id!= (i+1))
		{
			TOTAL_TH_ID = i;
			break;
		}
	}
	printf("total th id %d\n", TOTAL_TH_ID);
  pinMode(CC1120_MOSI, SPI_PIN);
  pinMode(CC1120_MISO, SPI_PIN);
  pinMode(CC1120_SCLK, SPI_PIN);
  pinMode(CC1120_SSEL, OUTPUT) ;  
  pinMode(CC1120_RST, OUTPUT) ;
  
  //getchar();
  wiringPiSPISetup(0, 5000000); //32*1000*1000);  
  digitalWrite(CC1120_SSEL,  HIGH) ;

  cc112x_hw_rst();
  //cc112x_init(23,0);// freq 410 Mhz + (1 Mhz * freq_main)
  cc112x_init(freq_main,0);// freq 410 Mhz + (1 Mhz * freq_main)
  memset(&txBuffer[0],0,sizeof(txBuffer));
  struct timespec spec;
	Pondok_Pinang.start_hour = 11;
	Pondok_Pinang.close_hour = 23;	
	int hour;
	int min;
	int sec;
	int flag_reset = 0;
//  long   ms; // Milliseconds	
  
	  
	
	
  while(1) {
		clock_gettime(CLOCK_REALTIME, &spec);
		time_t t = time(NULL);
		struct tm *tm_struct = localtime(&t);
		hour = tm_struct->tm_hour;
		//hour = 3;
		min = tm_struct->tm_min;
		sec = tm_struct->tm_sec;
		// send data every 5 minutes
		if ( min % 5 == 0 && sec == 0 && flag_reset == 1)
		{
			flag_ir=0;
			printf("masuk sini\n");
			sleep(1);
			//printf ("min is %d\n", min);
		}
		if ( min % 5 == 0 && sec == 5 && flag_reset == 0)
		{
			flag_reset  = 1;
		}	
  	if(flag_ir==0){
    for(loop_th_id=0;loop_th_id<TOTAL_TH_ID;loop_th_id++)
    {
			int suhu_real = th_nodes[loop_th_id].th_set;
			if ( hour < Pondok_Pinang.start_hour || hour > Pondok_Pinang.close_hour)
			{
				suhu_real = 31;
			}	
			printf("suhu real %d hour %d\n\n",suhu_real, hour);
      get_ir_command("localhost","root","satunol10","paring","ir_command", th_nodes[loop_th_id].ac_type, suhu_real);
      for(i=0;i<=68;i++)
      {
        ir_command_save[loop_th_id][i] = ir_command[i].value_byte;
      }
    }
		flag_ir++;
  }
 	/*f = fopen("/home/data.log", "a");
	if (f == NULL)
	{
    	printf("Error opening file!\n");
    	exit(1);
	}*/
	
	/* print some text */
	/*const char *text = "Why the result are diffrent";
	fprintf(f, "Some text: %s %d \n", text, datalog);*/
	
	/* communication handler */
	cc112x_run();
	//datalog++;
	//fclose(f);
  }
  
  /*int repeat;
	for(repeat=0;repeat<=100;repeat++)
	{
  // init cc112x radio
  cc112x_init(repeat,0);// freq 410 Mhz + (1 Mhz * repeat)

  printf("Run CC1120..\r\n");
  //getchar();

  // settup base frequency
    
	memcpy(&txBuffer[1],DUMMY_BUF,10);
	txBuffer[0]=10;
	int comm = 0;
	while(comm<6) {
				
		cc112x_run();
		comm++;
		sleep (60);
	}
	int frekuensi = (410 + repeat);
	printf("No File found frek = %d\n", frekuensi);  	
	}*/

}


/*******************************************************************************
********************************************************************************/

int main(int argc, char *argv[]) {
  int ret = 0;
  
  pthread_t thread_cc1120, thread_res, thread_poll_kwh;	// thread pointers
  
  /* Create independent threads each of which will execute function */
  pthread_create( &thread_cc1120, NULL, cc1120_service, NULL);
  pthread_create( &thread_res, NULL, res_service, NULL);
  pthread_create( &thread_poll_kwh, NULL, poll_kwh_service, NULL);

  pthread_join( thread_cc1120, NULL);
  pthread_join( thread_res, NULL);
  pthread_join( thread_poll_kwh, NULL);

  //int datalog = 0;
  while (1)
  {

  }

  return ret;
	
}
