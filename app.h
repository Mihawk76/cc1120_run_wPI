#ifndef __APP_H__
#define __APP_H__

enum {
	AllType =  0,	
	WiGtwLPC,			// 1
	WiGtwLinux,   // 2
	KwhGateway,	  // 3
	TH_IRGateway,	// 4
	na_05,			 	// 5
	na_06,			 	// 6
	na_07,			 	// 7
	na_08,			 	// 8
	na_09,			 	// 9
	na_10,			 	// 10
	na_11,			 	// 11
	na_12,			 	// 12
	na_13,			 	// 13
	na_14,			 	// 14
	na_15,			 	// 15
	na_16,			 	// 16
	WiT3H1, 			// 17
	WiT3, 				// 18
	WiLevelMeter,	// 19
	WiADE18Ch,		// 20
	WiIR,			    // 21
	WiIO446,			// 22
	WiIO824,      // 23 IOA
	WiI6O6,       // 24 IOB
  WiI12,	      // 25 IOC
	WiUART,
	WiIOD,	      // 27
	WiIOE,	      // 28
};

enum DEVICE_CODE_E{
  DEV_TH = 0,
  DEV_KWH,
	DEV_IO,
  DEV_IR,
 
  DEV_MAX
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
}TH_NODE_T;

typedef struct {
	uint16_t index;
	uint16_t io_id;
	uint8_t  inputNum;
	uint8_t  input[16];
	uint32_t input_timeCount[16];
	uint8_t  outputNum;
	uint8_t  outputSet[16];
	uint8_t  outputGet[16];
	uint32_t output_timeCount[16];
	time_t   ts;
	uint8_t  updated;
}IO_NODE;

typedef struct {
	uint32_t start_hour;
	uint32_t close_hour;
}STORE;

typedef struct{
	uint16_t ir_id;
	uint8_t default_temp;
	uint8_t set_temp;
	char *ac_type;
	time_t ts;
}IR_CONFIG;

typedef struct {
	uint8_t len;
	uint8_t buf[127];
}RADIO_DATA;

#define RDATA_COUNT 20
typedef struct {
	uint8_t head;
	uint8_t tail;
	RADIO_DATA rdata[RDATA_COUNT];
}CC120_BUFFER;

#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
	
#define SENSOR_TONLY    10010
#define SENSOR_BUSDUCT  10020
#define SENSOR_DINONLY  10030
#define VALUE_INVALID   10040

#define TH_NODES_MAX 20
#define IO_NODES_MAX 20
#define IR_NODES_MAX 20

#define STATUS_CLEARED 0
#define STATUS_UPDATED 1

extern TH_NODE_T th_nodes[TH_NODES_MAX];
extern IO_NODE io_nodes[IO_NODES_MAX];
extern IR_CONFIG ir_config[IR_NODES_MAX];

extern uint16_t kwh_ID;
extern uint16_t gateway_ID;

extern CC120_BUFFER hi_priority_tx;
extern CC120_BUFFER lo_priority_tx;


void flush_buffer(CC120_BUFFER *buf);
uint8_t inc_ptr (uint8_t ptr, uint8_t buff_len);
uint8_t next_buffer_free(CC120_BUFFER *buf);
uint8_t put_to_buffer (CC120_BUFFER *buf, uint8_t * buf_in, uint8_t len, uint8_t force);
uint8_t get_from_buffer (CC120_BUFFER *buf, uint8_t * buf_out);


#endif // __APP_H__
