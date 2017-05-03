#ifndef __APP_H__
#define __APP_H__

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
	uint16_t th_set;
	int start_operation;
	int end_operation;
  char* ac_type;
}TH_NODE_T;

typedef struct {
	uint16_t id;
	int start_operation;
	int end_operation;
	int channel;
	int type;
}IO_NODE;

typedef struct {
	int start_hour;
	int close_hour;
}STORE;

#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
	
#define SENSOR_TONLY    10010
#define SENSOR_BUSDUCT  10020
#define SENSOR_DINONLY  10030
#define VALUE_INVALID   10040

#define TH_NODES_MAX 20

extern IO_NODE io_nodes[16];
extern TH_NODE_T th_nodes[TH_NODES_MAX];

extern uint16_t gateway_ID;

#endif // __APP_H__
